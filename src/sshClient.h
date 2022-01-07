#ifndef _SSH_CLIENT_H
#define _SSH_CLIENT_H

#include <libssh/libssh.h>
#include <libssh_esp32.h>
#include <Minitel1B_Hard.h>

class SSHClient {
    public:
    SSHClient();

    enum class SSHStatus {
        OK,
        AUTHENTICATION_ERROR,
        GENERAL_ERROR
    };

    SSHStatus init(const char *host, const char *user, const char *password);
    SSHStatus connect_ssh(const char *host, const char *user, const char *password, const int verbosity);
    bool poll(Minitel* minitel);
    SSHStatus start_session(const char *host, const char *user, const char *password);
    void close_session();
    int interactive_shell_session();
    void close_channel();
    bool open_channel();
    void cleanup();

    private:
    ssh_session _session;
    ssh_channel _channel;
    char _readBuffer[256] = {'\0'};
    size_t getMinitelInput(unsigned long key, char* buffer);
};

#endif