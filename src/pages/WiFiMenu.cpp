#include "WiFiMenu.h"
#include "utils.h"

#include <string>

static system_event_id_t _connectionStatus;

MenuItem WiFiMenu::run() {
    switch (_state) {
    case STATE_NEW:
        _state = showPage();
        break;
    case STATE_READY_TO_DISCONNECT: {
        const unsigned long key = _minitel->getKeyCode();
        if (key == ENVOI) {
            WiFi.disconnect();
            //TODO add a message box
            _minitel->println(l10n.get(L10N_STRINGS::WIFI_PAGE_DISCONNECTING));
            while(WiFi.isConnected()); // wait for disconnection
            _state = STATE_NEW;
        }
        else if (key == SOMMAIRE) {
            _state = STATE_DONE;
        }
        break;
    }
    case STATE_READY_TO_SCAN_AGAIN: {
        const unsigned long key = _minitel->getKeyCode();
        if (key == ENVOI) {
            _state = STATE_NEW;
        }
        break;
    }
    case STATE_SELECT_NETWORK:
        // if ENVOI has been pressed and there is a valid SSID at _input-1
        if (ENVOI == getSSIDSelection()) {
            if (WiFi.SSID(ghettoAtoi(_input) - 1) != "") {
                _state = STATE_ENTER_PASSWORD;
            }
        }
        break;
    case STATE_ENTER_PASSWORD:
        passwordForm();
        break;
    case STATE_DONE:
        return MenuItem::MenuOutput_HOME;
    }
    return MenuItem::MenuOutput_NONE;
}

WiFiMenu::State WiFiMenu::showPage() {
    Page::showTitle(l10n.get(L10N_STRINGS::WIFI_PAGE_TITLE));

    if(WiFi.isConnected()) {
        _minitel->print(l10n.get(L10N_STRINGS::WIFI_PAGE_CONNECTED_TO));
        _minitel->println(WiFi.SSID());

        _minitel->moveCursorDown(2);
        _minitel->attributs(INVERSION_FOND);
        _minitel->print("ENVOI");
        _minitel->attributs(FOND_NORMAL);
        _minitel->println(l10n.get(L10N_STRINGS::WIFI_PAGE_DISCONNECT));
        _minitel->attributs(INVERSION_FOND);
        _minitel->print("SOMMAIRE");
        _minitel->attributs(FOND_NORMAL);
        _minitel->print(l10n.get(L10N_STRINGS::WIFI_PAGE_GO_BACK));
        return STATE_READY_TO_DISCONNECT;
    }

    _minitel->println(l10n.get(L10N_STRINGS::WIFI_PAGE_SCANNING));
    const int16_t n = WiFi.scanNetworks();
    _minitel->moveCursorUp(1);
    if (n == 0) {
        _minitel->println(l10n.get(L10N_STRINGS::WIFI_PAGE_NO_NETWORK_FOUND));
        _minitel->moveCursorDown(2);
        _minitel->attributs(INVERSION_FOND);
        _minitel->print("ENVOI");
        _minitel->attributs(FOND_NORMAL);
        _minitel->print(l10n.get(L10N_STRINGS::WIFI_PAGE_TO_SCAN_AGAIN));
        return STATE_READY_TO_SCAN_AGAIN;
    }

    char buffer[255] = { '\0' };
    if (n == 1) {
        sprintf(buffer, l10n.get(L10N_STRINGS::WIFI_PAGE_ONE_NETWORK_FOUND).c_str());
    }
    else {
        sprintf(buffer, "%hi %s", n, l10n.get(L10N_STRINGS::WIFI_PAGE_NETWORKS_FOUND).c_str());
    }
    _minitel->println(buffer);

    _minitel->moveCursorDown(2);
    for (int16_t i = 0; i < n; ++i) {
        sprintf(buffer, "%i : %s", i+1, WiFi.SSID(i).c_str());
        _minitel->println(buffer);
    }
    _minitel->moveCursorDown(1);
    _minitel->cursor();
    _minitel->print(l10n.get(L10N_STRINGS::WIFI_PAGE_CHOICE_THEN));
    _minitel->attributs(INVERSION_FOND);
    _minitel->print("ENVOI");
    _minitel->attributs(FOND_NORMAL);
    _minitel->print(" : .");
    _minitel->moveCursorLeft(1);

    return STATE_SELECT_NETWORK;
}

