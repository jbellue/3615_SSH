#ifndef _PAGE_PAGE_H
#define _PAGE_PAGE_H

#include <Minitel1B_Hard.h>

class Page {
protected:
    Minitel* _minitel;
public:
    Page(Minitel* m) :
        _minitel(m) {};
    virtual ~Page() = default;

    virtual void run() {}
};

#endif