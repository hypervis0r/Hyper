/** \file hyper.h
 * \brief Main library header with common functions and variables
 *
 * The hyper.h header includes common functions and variables, such
 * as memory management functions and status codes. It also serves as 
 * a main header file for including the rest of the HyperProtocol library.
 */

#ifndef _HYPER_H
#define _HYPER_H

/* Status Codes */
#define HYPER_SUCCESS           0
#define HYPER_FAILED           -1
#define HYPER_BAD_PARAMETER    -2

/* Typedefs */
typedef void*   HYPERFILE;
typedef int     HYPERSTATUS;

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
#define  MAX_COMMAND_LENGTH     1024

/* Platform Specifics */
#ifdef _WIN32
    #define _WINSOCK_DEPRECATED_NO_WARNINGS // Make WinSock STFU
    #include <winsock2.h> 
    #pragma comment (lib, "Ws2_32.lib") // Link to WinSock
    
    typedef int SOCKLEN;
#else
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <errno.h>

    // WinSock2 uses unsigned int for sockets, while POSIX uses int
    typedef int SOCKET;

    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;
    typedef socklen_t SOCKLEN;

    #define INVALID_SOCKET  -1
    #define SOCKET_ERROR    -1
#endif

#define CONNECTION_CLOSED   0

#define HYPERLIB static

/* Libc Includes */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

/*! 
 * \brief Allocate memory in a platform-agnostic way
 *
 * Allocate memory in a platform-agnostic way. We use HeapAlloc
 * for Windows, and malloc for POSIX. We do this instead of malloc
 * for Windows, as it reduces overhead.
 * 
 * \warning Objects allocated with HyperMemAlloc must be freed with HyperMemFree
 *
 * \param[out] src      The memory area to allocate
 * \param[in]  size     The amount of bytes to allocate
 * 
 * \result Returns HYPER_SUCCESS on success. If memory allocation 
 *      fails, returns HYPER_FAILED.
 *
 * \see HyperMemFree
 * \see HyperMemRealloc
 */
HYPERLIB
HYPERSTATUS
HyperMemAlloc(
    void                **src,
    size_t              size
);

/*! 
 * \brief Free memory in a platform-agnostic way
 *
 * Free memory in a platform-agnostic way. We use HeapFree
 * for Windows, and free for POSIX. We do this instead of free
 * for Windows, as calling free on a HeapAlloc-allocated function
 * causes heap corruption.
 *
 * \param[in] src       The memory area to free
 * 
 * \result Returns HYPER_SUCCESS on success. If freeing memory 
 *      fails, returns HYPER_FAILED.
 *
 * \see HyperMemAlloc
 * \see HyperMemRealloc
 */
HYPERLIB
HYPERSTATUS
HyperMemFree(
    void                *src
);

/*! 
 * \brief Reallocate memory in a platform-agnostic way
 *
 * Reallocate memory in a platform-agnostic way. We use HeapReAlloc
 * for Windows, and realloc for POSIX. We do this instead of realloc
 * for Windows, as it reduces overhead.
 *
 * \warning Objects reallocated with HyperMemRealloc must be freed with HyperMemFree
 *
 * \param[in,out] src   The memory area to reallocate
 * \param[out]    size  The size to reallocate memory to
 * 
 * \result Returns HYPER_SUCCESS on success. If reallocating memory 
 *      fails, returns HYPER_FAILED.
 *
 * \see HyperMemAlloc
 * \see HyperMemFree
 */
HYPERLIB
HYPERSTATUS
HyperMemRealloc(
    void                **src,
    size_t              size
);

