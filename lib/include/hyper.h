#ifndef _HYPER_H
#define _HYPER_H

#define MAX_COMMAND_LENGTH      1024
#define HYPER_SUCCESS           0
#define HYPER_FAILED           -1
#define HYPER_BAD_PARAMETER    -2

typedef int     HYPERSTATUS;

#include <hyper/hyper_network.h>
#include <hyper/hyper_file.h>

HYPERSTATUS
HyperMemAlloc(
    void                **src,
    size_t              size
);

HYPERSTATUS
HyperMemFree(
    void                *src
);

HYPERSTATUS
HyperMemRealloc(
    void                **src,
    size_t              size
);

#endif
