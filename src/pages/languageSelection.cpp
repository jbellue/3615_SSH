#include "languageSelection.h"

LanguageSelection::LanguageSelection(Minitel* m) :
    Page {m} {
    _state = STATE_NEW;
}

MenuItem LanguageSelection::run() {
    switch (_state) {
    case STATE_NEW:
        showPage();
        _minitel->echo(false);
        _state = STATE_WAITING_FOR_INPUT;
        break;
    case STATE_WAITING_FOR_INPUT: {
        Input i = getInput();
        if (i != INPUT_NONE) {
            // TODO actually select a language
            _minitel->echo(true);
            return MenuItem::MenuOutput_HOME;
        }
        // else ignore
        break;
    }
    default:
        break;
    }
    return MenuItem::MenuOutput_NONE;
}

void LanguageSelection::showTitle() {
    _minitel->noCursor();
    _minitel->newScreen();
    _minitel->attributs(DOUBLE_HAUTEUR);
    _minitel->println("Langue");
    _minitel->attributs(GRANDEUR_NORMALE);

    // underline the title
    for (int i = 1; i <= 40; i++) {
        _minitel->writeByte(0x7E);
    }
}


void LanguageSelection::showPage() {
    showTitle();

    _minitel->moveCursorDown(1);
    //TODO get a dynamic list of languages
    showInput(1, "English");
    showInput(2, "Français");
    showInput(3, "Espanol");
}

void LanguageSelection::showInput(const int num, const char* title) {
    _minitel->attributs(INVERSION_FOND);
    _minitel->print(String(num));
    _minitel->attributs(FOND_NORMAL);
    _minitel->print(" - ");
    _minitel->println(title);
}

LanguageSelection::Input LanguageSelection::getInput() {
    unsigned long key = _minitel->getKeyCode();
    switch (key) {
        case '1':
            return INPUT_ENGLISH;
        case '2':
            return INPUT_FRENCH;
        case '3':
            return INPUT_SPANISH;
        default:
            return INPUT_NONE;
    }
}