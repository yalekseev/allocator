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
    class FixedAllocatorHandle {
    public:
        explicit FixedAllocatorHandle(std::size_t block_size = 0) : m_block_size(block_size), m_ptr(0) { }

        FixedAllocatorHandle(const FixedAllocatorHandle& other) = delete;

        FixedAllocatorHandle(FixedAllocatorHandle&& other);

        FixedAllocatorHandle& operator=(const FixedAllocatorHandle& other) = delete;

        FixedAllocatorHandle& operator=(FixedAllocatorHandle&& other);

        ~FixedAllocatorHandle();

        void * alloc();

        void free(void *p);

        void swap(FixedAllocatorHandle& other);

    private:
        std::mutex m_mutex;
        std::size_t m_block_size;
        FixedSizeBlocksAllocator *m_ptr;
    };

public:
    Allocator();

    // Forbid copy construction
    Allocator(const Allocator& other) = delete;

    Allocator(Allocator&& other);

    // Forbid copy assignment
    Allocator& operator=(const Allocator& other) = delete;

    Allocator& operator=(Allocator&& other);

    // Use copmiler generated destructor
    ~Allocator() = default;

    void * alloc(size_type size);

    void free(void *p, size_type size);

private:
    std::vector<FixedAllocatorHandle, impl::StdAllocator<FixedAllocatorHandle> > m_allocators;
};

} // namespace util
