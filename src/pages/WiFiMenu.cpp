#include "WiFiMenu.h"
#include <string>

MenuItem WiFiMenu::run() {
    switch (_state) {
        case STATE_NEW:
            showPage();
            _state = STATE_WAITING_FOR_INPUT;
            break;
        case STATE_WAITING_FOR_INPUT: {
            unsigned long input = getInput();
            if (input == ENVOI) {
                _state = STATE_CHECK_INPUT;
                _minitel->moveCursorReturn(1);
                _minitel->noCursor();
            }
            else if (input == SOMMAIRE) {
                _state = STATE_DONE;
            }
        }
        break;
        case STATE_CHECK_INPUT: {
            const uint8_t action = checkInput();
            if (!action) {
                _state = STATE_NEW;
            }
            else {
                if(_page == PAGE_SELECT_NETWORK) {
                    _ssid = WiFi.SSID(action - 1);
                    _authMode = WiFi.encryptionType(action - 1);
                    _channel = WiFi.channel(action - 1);
                    _state = STATE_ENTER_PASSWORD;
                }
                else if(_page == PAGE_NO_NETWORK_FOUND) {
                    _state = STATE_NEW;
                }
                else { // PAGE_DISCONNECT
                    WiFi.disconnect();
                    _minitel->println(l10n.get(L10N_STRINGS::WIFI_PAGE_DISCONNECTING));
                    while(WiFi.isConnected()); // wait for disconnection
                    _state = STATE_NEW;
                }
            }
            break;
        }
        case STATE_ENTER_PASSWORD:
            passwordForm();
            break;
        case STATE_DONE:
            return MenuItem::MenuOutput_HOME;
    }
    return MenuItem::MenuOutput_NONE;
}

static uint8_t connectionStatus;

void WiFiMenu::passwordForm() {
    connectionStatus = SYSTEM_EVENT_MAX; // fake unattainable event
    _minitel->moveCursorDown(1);
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
                break;
            case SOMMAIRE:
                _state = STATE_DONE;
                return;
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
    connectToAP();

    while(connectionStatus == SYSTEM_EVENT_MAX); // wait until we get a connection or rejection
    _minitel->moveCursorDown(1);
    if (connectionStatus == SYSTEM_EVENT_STA_GOT_IP) {
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

void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {
    if(event == SYSTEM_EVENT_STA_DISCONNECTED) {
        connectionStatus = SYSTEM_EVENT_STA_DISCONNECTED;
    }
    else if (event == SYSTEM_EVENT_STA_GOT_IP) {
        connectionStatus = SYSTEM_EVENT_STA_GOT_IP;
    }
}

void WiFiMenu::connectToAP() {
    WiFi.onEvent(WiFiEvent);
    WiFi.disconnect(true);
    WiFi.mode(WIFI_MODE_STA);

    WiFi.begin(_ssid.c_str(), _password.c_str());
}

uint8_t WiFiMenu::checkInput() {
    if (_input == '1') {
        return 1;
    }
    if (_input == '2') {
        return 2;
    }
    return 0;
}

void WiFiMenu::showPage() {
    _minitel->noCursor();
    _minitel->newScreen();
    _minitel->attributs(DOUBLE_HAUTEUR);
    _minitel->println(l10n.get(L10N_STRINGS::WIFI_PAGE_TITLE));
    _minitel->attributs(GRANDEUR_NORMALE);

    // underline the title
    for (int i = 1; i <= 40; i++) {
        _minitel->writeByte(0x7E);
    }

    _minitel->moveCursorDown(1);
    if(WiFi.isConnected()) {
        _minitel->print(l10n.get(L10N_STRINGS::WIFI_PAGE_CONNECTED_TO));
        _minitel->println(WiFi.SSID());
        _minitel->moveCursorDown(1);
        _minitel->print("1 - ");
        _minitel->println(l10n.get(L10N_STRINGS::WIFI_PAGE_DISCONNECT));
        _page = PAGE_DISCONNECT;
    }
    else {
        _minitel->println(l10n.get(L10N_STRINGS::WIFI_PAGE_SCANNING));
        const int16_t n = WiFi.scanNetworks();
        _minitel->moveCursorUp(1);
        if (n == 0) {
            _minitel->println(l10n.get(L10N_STRINGS::WIFI_PAGE_NO_NETWORK_FOUND));
            _page = PAGE_NO_NETWORK_FOUND;
        }
        else {
            char buffer[255] = { '\0' };
            if (n == 1) {
                sprintf(buffer, l10n.get(L10N_STRINGS::WIFI_PAGE_ONE_NETWORK_FOUND).c_str());
            }
            else {
                sprintf(buffer, "%u %s", n, l10n.get(L10N_STRINGS::WIFI_PAGE_NETWORKS_FOUND).c_str());
            }
            _minitel->moveCursorDown(2);
            _minitel->println(buffer);
            for (int16_t i = 0; i < n; ++i) {
                sprintf(buffer, "%i : %s", i+1, WiFi.SSID(i).c_str());
                _minitel->println(buffer);
            }
            _page = PAGE_SELECT_NETWORK;
        }
    }
    _minitel->moveCursorDown(1);
    _minitel->cursor();
    _minitel->print(l10n.get(L10N_STRINGS::WIFI_PAGE_CHOICE_THEN));
    _minitel->attributs(INVERSION_FOND);
    _minitel->print("ENVOI");
    _minitel->attributs(FOND_NORMAL);
    _minitel->print(" : .");
    _minitel->moveCursorLeft(1);
}

unsigned long WiFiMenu::getInput() {
    const unsigned long key = _minitel->getKeyCode();
    switch (key) {
        case CORRECTION:
        case ANNULATION:
            _input = '\0';
            _minitel->moveCursorLeft(1);
            break;
        case ENVOI:
        case SOMMAIRE:
            return key;
        case 0:
        case '\n':
            // ignore
            break;
        default:
            _input = (char)key;
            _minitel->moveCursorLeft(1);
            break;
    }
    return 0;
}
