#ifndef _PAGE_SETTINGS_H
#define _PAGE_SETTINGS_H

#include <Arduino.h>
#include "page.h"

class LanguageSelection : public Page {
public:
    LanguageSelection(Minitel* m);
    MenuItem run();

private:
    void showTitle();
    void showPage();
    void showInput(const int num, const char* title);

    enum Input {
        INPUT_NONE,
        INPUT_ENGLISH,
        INPUT_FRENCH,
        INPUT_SPANISH
    };
    Input getInput();

    enum State {
        STATE_NEW,
        STATE_WAITING_FOR_INPUT
    };

    State _state;
};

#endif