/*!
 * \brief Read file into HYPERFILE buffer using standard C
 *
 * Reads a file from a FILE pointer into a HYPERFILE buffer. Uses 
 * Standard C for platform-agnostic file I/O.
 *
 * \param[in]  in       FILE pointer to an open file
 * \param[out] dataptr  Pointer to a NULL HYPERFILE buffer that holds the file
 * \param[out] sizeptr  Pointer to a size_t that holds the file size
 *
 * \result Returns HYPER_SUCCESS if successful. If something fails, returns 
 *      HYPER_FAILED.
 *
 * \see HyperWriteFileC
 * \see HyperReadFile
 * \see HyperWriteFile
 */
HYPERLIB
HYPERSTATUS 
HyperReadFileC(
    FILE                *in, 
    HYPERFILE           *dataptr, 
    size_t              *sizeptr
);

/*!
 * \brief Read file into HYPERFILE buffer using platform-specifics
 *
 * Reads a file from a file path into a HYPERFILE buffer. Uses 
 * platform-specific file I/O functions for faster speed.
 *
 * \param[in]  cpFilePath       char pointer to file path on disk
 * \param[out] lpBuffer         Pointer to a NULL HYPERFILE buffer that holds the file
 * \param[out] lpFileSize       Pointer to a size_t that holds the file size
 *
 * \result Returns HYPER_SUCCESS if successful. If something fails, returns 
 *      HYPER_FAILED.
 *
 * \see HyperWriteFile
 * \see HyperReadFileC
 * \see HyperWriteFileC
 */
HYPERLIB
HYPERSTATUS
HyperReadFile(
    const char          *cpFilePath,
    HYPERFILE           *lpBuffer,
    size_t              *lpFileSize
);

/*!
 * \brief Write file from HYPERFILE buffer to disk using standard C
 *
 * Writes a file from a HYPERFILE buffer to disk. Uses 
 * Standard C for platform-agnostic file I/O.
 *
 * \param[in/out]  in           FILE pointer to an open file
 * \param[in]      hfBuffer     HYPERFILE buffer thats holding file data
 * \param[in]      stFileSize   size_t that holds the file size
 *
 * \result Returns HYPER_SUCCESS if successful. If something fails, returns 
 *      HYPER_FAILED.
 *
 * \see HyperReadFileC
 * \see HyperWriteFile
 * \see HyperReadFile
 */
HYPERLIB
HYPERSTATUS
HyperWriteFileC(
    FILE                *pFile,
    const HYPERFILE     hfBuffer,
    const size_t        stFileSize
);

/*!
 * \brief Write file from HYPERFILE buffer to disk using platform-specifics
 *
 * Writes a file from a HYPERFILE buffer to disk. Uses 
 * platform-specific file I/O for faster speeds.
 *
 * \param[in]  cpFilePath   File path to write file to           
 * \param[in]  hfBuffer     HYPERFILE buffer thats holding file data
 * \param[in]  stFileSize   size_t that holds the file size
 *
 * \result Returns HYPER_SUCCESS if successful. If something fails, returns 
 *      HYPER_FAILED.
 *
 * \see HyperReadFile
 * \see HyperWriteFileC
 * \see HyperReadFileC
 */
HYPERLIB
HYPERSTATUS
HyperWriteFile(
    const char          *cpFilePath,
    const HYPERFILE     hfBuffer,
    const size_t        stFileSize
);

/*!
 * \brief Receive file from network into HYPERFILE buffer
 *
 * Recieves a file from a connected socket, and writes it into a HYPERFILE
 * buffer.
 *
 * \param[in]  sockServer   Open, connected socket to receive from
 * \param[out] lpBuffer     HYPERFILE buffer to write data to
 * \param[out] stFileSize   size_t to write file size to
 *
 * \result Returns HYPER_SUCCESS if successful. If something fails, returns 
 *      HYPER_FAILED.
 *
 * \see HyperSendFile
 */
HYPERLIB
HYPERSTATUS 
HyperReceiveFile(
    const SOCKET        sockServer, 
    HYPERFILE           *lpBuffer, 
    unsigned long       *ulSize
);

