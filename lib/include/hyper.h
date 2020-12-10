/** \file hyper.h
 * \brief Main library header with common functions and variables
 *
 * The hyper.h header includes common functions and variables, such
 * as memory management functions and status codes. It also serves as 
 * a main header file for including the rest of the HyperProtocol library.
 */

#ifndef _HYPER_H
#define _HYPER_H

#define MAX_COMMAND_LENGTH      1024

#define HYPER_SUCCESS           0
#define HYPER_FAILED           -1
#define HYPER_BAD_PARAMETER    -2

typedef int     HYPERSTATUS;

#include <hyper/hyper_network.h>
#include <hyper/hyper_file.h>

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
HYPERSTATUS
HyperMemRealloc(
    void                **src,
    size_t              size
);

#endif
