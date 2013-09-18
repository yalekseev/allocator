#include "fixed_size_blocks_allocator.h"
#include "allocator.h"

#include <stdexcept>
#include <cstdlib>
#include <utility>
#include <mutex>
#include <new>

namespace util {

/* Allocator::FixedAllocatorHandle */

Allocator::FixedAllocatorHandle::FixedAllocatorHandle(FixedAllocatorHandle&& other) {
    std::lock_guard<std::mutex> lock(other.m_mutex);

    m_block_size = other.m_block_size;
    m_allocator = other.m_allocator;
    other.m_block_size = 0;
    other.m_allocator = 0;
}

Allocator::FixedAllocatorHandle& Allocator::FixedAllocatorHandle::operator=(Allocator::FixedAllocatorHandle&& other) {
    FixedAllocatorHandle tmp(std::forward<FixedAllocatorHandle>(other));
    swap(tmp);
    return *this;
}

Allocator::FixedAllocatorHandle::~FixedAllocatorHandle() {
    delete m_allocator;
}

void * Allocator::FixedAllocatorHandle::alloc() {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (0 == m_allocator) {
        m_allocator = static_cast<FixedSizeBlocksAllocator *>(std::malloc(sizeof(FixedSizeBlocksAllocator)));
        if (0 == m_allocator) {
            throw std::bad_alloc();
        }

        // Construct new allocator
        new (m_allocator) FixedSizeBlocksAllocator(m_block_size);
    }

    return m_allocator->alloc();
}

void Allocator::FixedAllocatorHandle::free(void *p) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (0 == m_allocator) {
        throw std::logic_error("Trying to free block to not yet created allocator.");
    }

    m_allocator->free(p);
}

void Allocator::FixedAllocatorHandle::swap(Allocator::FixedAllocatorHandle& other) {
    std::swap(m_block_size, other.m_block_size);
    std::swap(m_allocator, other.m_allocator);
}

/* Allocator */

Allocator::Allocator() {
    // Allocate fixed allocator handle for each block size in the range 0..MY_MAX_SIZE
    m_allocators.reserve(MY_MAX_SIZE + 1);
    for (std::size_t block_size = 0; block_size < MY_MAX_SIZE + 1; ++block_size) {
        m_allocators.push_back(FixedAllocatorHandle(block_size));
    }
}

void * Allocator::alloc(size_type size) {
    if (m_allocators.size() <= size) {
        return std::malloc(size);
    }

    return m_allocators[size].alloc();
}

void Allocator::free(void *p, size_type size) {
    if (m_allocators.size() <= size) {
        std::free(p);
        return;
    }

    m_allocators[size].free(p);
}

} // namespace util