/*!
 * \brief Send file from HYPERFILE buffer over network
 *
 * Sends a file stored in a HYPERFILE buffer over the network to a
 * connected socket.
 *
 * \param[in]       sockServer   Open, connected socket to send to
 * \param[in/out]   lpBuffer     HYPERFILE buffer to write data to
 * \param[in]       stFileSize   size_t to write file size to
 *
 * \result Returns HYPER_SUCCESS if successful. If something fails, returns 
 *      HYPER_FAILED.
 *
 * \remarks lpBuffer is reallocated to (ulSize + SEND_BLOCK_SIZE)
 *
 * \see HyperReceiveFile
 */
HYPERLIB
HYPERSTATUS 
HyperSendFile(
    const SOCKET        sockServer, 
    HYPERFILE           *lpBuffer, 
    const unsigned long ulSize
);

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
HYPERLIB
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
HYPERLIB
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
HYPERLIB
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
HYPERLIB
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
HYPERLIB
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
HYPERLIB
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
HYPERLIB
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
HYPERLIB
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
HYPERLIB
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
HYPERLIB
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
HYPERLIB
HYPERSTATUS
HyperReceiveStatus(
    const SOCKET        sock,
    unsigned short      *status
);

#ifdef HYPER_IMPLEMENTATION

HYPERLIB
HYPERSTATUS
HyperMemAlloc(
    void                **src,
    size_t              size
)
{
    if (src == NULL)
        return HYPER_BAD_PARAMETER;

    void *data = NULL;

#ifdef _WIN32
    data = HeapAlloc(GetProcessHeap(), 0, size);
#else
    data = malloc(size);
#endif

    if (data == NULL)
        return HYPER_FAILED;

    *src = data;

    return HYPER_SUCCESS;
}

HYPERLIB
HYPERSTATUS
HyperMemFree(
    void                *src
)
{
    HYPERSTATUS iResult = HYPER_SUCCESS;

#ifdef _WIN32
    iResult = HeapFree(GetProcessHeap(), 0, src);
#else
    free(src);
#endif

    return iResult;
}

HYPERLIB
HYPERSTATUS
HyperMemRealloc(
    void                **src,
    size_t              size
)
{
    if (src == NULL)
        return HYPER_BAD_PARAMETER;

#ifdef _WIN32
    *src = HeapReAlloc(GetProcessHeap(), 0, *src, size);
#else
    *src = realloc(*src, size);
#endif

    if (*src == NULL)
        return HYPER_FAILED;

    return HYPER_SUCCESS;
}

/* Lol I stole all of the readall code from SO */
/* This function returns one of the READALL_ constants above.
   If the return value is zero == READALL_OK, then:
     (*dataptr) points to a dynamically allocated buffer, with
     (*sizeptr) chars read from the file.
     The buffer is allocated for one extra char, which is NUL,
     and automatically appended after the data.
   Initial values of (*dataptr) and (*sizeptr) are ignored.
*/
HYPERLIB
HYPERSTATUS 
HyperReadFileC(
    FILE                *in, 
    void                **dataptr, 
    size_t              *sizeptr)
{
    char  *data = NULL, *temp;
    size_t size = 0;
    size_t used = 0;
    size_t n;

    /* None of the parameters can be NULL. */
    if (in == NULL || dataptr == NULL || sizeptr == NULL)
        return READALL_INVALID;

    /* A read error already occurred? */
    if (ferror(in))
        return READALL_ERROR;

    while (1) {

        if (used + READALL_CHUNK + 1 > size) {
            size = used + READALL_CHUNK + 1;

            /* Overflow check. Some ANSI C compilers
               may optimize this away, though. */
            if (size <= used) {
                free(data);
                return READALL_TOOMUCH;
            }

            temp = realloc(data, size);
            if (temp == NULL) {
                free(data);
                return READALL_NOMEM;
            }
            data = temp;
        }

        n = fread(data + used, 1, READALL_CHUNK, in);
        if (n == 0)
            break;

        used += n;
    }

    if (ferror(in)) {
        free(data);
        return READALL_ERROR;
    }

    temp = realloc(data, used + 1);
    if (temp == NULL) {
        free(data);
        return READALL_NOMEM;
    }
    data = temp;
    data[used] = '\0';

    *dataptr = data;
    *sizeptr = used;

    return READALL_OK;
}

