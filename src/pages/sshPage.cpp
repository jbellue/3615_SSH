#include "pages/sshPage.h"
#include <iterator>

MenuItem SSHPage::run() {
    switch(_state) {
        case STATE_NEW:
            clearFields();
            showPage();
            _state = STATE_WAITING_FOR_INPUT;
            break;
        case STATE_WAITING_FOR_INPUT: {
            Input i = getInput();
            if (i == INPUT_ENVOI) {
                _state = STATE_CONNECTING;
            }
            else if (i == INPUT_SOMMAIRE) {
                cleanup();
                return MenuItem::MenuOutput_WIFI_MENU;
            }
            else if (i == INPUT_GUIDE) {
                cleanup();
                return MenuItem::MenuOutput_LANGUAGE;
            }
            break;
        }
        case STATE_CONNECTING: {
            _sshClient = new SSHClient();
            SSHClient::SSHStatus status = _sshClient->init(
                _inputs[FIELD_HOST].c_str(),
                _inputs[FIELD_USERNAME].c_str(),
                _inputs[FIELD_PASSWORD].c_str()
            );
            if (status == SSHClient::SSHStatus::OK) {
                _minitel->modeMixte();
                _minitel->echo(false);
                _state = STATE_CONNECTED;
            }
            else {
                _minitel->moveCursorDown(1);
                if (status == SSHClient::SSHStatus::AUTHENTICATION_ERROR) {
                    _minitel->println("ERREUR D'AUTHENTIFICATION");
                }
                else {
                    _minitel->println("ERREUR DE CONNEXION");
                }
                delay(2000);
                _state = STATE_NEW;
            }
            break;
        }
        case STATE_CONNECTED:
            if(!_sshClient->poll(_minitel)) {
                _state = STATE_DONE;
            }
            break;
        case STATE_DONE:
            cleanup();
            return MenuItem::MenuOutput_HOME;
    }
    return MenuItem::MenuOutput_NONE;
}

void SSHPage::cleanup() {
    if(_sshClient) {
        _sshClient->cleanup();
        delete _sshClient;
    }
    _minitel->modeVideotex();
    _minitel->echo(true);
}

void SSHPage::showPage() {
    _minitel->noCursor();
    _minitel->newScreen();
    _minitel->attributs(DOUBLE_HAUTEUR);
    _minitel->println("MENU client SSH");
    _minitel->attributs(GRANDEUR_NORMALE);

    // underline the title
    for (int i = 1; i <= 40; i++) {
        _minitel->writeByte(0x7E);
    }

    _minitel->moveCursorDown(1);
    _minitel->println("Hôte :");
    _minitel->print(".");
    _minitel->repeat(39);
    _minitel->moveCursorLeft(40);
    _minitel->println("Nom d'utilisateur :");
    _minitel->print(".");
    _minitel->repeat(39);
    _minitel->moveCursorLeft(40);
    _minitel->println("Mot de passe :");
    _minitel->print(".");
    _minitel->repeat(39);
    _minitel->moveCursorLeft(40);
    _minitel->cursor();

    displayBottomMenu();

    _minitel->moveCursorXY(0, 7);
}

void SSHPage::clearFields() {
    for(String &s : _inputs) {
        s = "";
    }
}

SSHPage::Input SSHPage::getInput() {
    unsigned long key = _minitel->getKeyCode();
    _field = FIELD_HOST;
    uint8_t x, y = 0;
    while(key != ENVOI && key != SOMMAIRE && key != GUIDE) {
        switch (key) {
            case RETOUR:
                switch (_field) {
                    case FIELD_HOST:
                        _field = FIELD_PASSWORD;
                        y = 11;
                        break;
                    case FIELD_USERNAME:
                        _field = FIELD_HOST;
                        y = 7;
                        break;
                    case FIELD_PASSWORD:
                        _field = FIELD_USERNAME;
                        y = 9;
                        break;
                }
                x = _inputs[_field].length() + 1;
                _minitel->moveCursorXY(x, y);
                break;
            case SUITE:
                switch (_field) {
                    case FIELD_HOST:
                        _field = FIELD_USERNAME;
                        y = 9;
                        break;
                    case FIELD_USERNAME:
                        _field = FIELD_PASSWORD;
                        y = 11;
                        break;
                    case FIELD_PASSWORD:
                        y = 7;
                        _field = FIELD_HOST;
                        break;
                }
                x = _inputs[_field].length() + 1;
                _minitel->moveCursorXY(x, y);
                break;
            case CORRECTION:
                if (_inputs[_field].length() > 0) {
                    _inputs[_field].remove(_inputs[_field].length() - 1, 1);
                }
                _minitel->moveCursorLeft(1);
                _minitel->print(".");
                _minitel->moveCursorLeft(1);
                break;
            case ANNULATION:
                _inputs[_field] = "";
                _minitel->moveCursorLeft(40);
                _minitel->print(".");
                _minitel->repeat(39);
                _minitel->moveCursorLeft(40);
                _minitel->moveCursorUp(1);
                break;
            case 0:
            case REPETITION:
            case GUIDE:
            case SOMMAIRE:
            case ENVOI:
            case CONNEXION_FIN:
                break;
            default:
                if (_field == FIELD_PASSWORD) {
                    _minitel->moveCursorLeft(1);
                    _minitel->print("*");
                }
                _inputs[_field] += char(key);
        }
        key = _minitel->getKeyCode();
    }

    switch (key) {
        case ENVOI:
            return INPUT_ENVOI;
        case GUIDE:
            return INPUT_GUIDE;
        case SOMMAIRE:
        default:
            return INPUT_SOMMAIRE;
    }
}

void SSHPage::displayBottomMenu() {
    _minitel->moveCursorXY(0, 23);
    _minitel->attributs(INVERSION_FOND);
    _minitel->print("Sommaire");
    _minitel->attributs(FOND_NORMAL);
    _minitel->println(" : Options WiFi");

    _minitel->attributs(INVERSION_FOND);
    _minitel->print("Guide");
    _minitel->attributs(FOND_NORMAL);
    _minitel->println(" : Language");
}