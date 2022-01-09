#include "languageSelection.h"
#include <Preferences.h>

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
        Languages i = getInput();
        if (i != LANGUAGES_LAST) {
            extern Preferences preferences;
            preferences.putUShort("language_index", (uint16_t)i);
            l10n.setLanguage(i);
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

Languages LanguageSelection::getInput() {
    unsigned long key = _minitel->getKeyCode();
    switch (key) {
        case '1':
            return ENGLISH;
        case '2':
            return FRENCH;
        case '3':
            return SPANISH;
        default:
            return LANGUAGES_LAST;
    }
}