unsigned long WiFiMenu::getSSIDSelection() {
    const unsigned long key = _minitel->getKeyCode();
    switch (key) {
        case CORRECTION:
        case ANNULATION:
            _input = '\0';
            _minitel->print(".");
            _minitel->moveCursorLeft(1);
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            _input = (char)key;
            _minitel->moveCursorLeft(1);
            break;
        default:
            // ignore
            break;
    }
    return key;
}

void WiFiMenu::passwordForm() {
    _connectionStatus = SYSTEM_EVENT_MAX; // fake unattainable event
    _minitel->moveCursorReturn(2);
    _minitel->println(l10n.get(L10N_STRINGS::WIFI_PAGE_PASSWORD));
    _minitel->print(".");
    _minitel->repeat(39);
    _minitel->moveCursorLeft(40);
    _minitel->moveCursorUp(1);
    _password = "";
    _minitel->cursor();

    unsigned long key = _minitel->getKeyCode();
    while(key != ENVOI) {
        switch (key) {
            case RETOUR:
                _state = STATE_NEW;
                return;
            case 0:
            case '\n':
            case REPETITION:
            case GUIDE:
            case SUITE:
            case CONNEXION_FIN:
            case SOMMAIRE:
                break;
            case CORRECTION:
                if (_password.length() > 0) {
                    _password.remove(_password.length() - 1, 1);
                }
                _minitel->moveCursorLeft(1);
                _minitel->print(".");
                _minitel->moveCursorLeft(1);
                break;
            case ANNULATION:
                _password = "";
                _minitel->moveCursorLeft(40);
                _minitel->print(".");
                _minitel->repeat(39);
                _minitel->moveCursorLeft(40);
                _minitel->moveCursorUp(1);
                break;
            default:
                _minitel->moveCursorLeft(1);
                _minitel->print("*");
                _password += char(key);
        }
        key = _minitel->getKeyCode();
    }
    _minitel->moveCursorDown(1);
    _minitel->moveCursorLeft(40);
    _minitel->println(l10n.get(L10N_STRINGS::WIFI_PAGE_CONNECTING));
    connectToAP(WiFi.SSID(ghettoAtoi(_input) - 1), _password);

    // wait until we get a connection or rejection
    while(!(_connectionStatus == SYSTEM_EVENT_STA_DISCONNECTED || _connectionStatus == SYSTEM_EVENT_STA_GOT_IP)) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    _minitel->moveCursorDown(1);
    if (_connectionStatus == SYSTEM_EVENT_STA_GOT_IP) {
        _minitel->println(l10n.get(L10N_STRINGS::WIFI_PAGE_CONNECTED));
        _state = STATE_DONE;
        delay(1000);
    }
    else {
        _minitel->println(l10n.get(L10N_STRINGS::WIFI_PAGE_CONNECTION_FAILED));
        _minitel->println(l10n.get(L10N_STRINGS::WIFI_PAGE_CHECK_PASSWORD));
        delay(2000);
        _minitel->moveCursorUp(7);
        _minitel->clearScreenFromCursor();
    }
}


void WiFiMenu::connectToAP(const String ssid, const String password) {
    WiFi.onEvent(WiFiMenu::WiFiEvent);
    WiFi.disconnect(true);
    WiFi.mode(WIFI_MODE_STA);

    WiFi.begin(ssid.c_str(), password.c_str());
}

void WiFiMenu::WiFiEvent(system_event_id_t event, WiFiEventInfo_t info) {
    _connectionStatus = event;
}