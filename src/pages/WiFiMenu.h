#ifndef _PAGE_WIFI_MENU_H
#define _PAGE_WIFI_MENU_H

#include <Arduino.h>
#include "page.h"
#include "esp_wifi_types.h"
#include "esp_wifi.h"
#include <WiFi.h>

class WiFiMenu : public Page {
public:
    WiFiMenu(Minitel* m) :
        Page {m},
        _state(STATE_NEW),
        _input('\0') {}

    MenuItem run();

private:
    enum State {
        STATE_NEW,
        STATE_READY_TO_DISCONNECT,
        STATE_READY_TO_SCAN_AGAIN,
        STATE_SELECT_NETWORK,
        STATE_ENTER_PASSWORD,
        STATE_DONE
    };

    State showPage();

    unsigned long getSSIDSelection();
    void passwordForm();
    void connectToAP(const String SSID, const String password);

    static void WiFiEvent(system_event_id_t event, WiFiEventInfo_t info);

    State _state;
    String _ssid;
    String _password;
    wifi_auth_mode_t _authMode;
    int32_t _channel;

    char _input;
};

#endif