HYPERLIB
HYPERSTATUS 
HyperReceiveFile(
    const SOCKET        sockServer, 
    void                **lpBuffer, 
    unsigned long       *ulSize)
{
    HYPERSTATUS iResult = 0;

    unsigned long ulFileSize = 0;
	unsigned long ulWrittenSize = 0;
	unsigned long ulBytesWritten = 0;
    void *data = NULL;

    char cpSizeBuf[1024];
    memset(cpSizeBuf, 0, sizeof(cpSizeBuf));

	// Recieve file size from server
	recv(sockServer, cpSizeBuf, sizeof(cpSizeBuf), 0);
	ulFileSize = strtoull(cpSizeBuf, 0, 10);

    // If data is less than a block size long, lets not bloat the size lol
    if (ulFileSize < RECV_BLOCK_SIZE)
    {
        HyperMemAlloc(&data, ulFileSize);
        
        recv(sockServer, (char*)data, ulFileSize, 0);
        
        *lpBuffer = data;
        *ulSize = ulFileSize;
        return HYPER_SUCCESS;
    }

    // Prevent integer overflow leading to heap overflow, thx zenpai *_*
    if (ulFileSize >= ULONG_MAX-RECV_BLOCK_SIZE)
        return HYPER_FAILED;

    // Allocate data buffer
    iResult = HyperMemAlloc(&data, ulFileSize + RECV_BLOCK_SIZE);
    if (iResult == HYPER_FAILED)
        return HYPER_FAILED;

	// Recieve binary data from server, and write to buffer.
	while (ulWrittenSize < ulFileSize)
	{
        ulBytesWritten = recv(sockServer, (char*)(data) + ulWrittenSize, RECV_BLOCK_SIZE, 0);

		// If we have recieved final bit of data, finish writing and break.
		if (ulWrittenSize > ulFileSize)
            break;
		else
		    ulWrittenSize += ulBytesWritten;
	}

    // Set input buffer to recieved data.
    *lpBuffer = data;
    
    // Set ulSize to ulFileSize.
	*ulSize = ulFileSize;

    return 0;
}

