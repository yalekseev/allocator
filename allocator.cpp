#include "fixed_size_blocks_allocator.h"
#include "std_allocator.h"
#include "allocator.h"

#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <utility>
#include <mutex>
#include <new>

namespace util {

/* Allocator::FixedAllocatorHandle */

Allocator::FixedAllocatorHandle::FixedAllocatorHandle(FixedAllocatorHandle&& other) {
    m_block_size = other.m_block_size;
    m_ptr = other.m_ptr;
    other.m_block_size = 0;
    other.m_ptr = 0;
}

Allocator::FixedAllocatorHandle& Allocator::FixedAllocatorHandle::operator=(Allocator::FixedAllocatorHandle&& other) {
    FixedAllocatorHandle tmp(std::forward<FixedAllocatorHandle>(other));
    swap(tmp);
    return *this;
}

Allocator::FixedAllocatorHandle::~FixedAllocatorHandle() {
    delete m_ptr;
}

void * Allocator::FixedAllocatorHandle::alloc() {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (0 == m_ptr) {
        m_ptr = static_cast<FixedSizeBlocksAllocator *>(::malloc(sizeof(FixedSizeBlocksAllocator)));
        if (0 == m_ptr) {
            throw std::bad_alloc();
        }

        // Construct new allocator
        new (m_ptr) FixedSizeBlocksAllocator(m_block_size);
    }

    return m_ptr->alloc();
}

void Allocator::FixedAllocatorHandle::free(void *p) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (0 == m_ptr) {
        throw std::logic_error("Trying to free block to not yet created allocator.");
    }

    m_ptr->free(p);
}

void Allocator::FixedAllocatorHandle::swap(Allocator::FixedAllocatorHandle& other) {
    std::swap(m_block_size, other.m_block_size);
    std::swap(m_ptr, other.m_ptr);
}

/* Allocator */

Allocator::Allocator() {
    m_allocators.reserve(65);
    for (int i = 0; i < 65; ++i) {
        m_allocators.push_back(FixedAllocatorHandle(i));
    }
}

void * Allocator::alloc(size_type size) {
    if (m_allocators.size() <= size) {
        return ::malloc(size);
    }

    return m_allocators[size].alloc();
}

void Allocator::free(void *p, size_type size) {
    if (m_allocators.size() <= size) {
        ::free(p);
        return;
    }

    m_allocators[size].free(p);
}

} // namespace util
