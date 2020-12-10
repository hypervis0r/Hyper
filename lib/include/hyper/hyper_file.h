/*! \file hyper_file.h
 *  
 * \brief Contains all file I/O functions
 */

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
HYPERSTATUS 
HyperSendFile(
    const SOCKET        sockServer, 
    HYPERFILE           *lpBuffer, 
    const unsigned long ulSize
);

#endif