HYPERLIB
HYPERSTATUS
HyperReadFile(
    const char          *cpFilePath,
    HYPERFILE           *lpBuffer,
    size_t              *lpFileSize)
{
    HYPERSTATUS iResult = 0;
    HYPERFILE data = NULL;
    size_t stBytesRead = 0;
    
#ifdef _WIN32
    HANDLE hFile = 0;
    LARGE_INTEGER liFileSize = {0};
    DWORD dwLength = 0;

    hFile = CreateFileA(
            cpFilePath,     /* lpFileName */ 
            GENERIC_READ,   /* dwDesiredAccess */
            0,              /* dwShareMode */
            0,              /* lpSecurityAttributes */
            OPEN_EXISTING,  /* dwCreationDisposition */
            FILE_ATTRIBUTE_READONLY,    /* dwFlagsAndAttributes */
            0               /* hTemplateFile */
    );
    if (hFile == NULL)
        return HYPER_FAILED;

    /* Get file size */
    iResult = GetFileSizeEx(hFile, &liFileSize);
    if (iResult == NULL)
    {
        CloseHandle(hFile);
        return HYPER_FAILED;
    }

    dwLength = liFileSize.QuadPart;

    /* Allocate Memory */
    iResult = HyperMemAlloc(&data, dwLength);
    if (iResult == HYPER_FAILED)
    {
        CloseHandle(hFile);
        return HYPER_FAILED;
    }

    /* Read file into buffer */
    iResult = ReadFile(
            hFile,          /* hFile */
            data,           /* lpBuffer */
            dwLength,       /* nNumberOfBytesToRead */
            &stBytesRead,   /* lpNumberOfBytesRead */
            0               /* lpOverlapped */
    );
    if (iResult == FALSE)
    {
        CloseHandle(hFile);
        HyperMemFree(data);
        return HYPER_FAILED;
    }
    
    CloseHandle(hFile);
#else
    struct stat st = {0};
    int filesize = 0;
    int fd = 0;

    /* Get File Size */
    iResult = stat(cpFilePath, &st);
    if (iResult == -1)
        return HYPER_FAILED;

    filesize = st.st_size;

    /* Open File Descriptor */
    fd = open(cpFilePath, O_RDONLY);
    if (fd == -1)
        return HYPER_FAILED;

    /* Allocate buffer */
    iResult = HyperMemAlloc(&data, filesize);
    if (iResult == HYPER_FAILED)
        return HYPER_FAILED;

    /* Read file into buffer */
    stBytesRead = read(fd, data, filesize);
    if ((int)stBytesRead == -1)
    {
        close(fd);
        HyperMemFree(data);
        return HYPER_FAILED;
    }

    close(fd);
#endif
    
    /* Set HYPERFILE and size_t parameter */
    *lpBuffer = data;
    *lpFileSize = stBytesRead;

    return HYPER_SUCCESS;
}

HYPERLIB
HYPERSTATUS
HyperWriteFileC(
    FILE                *pFile,
    const HYPERFILE     hfBuffer,
    const size_t        stFileSize
)
{
    HYPERSTATUS hsResult = 0;
    
    if (pFile)
    {
        hsResult = fwrite(hfBuffer, stFileSize, 1, pFile);
        if (hsResult < 1)
            return HYPER_FAILED;
    }

    return HYPER_SUCCESS;
}

