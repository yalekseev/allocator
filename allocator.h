#pragma once

#include "std_allocator.h"

#include <vector>
#include <mutex>

namespace util {

class FixedSizeBlocksAllocator;

class Allocator {
public:
    typedef std::size_t size_type;

private:
    // Blocks of greater size are served by the global allocator
    enum { MY_MAX_SIZE = 64 };

    class FixedAllocatorHandle {
    public:
        explicit FixedAllocatorHandle(std::size_t block_size = 0)
            : m_block_size(block_size)
            , m_allocator(0) { }

        // Forbid copy construction
        FixedAllocatorHandle(const FixedAllocatorHandle& other) = delete;
        // Allow move construction
        FixedAllocatorHandle(FixedAllocatorHandle&& other);
        // Forbid copy assignment
        FixedAllocatorHandle& operator=(const FixedAllocatorHandle& other) = delete;
        // Allow move assignment
        FixedAllocatorHandle& operator=(FixedAllocatorHandle&& other);

        ~FixedAllocatorHandle();

        void * alloc();

        void free(void *p);

        void swap(FixedAllocatorHandle& other);

    private:
        std::mutex m_mutex;
        std::size_t m_block_size;
        FixedSizeBlocksAllocator *m_allocator;
    };

public:
    // Allow default construction
    Allocator();
    // Forbid copy construction
    Allocator(const Allocator& other) = delete;
    // Allow move construction
    Allocator(Allocator&& other);
    // Forbid copy assignment
    Allocator& operator=(const Allocator& other) = delete;
    // Allow move assignment;
    Allocator& operator=(Allocator&& other);
    // Use compiler generated destructor
    ~Allocator() = default;

    /*! Allocate memory block from the free pool. */
    void * alloc(size_type size);
    /*! Return memory block to the free pool.*/
    void free(void *p, size_type size);

private:
    std::vector<FixedAllocatorHandle, impl::StdAllocator<FixedAllocatorHandle> > m_allocators;
};

} // namespace util
