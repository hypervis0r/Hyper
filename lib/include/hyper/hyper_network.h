/*! \file hyper_network.h
 *
 * \brief Contains Hyper Networking functions and definitions
 */

#ifndef _NETWORK_H
#define _NETWORK_H

#define CONNECTION_CLOSED   0

#ifdef _WIN32
    #define _WINSOCK_DEPRECATED_NO_WARNINGS // Make WinSock STFU
    #include <winsock2.h> 
    #pragma comment (lib, "Ws2_32.lib") // Link to WinSock
    
    typedef int SOCKLEN;
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <netdb.h>
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

/*!
 * \brief Initializes the Hyper Network functions
 *
 * Initializes the Hyper Network functions so networking works properly.
 *
 * \result Returns HYPER_SUCCESS if successful, else returns HYPER_FAILED
 * 
 * \remarks As of right now, this is only needed for Windows applications. 
 *      However, this could change, so it is advised to use this function anyways.
 *
 * \see HyperSocketCleanup
 */ 

HYPERSTATUS
HyperNetworkInit(void);

/*!
 * \brief Closes a SOCKET object
 *
 * Closes a SOCKET object.
 *
 * \param[in] sock      Socket to close
 *
 * \result Returns HYPER_SUCCESS if successful, else returns HYPER_FAILED
 *
 * \see HyperSocket
 */
HYPERSTATUS
HyperCloseSocket(
    SOCKET              sock
);

/*!
 * \brief Cleans up Hyper Network functions for exiting.
 *
 * Cleans up Hyper Network functions. Once this function is called, networking 
 * will no longer be successful, so it should only be called when finished with
 * networking.
 *
 * \result Returns HYPER_SUCCESS if successful, else returns HYPER_FAILED
 *
 * \remarks As of right now, this is only needed for Windows applications. 
 *      However, this could change, so it is advised to use this function anyways.
 *
 * \see HyperNetworkInit
 */
HYPERSTATUS 
HyperSocketCleanup(void);

/*!
 * \brief Creates a valid SOCKET object for use with Hyper Networking.
 *
 * Creates a valid SOCKET object for use with Hyper Networking functions. Must be
 * connected using either connect() or accept()/listen().
 *
 * \param[out] sock     Pointer to SOCKET object to write socket into
 *
 * \result Returns HYPER_SUCCESS if successful, else returns HYPER_FAILED
 *  
 * \see HyperCloseSocket
 */
HYPERSTATUS
HyperSocket(
    SOCKET              *sock
);

/*!
 * \brief Connects to an open Hyper Server
 *
 * Connects to an open Hyper Server using the specified server IP and port.
 *
 * \param[out]  sock            Pointer to SOCKET object to connect with
 * \param[in]   cpServerIP      Char pointer containing IP address of server
 * \param[in]   usPort          Unsigned port number of server
 *
 * \result Returns HYPER_SUCCESS if successful, else returns HYPER_FAILED
 * 
 * \see HyperStartServer
 */
HYPERSTATUS
HyperConnectServer(
    SOCKET              *sock, 
    const char          *cpServerIP, 
    const unsigned short usPort
);

/*!
 * \brief Starts a Hyper Server at specified port
 *
 * Starts and initializes a Hyper Server at the specified port.
 *
 * \param[out]  sock            Pointer to SOCKET object to use for connections
 * \param[in]   usPort          Unsigned port number to bind to
 *
 * \result Returns HYPER_SUCCESS if successful, else returns HYPER_FAILED
 * 
 * \see HyperConnectServer
 * \see HyperServerListen
 */
HYPERSTATUS
HyperStartServer(
    SOCKET              *sock, 
    const unsigned short usPort
);

/*!
 * \brief Listens for connections to the server
 *
 * Listens for incoming connections to the server. Blocks thread until connection
 * is received.
 *
 * \param[in]   sockServer      SOCKET object to server
 * \param[out]  sockClient      Pointer to SOCKET object used to store the connected client
 *
 * \result Returns HYPER_SUCCESS if successful, else returns HYPER_FAILED
 *
 * \see HyperStartServer
 */
HYPERSTATUS
HyperServerListen(
    const SOCKET        sockServer, 
    SOCKET              *sockClient
);

/*!
 * \brief Recieve command from connection
 *
 * Recieves a command sent from connected peer.
 *
 * \param[in]   sock                    SOCKET object to receive from
 * \param[out]  cpCommand               Char pointer buffer to write command to
 * \param[in]   stMaxCommandLength      Max buffer size for cpCommand
 *
 * \result Returns HYPER_SUCCESS if successful, else returns HYPER_FAILED
 *
 * \see HyperSendCommand
 * \see HyperSendStatus
 * \see HyperReceiveStatus
 */
HYPERSTATUS
HyperReceiveCommand(
    const SOCKET        sock, 
    char                *cpCommand,
    size_t              stMaxCommandLength
);

/*!
 * \brief Send command to connection
 *
 * Sends a command to connected peer.
 *
 * \param[in]   sock                    SOCKET object to send to
 * \param[in]   cpCommand               Char pointer buffer storing command to send
 *
 * \result Returns HYPER_SUCCESS if successful, else returns HYPER_FAILED
 *
 * \see HyperReceiveCommand
 * \see HyperSendStatus
 * \see HyperReceiveStatus
 */
HYPERSTATUS
HyperSendCommand(
    const SOCKET        sock, 
    const char          *cpCommand
);

/*!
 * \brief Send Status Response to connection
 *
 * Sends a Status Response Code to connected peer.
 *
 * \param[in]   sock                    SOCKET object to send to
 * \param[in]   status                  unsigned short storing status to send
 *
 * \result Returns HYPER_SUCCESS if successful, else returns HYPER_FAILED
 *
 * \see HyperReceiveStatus
 * \see HyperSendCommand
 * \see HyperReceiveCommand
 */
HYPERSTATUS
HyperSendStatus(
    const SOCKET         sock,
    const unsigned short status
);

/*!
 * \brief Receive Status Response from connection
 *
 * Receives a Status Response Code from a connected peer.
 *
 * \param[in]   sock                    SOCKET object to receive from
 * \param[out]  status                  Pointer to unsigned short to store status in
 *
 * \result Returns HYPER_SUCCESS if successful, else returns HYPER_FAILED
 *
 * \see HyperSendStatus
 * \see HyperSendCommand
 * \see HyperReceiveCommand
 */
HYPERSTATUS
HyperReceiveStatus(
    const SOCKET        sock,
    unsigned short      *status
);

#endif
