#ifndef _PAGE_WIFI_MENU_H
#define _PAGE_WIFI_MENU_H

#include <Arduino.h>
#include "page.h"
#include "esp_wifi_types.h"
#include "esp_wifi.h"
#include <WiFi.h>


static uint8_t _connectionRetryCount;

class WiFiMenu : public Page {
public:
    WiFiMenu(Minitel* m) :
        Page {m},
        _state(STATE_NEW),
        _input('\0') { _connectionRetryCount = 0; }

    void run();

private:
    void showPage();
    unsigned long getInput();
    uint8_t checkInput();
    void passwordForm();
    void connectToAP();

    enum State {
        STATE_NEW,
        STATE_WAITING_FOR_INPUT,
        STATE_CHECK_INPUT,
        STATE_ENTER_PASSWORD,
        STATE_DONE
    };

    enum SubPage {
        PAGE_DISCONNECT,
        PAGE_SELECT_NETWORK,
        PAGE_NO_NETWORK_FOUND
    };

    State _state;
    SubPage _page;
    String _ssid;
    String _password;
    wifi_auth_mode_t _authMode;
    int32_t _channel;

    char _input;
};

void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);

#endif