#define MAX_CONNECTION_TRIES 3
#define CONNECTION_TIMEOUT   5

#define HYPER_IMPLEMENTATION
#include <hyper.h>

#include <stdio.h>
#include <unistd.h>

void usage(void);

void
get_file(
    const SOCKET        sock,
    const char          **argv,
    const size_t        argc
);

void
client_quit(
    const SOCKET        sock,
    const char          **argv,
    const size_t        argc
);

void
list_dir(
    const SOCKET        sock,
    const char          **argv,
    const size_t        argc
);

typedef void(*FUNCPTR)(
    SOCKET,
    const char**,
    const size_t
);

typedef struct _COMMAND
{
    const char          *command;
    FUNCPTR             execute;
} COMMAND, * PCOMMAND;

COMMAND command_list[3] = {
    {"get", &get_file},
    {"ls", &list_dir},
    {"quit", &client_quit}
};
unsigned long numCommands = 3;
