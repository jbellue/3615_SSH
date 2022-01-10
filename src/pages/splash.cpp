#include "splash.h"
#include "splash_img.h"

Splash::Splash(Minitel* m) :
    Page {m} {
    _state = STATE_NEW;
}

MenuItem Splash::run() {
    switch (_state) {
    case STATE_NEW:
        showPage();
        _state = STATE_WAITING_FOR_INPUT;
        break;
    case STATE_WAITING_FOR_INPUT: {
        if (_minitel->getKeyCode() != 0) {
            return MenuItem::MenuOutput_HOME;
        }
    }
    default:
        break;
    }
    return MenuItem::MenuOutput_NONE;
}

void Splash::showPage() {
    _minitel->newScreen();

    for (uint16_t i = 0; i < sizeof(SPLASH); i++) {
        _minitel->writeByte(SPLASH[i]);
    }
}
