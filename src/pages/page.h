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
};

#endif