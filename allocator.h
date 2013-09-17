#pragma once

#include "fixed_size_blocks_allocator.h"
#include "std_allocator.h"

#include <stdexcept>
#include <vector>
#include <new>

namespace util {

class Allocator {
public:
    typedef std::size_t size_type;

private:
    class FixedAllocatorHandle {
    public:
        FixedAllocatorHandle() : m_block_size(0), m_ptr(0) { }

        explicit FixedAllocatorHandle(std::size_t block_size) : m_block_size(block_size), m_ptr(0) { }

        ~FixedAllocatorHandle() {
            delete m_ptr;
        }

        void * alloc() {
            if (0 == m_ptr) {
                m_ptr = static_cast<FixedSizeBlocksAllocator *>(::malloc(sizeof(FixedSizeBlocksAllocator)));
                if (0 == m_ptr) {
                    throw std::bad_alloc();
                }
                
                new (m_ptr) FixedSizeBlocksAllocator(m_block_size);
            }

            m_ptr->alloc();
        }

        void free(void *p) {
            assert (0 != m_ptr);
            m_ptr->free(p);
        }

    private:
        // std::mutex
        std::size_t m_block_size;
        FixedSizeBlocksAllocator *m_ptr;
    };

public:
    Allocator() {
        m_allocators.reserve(65);
        for (int i = 0; i < 65; ++i) {
            m_allocators.push_back(FixedAllocatorHandle(i));
        }
    }

    void * alloc(size_type size) {
        if (m_allocators.size() <= size) {
            return ::malloc(size);
        }

        return m_allocators[size].alloc();
    }

    void free(void *p, size_type size) {
        if (m_allocators.size() <= size) {
            ::free(p);
            return;
        }

        m_allocators[size].free(p);
    }

private:
    std::vector<FixedAllocatorHandle, impl::StdAllocator<FixedAllocatorHandle> > m_allocators;
};

} // namespace util
