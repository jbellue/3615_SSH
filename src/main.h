#ifndef _MAIN_H
#define _MAIN_H

#include <libssh/libssh.h>
#include <libssh/threads.h>

typedef enum {
    STATE_NEW,
    STATE_SPLASH,
    STATE_WIFI_MENU,
    STATE_HOME,
    STATE_LANGUAGE
} state_t;

#endif