#pragma once

#include "fixed_size_blocks_allocator.h"
#include "std_allocator.h"

#include <stdexcept>
#include <utility>
#include <vector>
#include <mutex>
#include <new>

namespace util {

class Allocator {
public:
    typedef std::size_t size_type;

private:
    class FixedAllocatorHandle {
    public:
        explicit FixedAllocatorHandle(std::size_t block_size = 0) : m_block_size(block_size), m_ptr(0) { }

        FixedAllocatorHandle(const FixedAllocatorHandle& other) = delete;

        FixedAllocatorHandle(FixedAllocatorHandle&& other) {
            m_block_size = other.m_block_size;
            m_ptr = other.m_ptr;
            other.m_block_size = 0;
            other.m_ptr = 0;
        }

        FixedAllocatorHandle& operator=(const FixedAllocatorHandle& other) = delete;

        FixedAllocatorHandle& operator=(FixedAllocatorHandle&& other) {
            FixedAllocatorHandle tmp(std::forward<FixedAllocatorHandle>(other));
            swap(tmp);
            return *this;
        }

        ~FixedAllocatorHandle() {
            delete m_ptr;
        }

        void * alloc() {
            std::lock_guard<std::mutex> lock(m_mutex);

            if (0 == m_ptr) {
                m_ptr = static_cast<FixedSizeBlocksAllocator *>(::malloc(sizeof(FixedSizeBlocksAllocator)));
                if (0 == m_ptr) {
                    throw std::bad_alloc();
                }

                new (m_ptr) FixedSizeBlocksAllocator(m_block_size);
            }

            return m_ptr->alloc();
        }

        void free(void *p) {
            std::lock_guard<std::mutex> lock(m_mutex);

            assert (0 != m_ptr);
            m_ptr->free(p);
        }

        void swap(FixedAllocatorHandle& other) {
            std::swap(m_block_size, other.m_block_size);
            std::swap(m_ptr, other.m_ptr);
        }

    private:
        std::mutex m_mutex;
        std::size_t m_block_size;
        FixedSizeBlocksAllocator *m_ptr;
    };

public:
    Allocator() {
        m_allocators.reserve(65);
        for (int i = 0; i < 65; ++i) {
            m_allocators.push_back(std::move(FixedAllocatorHandle(i)));
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
