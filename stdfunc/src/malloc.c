///////////////
/// @file malloc.c
/// @brief \c Malloc and \c Free definition.
///////////////

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define HEAP_MEMORY_SIZE 150 ///< How much memory we can use in total.
#define MEMORY_AVAILABLE 1   ///< Flag value if memory chunk is available.
#define MEMORY_USED 0        ///< Flag value if memory chunk is not available.

///////////////
/// @brief Only the storage referenced by the returned pointer is modified. No other storage locations are accessed by the call.
/// @details If the function reuses the same unit of storage released by a deallocation function (such as \c free), the functions are synchronized in such a way that the deallocation happens entirely before the next allocation.
///////////////
typedef struct CHUNK_HEADER {
    int32_t startAddress;    ///< Start address of this chunk.
    struct CHUNK_HEADER* next;      ///< Next pointer on free list.
    size_t size;             ///< The size of this chunk.
    bool isAvailable;        ///< Indicates if this chunk is \c MEMORY_AVAILABLE or \c MEMORY_USED.
    int32_t endAddress;      ///< End address of this chunk.
} chunk_header_t;

//! <b>[global]</b>
/// Global declaration of variables.
/// @code{.c}
chunk_header_t* g_chunkHeaderBegin;
static char g_buffer[ HEAP_MEMORY_SIZE ];
uint32_t g_heapSize;
/// @endcode
//! <b>[global]</b>

///////////////
/// @brief Static function that assign some variables on first call.
///////////////
static void init( void ) {
    //! <b>[assign]</b>
    /// Assign some variables on first call.
    /// @code{.c}
    g_heapSize = HEAP_MEMORY_SIZE;
    g_chunkHeaderBegin = (chunk_header_t*)&g_buffer;
    g_chunkHeaderBegin->next = NULL;
    g_chunkHeaderBegin->size = g_heapSize;
    g_chunkHeaderBegin->isAvailable = MEMORY_AVAILABLE;
    /// @endcode
    //! <b>[assign]</b>
}

///////////////
/// @brief Static function that reinitialize free space chunk.
/// @param[in] _currentChunk Pointer to chunk.
/// @param[in] _numberOfBytes Size of chunk.
///////////////
static void initNextChunk( chunk_header_t* _currentChunk, unsigned int _numberOfBytes ) {
    //! <b>[assign]</b>
    /// Assign some variables on re-initialization free space chunk.
    /// @code{.c}
    g_heapSize -= _numberOfBytes;
    _currentChunk->next = NULL;
    _currentChunk->size = g_heapSize;
    _currentChunk->isAvailable = MEMORY_AVAILABLE;
    /// @endcode
    //! <b>[assign]</b>
}

///////////////
/// @brief Allocates a block of size bytes of memory.
/// @details The content of the newly allocated block of memory is not initialized, remaining with indeterminate values.
/// @param[in] _numberOfBytes Size of the memory block, in bytes.
/// @return Returning a pointer to the beginning of the block. If the function failed to allocate the requested block of memory, a NULL pointer is returned.
///////////////
void* Malloc( unsigned int _numberOfBytes ) {
    //! <b>[init]</b>
    /// Do \c init on first call.
    /// @code{.c}
    static bool l_initFlag = false;

    if ( !l_initFlag ) {
        init();
        l_initFlag = true;
    }
    /// @endcode
    //! <b>[init]</b>

    //! <b>[declare]</b>
    /// Declare allocation size and begin of current chunk.
    /// @code{.c}
    int32_t l_allocSize = _numberOfBytes + sizeof( chunk_header_t );
    chunk_header_t* l_currentChunk = g_chunkHeaderBegin;
    /// @endcode
    //! <b>[declare]</b>

    while ( l_currentChunk ) {
        if ( l_currentChunk->isAvailable && (int)l_currentChunk->size >= l_allocSize ) {
            //! <b>[alloc]</b>
            /// Allocate chunk of memory.
            /// @code{.c}
            l_currentChunk->isAvailable = MEMORY_USED;
            l_currentChunk->size = l_allocSize;
            l_currentChunk->next = l_currentChunk + l_allocSize;

            initNextChunk( l_currentChunk->next, l_allocSize );
            /// @endcode
            //! <b>[alloc]</b>

            //! <b>[return]</b>
            /// Return pointer to memory region.
            /// @code{.c}
            return ( l_currentChunk + sizeof( chunk_header_t ) );
            /// @endcode
            //! <b>[return]</b>
        }

        //! <b>[step]</b>
        /// Step on next chunk if this not suits.
        /// @code{.c}
        l_currentChunk = l_currentChunk->next;
        /// @endcode
        //! <b>[step]</b>
    }

    //! <b>[return]</b>
    /// Return NULL if failed.
    /// @code{.c}
    return ( NULL );
    /// @endcode
    //! <b>[return]</b>
}

///////////////
/// @brief A block of memory previously allocated by a call to \c Malloc is deallocated, making it available again for further allocations.
/// @details If \c pointer does not point to a block of memory allocated with the above functions, it causes undefined behavior. If \c pointer is a null pointer, the function does nothing. Notice that this function does not change the value of \c pointer itself, hence it still points to the same (now invalid) location.
/// @param[in] _firstByte Pointer to a memory block previously allocated with \c Malloc.
///////////////
void Free( void* _firstByte ) {
    //! <b>[declare]</b>
    /// Declare start address of memory region and start address of allocation heap.
    /// @code{.c}
    chunk_header_t* l_mem = (chunk_header_t*)_firstByte - sizeof( chunk_header_t );
    chunk_header_t* l_currentChunk = g_chunkHeaderBegin;
    /// @endcode
    //! <b>[declare]</b>

    while ( l_currentChunk ) {
        if ( l_currentChunk == l_mem ) {
            //! <b>[deallocate]</b>
            /// Mark the block as being available.
            /// @code{.c}
            g_heapSize += l_currentChunk->size;

            l_currentChunk->isAvailable = MEMORY_AVAILABLE;
            /// @endcode
            //! <b>[deallocate]</b>

            break;
        }

        //! <b>[step]</b>
        /// Step on next chunk if this not suits.
        /// @code{.c}
        l_currentChunk = l_currentChunk->next;
        /// @endcode
        //! <b>[step]</b>
    }

    //! <b>[free]</b>
    /// Free a \c pointer to memory chunk. End of function.
    /// @code{.c}
    // _firstByte = NULL;
    /// @endcode
    //! <b>[free]</b>
}
