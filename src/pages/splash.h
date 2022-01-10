#ifndef _PAGE_SPLASH_H
#define _PAGE_SPLASH_H

#include <Arduino.h>
#include "page.h"

class Splash : public Page {
public:
    Splash(Minitel* m);
    MenuItem run();

private:
    void showPage();

    enum State {
        STATE_NEW,
        STATE_WAITING_FOR_INPUT
    };

    State _state;
};

#endif
