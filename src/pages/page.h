#ifndef _PAGE_PAGE_H
#define _PAGE_PAGE_H

#include "menuItem.h"
#include <Minitel1B_Hard.h>
#include "l10n.h"

extern L10n l10n;

class Page {
protected:
    Minitel* _minitel;
public:
    Page(Minitel* m) :
        _minitel(m) {};
    virtual ~Page() = default;

    virtual MenuItem run() { return MenuItem::MenuOutput_HOME; }

    virtual void showTitle(String title) {
        _minitel->noCursor();
        _minitel->newScreen();
        _minitel->moveCursorRight((40 - title.length()) / 2);
        _minitel->attributs(DOUBLE_HAUTEUR);
        _minitel->println(title);
        _minitel->attributs(GRANDEUR_NORMALE);

        // underline the title
        _minitel->writeByte(0x7E);
        _minitel->repeat(39);
    };
};

#endif