HYPERLIB
HYPERSTATUS
HyperWriteFile(
    const char          *cpFilePath,
    const HYPERFILE     hfBuffer,
    const size_t        stFileSize
)
{
    HYPERSTATUS hsResult = 0;

#ifdef _WIN32
    HANDLE hFile = NULL;
    DWORD dwBytesWritten = 0;

    hFile = CreateFileA(
            cpFilePath,     /* lpFileName */ 
            GENERIC_READ,   /* dwDesiredAccess */
            0,              /* dwShareMode */
            0,              /* lpSecurityAttributes */
            CREATE_ALWAYS,  /* dwCreationDisposition */
            FILE_ATTRIBUTE_NORMAL,    /* dwFlagsAndAttributes */
            0               /* hTemplateFile */
    );
    if (hFile == NULL)
        return HYPER_FAILED;

    hsResult = WriteFile(
            hFile,          /* hFile */
            hfBuffer,       /* lpBuffer */
            stFileSize,     /* nNumberOfBytesToWrite */
            &dwBytesWritten,    /* lpNumberOfBytesWritten */
            NULL            /* lpOverlapped */
    );
    if (hsResult == NULL)
    {
        CloseHandle(hFile);
        return HYPER_FAILED;
    }

    CloseHandle(hFile);
#else
    int fd = 0;
    
    fd = open(cpFilePath, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    if (fd == -1)
        return HYPER_FAILED;

    hsResult = write(fd, hfBuffer, stFileSize);
    if (hsResult == -1)
    {
        close(fd);
        return HYPER_FAILED;
    }

    close(fd);
#endif

    return HYPER_SUCCESS;
}

HYPERLIB
HYPERSTATUS 
HyperSendFile(
    const SOCKET        sockServer, 
    HYPERFILE           *lpBuffer, 
    const unsigned long ulSize)
{
    HYPERSTATUS iResult = 0;
    unsigned long ulSentSize = 0;
    unsigned long ulBytesSent = 0;
    
    char fileSizeBuffer[FILESIZE_BUFFER_SIZE];
    memset(fileSizeBuffer, 0, FILESIZE_BUFFER_SIZE);

    // Add extra block due to memcpy scaring me
    iResult = HyperMemRealloc(lpBuffer, ulSize + SEND_BLOCK_SIZE);
    if (iResult == HYPER_FAILED)
        return HYPER_FAILED;

    // Send File Size to Peer
    snprintf(fileSizeBuffer, FILESIZE_BUFFER_SIZE, "%lu", ulSize);
    ulBytesSent = send(sockServer, fileSizeBuffer, FILESIZE_BUFFER_SIZE, 0);
    if ((int)ulBytesSent == SOCKET_ERROR)
        return SOCKET_ERROR;
  
    // If data is less than a block size long, lets not bloat the size lol
    if (ulSize < SEND_BLOCK_SIZE)
    {
        send(sockServer, (char*)(*lpBuffer), ulSize, 0);
        return HYPER_SUCCESS;
    }

    // Begin Sending file
    while(ulSentSize < ulSize)
    {
        // Copy data from buffer into block
        ulBytesSent = send(sockServer, (char*)(*lpBuffer)+ulSentSize, SEND_BLOCK_SIZE, 0);
        
        if ((int)ulBytesSent == SOCKET_ERROR)
            return SOCKET_ERROR;
        else
            ulSentSize += ulBytesSent;
    }
    
    return HYPER_SUCCESS;
}

HYPERLIB
HYPERSTATUS 
HyperNetworkInit(void)
{
    HYPERSTATUS iResult = 0;
#ifdef _WIN32
    WSADATA wsaData = { 0 };
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    return iResult;
}

HYPERLIB
HYPERSTATUS 
HyperCloseSocket(
    SOCKET              sock) 
{
    HYPERSTATUS iResult = 0;

#ifdef _WIN32
    iResult = closesocket(sock);
#else
    iResult = close(sock);
#endif

    return iResult;
}

HYPERLIB
HYPERSTATUS 
HyperSocketCleanup(void)
{
    HYPERSTATUS iResult = 0;
#ifdef _WIN32
    iResult = WSACleanup();
#endif
    return iResult;
}

HYPERLIB
HYPERSTATUS 
HyperSocket(
    SOCKET              *sock)
{
    SOCKET temp = 0;

    // Generate TCP IPv4 socket
    temp = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (temp == INVALID_SOCKET)
        return INVALID_SOCKET;

    // Set Socket
    *sock = temp;

    return HYPER_SUCCESS;
}

HYPERLIB
HYPERSTATUS 
HyperConnectServer(
    SOCKET              *sock, 
    const char          *cpServerIP, 
    const unsigned short usPort)
{
    struct addrinfo hints = { 0 }; 
    struct addrinfo *res = { 0 };
    int status = 0;
    SOCKET temp = 0;
    HYPERSTATUS iResult = 0;

    // Generate SOCKET object
    iResult = HyperSocket(&temp);
    if (iResult == INVALID_SOCKET)
        return INVALID_SOCKET;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    char portStr[1024];
    snprintf(portStr, 1024, "%hu", usPort);

    if ((status = getaddrinfo(cpServerIP, portStr, &hints, &res)) != 0)
        return HYPER_FAILED;

    // Connect to server
    iResult = connect(temp, res->ai_addr, res->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        HyperCloseSocket(temp);
        HyperSocketCleanup();
        return SOCKET_ERROR;
    }

    // Set socket
    *sock = temp;

    return HYPER_SUCCESS;
}

HYPERLIB
HYPERSTATUS 
HyperStartServer(
    SOCKET              *sock, 
    const unsigned short usPort)
{
    SOCKADDR_IN sin = {0};
    SOCKET temp = 0;
    HYPERSTATUS iResult = 0;

    // Generate Socket Object
    iResult = HyperSocket(&temp);
    if (temp == INVALID_SOCKET)
        return INVALID_SOCKET;

    // Enable SO_REUSEADDR
    // If something goes wrong, we won't have bind issues later
    int enable = 1;
	iResult = setsockopt(temp, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
    if (iResult == SOCKET_ERROR)
    {
        HyperCloseSocket(temp);
        HyperSocketCleanup();
        return SOCKET_ERROR;
    }

    // Set Server IP and Port
    // 0.0.0.0 == Bind to both local and public IPs
    sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr("0.0.0.0");
	sin.sin_port = htons(usPort);

    // Bind to address and port
    iResult = bind(temp, (SOCKADDR*)&sin, sizeof(sin));
    if (iResult == SOCKET_ERROR)
    {
        HyperCloseSocket(temp);
        HyperSocketCleanup();
        return SOCKET_ERROR;
    }

    // Set parameter output
    *sock = temp;

    return HYPER_SUCCESS;
}

HYPERLIB
HYPERSTATUS 
HyperServerListen(
    const SOCKET        sockServer, 
    SOCKET              *sockClient)
{
    HYPERSTATUS iResult = 0;
    SOCKET temp = 0;

    iResult = listen(sockServer, 0);
    if (iResult == SOCKET_ERROR)
        return SOCKET_ERROR;

    temp = accept(sockServer, 0, 0);
    if (temp == INVALID_SOCKET)
        return INVALID_SOCKET;

    *sockClient = temp;

    return HYPER_SUCCESS;
}

HYPERLIB
HYPERSTATUS 
HyperReceiveCommand(
    const SOCKET        sock, 
    char                *cpCommand,
    size_t              stMaxCommandLength
)
{
    HYPERSTATUS iResult = 0;

    // Recieve command
    iResult = recv(sock, cpCommand, stMaxCommandLength, 0);
    if (iResult == SOCKET_ERROR || iResult == CONNECTION_CLOSED)
        return HYPER_FAILED;

    return HYPER_SUCCESS;
}

HYPERLIB
HYPERSTATUS 
HyperSendCommand(
    const SOCKET        sock, 
    const char          *szCommand)
{
    HYPERSTATUS iResult = 0;

    if (!szCommand)
        return HYPER_BAD_PARAMETER;

    iResult = send(sock, szCommand, strlen(szCommand), 0);
    if (iResult == SOCKET_ERROR || iResult == CONNECTION_CLOSED)
        return SOCKET_ERROR;

    return HYPER_SUCCESS;
}

HYPERLIB
HYPERSTATUS
HyperSendStatus(
    const SOCKET         sock,
    const unsigned short status)
{
    HYPERSTATUS hsResult = 0;
    char buffer[255];
    memset(buffer, 0, sizeof(buffer));

    if (!sock)
        return HYPER_BAD_PARAMETER;

    snprintf(buffer, sizeof(buffer), "%u", status);
    
    hsResult = send(sock, buffer, sizeof(buffer), 0);
    if (hsResult == SOCKET_ERROR || hsResult == CONNECTION_CLOSED)
        return HYPER_FAILED;

    return HYPER_SUCCESS;
}

HYPERLIB
HYPERSTATUS
HyperReceiveStatus(
    const SOCKET        sock,
    unsigned short      *status)
{
    HYPERSTATUS hsResult = 0;
    unsigned short temp = 0;
    char buffer[255];
    memset(buffer, 0, sizeof(buffer));

    if (!sock || !status)
        return HYPER_BAD_PARAMETER;

    hsResult = recv(sock, buffer, sizeof(buffer), 0);
    if (hsResult == SOCKET_ERROR || hsResult == CONNECTION_CLOSED)
        return HYPER_FAILED;

    temp = (unsigned short)strtoul(buffer, NULL, 10);

    *status = temp;

    return HYPER_SUCCESS;
}

#endif

#endif
