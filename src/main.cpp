#include "main.h"
#include <WiFi.h>
#include <Preferences.h>
#include "l10n.h"
#include "pages/splash.h"
#include "pages/WiFiMenu.h"
#include "pages/sshPage.h"
#include "pages/languageSelection.h"

#define NET_WAIT_MS 100


const unsigned int configSTACK = 51200;

static volatile state_t state;

Preferences preferences;
L10n l10n;

void newState(const state_t s) {
    state = s;
}

void handleStateNew(Minitel* minitel) {
    minitel->changeSpeed(4800);
    minitel->smallMode();
    vTaskDelay(NET_WAIT_MS / portTICK_PERIOD_MS);

    if(WiFi.isConnected()) {
        newState(STATE_SPLASH);
    }
    else {
        unsigned long length = 0;
        unsigned long time = millis();
        while (!WiFi.isConnected() && (length < 5000)) {  // Voir p.141
            length = millis() - time;
        }
        if(WiFi.isConnected()) {
            newState(STATE_SPLASH);
        }
        else {
            newState(STATE_WIFI_MENU);
        }
    }
}

void controlTask(void *pvParameter) {
    Minitel minitel = Minitel(Serial);

    l10n.setLanguage((Languages)preferences.getUShort("language_index", 0));

    vTaskDelay(NET_WAIT_MS / portTICK_PERIOD_MS);
    std::unique_ptr<Page> page = nullptr;

    while(1) {
        switch(state) {
            case STATE_NEW:
                handleStateNew(&minitel);
                break;
            case STATE_SPLASH:
                page = std::unique_ptr<Page>(new Splash(&minitel));
                break;
            case STATE_HOME:
                page = std::unique_ptr<Page>(new SSHPage(&minitel));
                break;
            case STATE_WIFI_MENU:
                page = std::unique_ptr<Page>(new WiFiMenu(&minitel));
                break;
            case STATE_LANGUAGE:
                page = std::unique_ptr<Page>(new LanguageSelection(&minitel));
                break;
            default:
                break;
        }
        if (page) {
            MenuItem ret;
            do {
                ret = page->run();
            }
            while(ret == MenuItem::MenuOutput_NONE);
            switch (ret) {
                case MenuItem::MenuOutput_WIFI_MENU:
                    newState(STATE_WIFI_MENU);
                    break;
                case MenuItem::MenuOutput_HOME:
                    newState(STATE_HOME);
                    break;
                case MenuItem::MenuOutput_LANGUAGE:
                    newState(STATE_LANGUAGE);
                    break;
                default:
                    // ignore
                    break;
            }
        }
    }
}

void setup() {
    state = STATE_NEW;
    preferences.begin("3615_SSH");

    xTaskCreatePinnedToCore(controlTask, "control", configSTACK, NULL, (tskIDLE_PRIORITY + 3), NULL, portNUM_PROCESSORS - 1);
    WiFi.begin();
}

void loop() {
    vTaskDelay(60000 / portTICK_PERIOD_MS);
}
