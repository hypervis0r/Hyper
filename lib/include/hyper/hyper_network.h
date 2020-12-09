#ifndef _NETWORK_H
#define _NETWORK_H

#define CONNECTION_CLOSED   0

#ifdef _WIN32
    #define _WINSOCK_DEPRECATED_NO_WARNINGS // Make WinSock STFU
    #include <winsock2.h> 
    #pragma comment (lib, "Ws2_32.lib") // Link to WinSock
    
    typedef int SOCKLEN;
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <errno.h>
    
    // WinSock2 uses unsigned int for sockets, while POSIX uses int
    typedef int SOCKET;

    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;
    typedef socklen_t SOCKLEN;

    #define INVALID_SOCKET  -1
    #define SOCKET_ERROR    -1
#endif

#include <hyper.h>
#include <string.h>

HYPERSTATUS
HyperNetworkInit(void);

HYPERSTATUS
HyperCloseSocket(
    SOCKET              sock
);

HYPERSTATUS 
HyperSocketCleanup(void);

HYPERSTATUS
HyperSocket(
    SOCKET              *sock
);

HYPERSTATUS
HyperConnectServer(
    SOCKET              *sock, 
    const char          *cpServerIP, 
    const unsigned short usPort
);

HYPERSTATUS
HyperStartServer(
    SOCKET              *sock, 
    const unsigned short usPort
);

HYPERSTATUS
HyperServerListen(
    const SOCKET        sockServer, 
    SOCKET              *sockClient
);

HYPERSTATUS
HyperReceiveCommand(
    const SOCKET        sock, 
    char                *cpCommand,
    size_t              stMaxCommandLength
);

HYPERSTATUS
HyperSendCommand(
    const SOCKET        sock, 
    const char          *cpCommand
);

HYPERSTATUS
HyperSendStatus(
    const SOCKET         sock,
    const unsigned short status
);

HYPERSTATUS
HyperReceiveStatus(
    const SOCKET        sock,
    unsigned short      *status
);

#endif
