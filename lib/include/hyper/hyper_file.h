#ifndef _FILE_H
#define _FILE_H

/* Size of each input chunk to be
   read and allocate for. */
#ifndef  READALL_CHUNK
#define  READALL_CHUNK      262144
#endif
#define  READALL_OK         0  /* Success */
#define  READALL_INVALID   -1  /* Invalid parameters */
#define  READALL_ERROR     -2  /* Stream error */
#define  READALL_TOOMUCH   -3  /* Too much input */
#define  READALL_NOMEM     -4  /* Out of memory */

/* Set block sizes. 4096 is a nice number lol */
#define  SEND_BLOCK_SIZE    4096
#define  RECV_BLOCK_SIZE    4096

#define  FILESIZE_BUFFER_SIZE   1024

/* Typedefs */
typedef void*   HYPERFILE;

/* Platform Specifics */
#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <unistd.h>
#endif

/* Includes */
#include <hyper.h>
#include <hyper/hyper_network.h>

/* Libc Includes */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

HYPERSTATUS 
HyperReadFileC(
    FILE                *in, 
    HYPERFILE           *dataptr, 
    size_t              *sizeptr
);

HYPERSTATUS
HyperReadFile(
    const char          *cpFilePath,
    HYPERFILE           *lpBuffer,
    size_t              *lpFileSize
);

HYPERSTATUS
HyperWriteFileC(
    FILE                *pFile,
    const HYPERFILE     hfBuffer,
    const size_t        stFileSize
);

HYPERSTATUS
HyperWriteFile(
    const char          *cpFilePath,
    const HYPERFILE     hfBuffer,
    const size_t        stFileSize
);

HYPERSTATUS 
HyperReceiveFile(
    const SOCKET        sockServer, 
    HYPERFILE           *lpBuffer, 
    unsigned long       *ulSize
);

HYPERSTATUS 
HyperSendFile(
    const SOCKET        sockServer, 
    HYPERFILE           *lpBuffer, 
    const unsigned long ulSize
);

#endif
