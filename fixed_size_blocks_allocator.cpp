#include "fixed_size_blocks_allocator.h"

#include <algorithm>
#include <cassert>

namespace util {

void * FixedSizeBlocksAllocator::alloc() {
    // Try a pointed allocator first
    if (m_try_this_allocator != 0 && !m_try_this_allocator->full()) {
        return m_try_this_allocator->alloc();
    }

    // Try to find an allocator with some free blocks
    for (auto &allocator : m_allocators) {
        if (!allocator.full()) {
            m_try_this_allocator = &allocator;
            return m_try_this_allocator->alloc();
        }
    }

    // No allocators with free blocks, create a new one
    Allocator new_allocator(m_block_size);

    // Find a place for the new allocator and insert it
    auto it = std::lower_bound(
        m_allocators.begin(),
        m_allocators.end(),
        new_allocator,
        [](const Allocator &left, const Allocator &right){ return left.begin() < right.begin(); });

    auto new_it = m_allocators.insert(it, std::move(new_allocator));
    m_try_this_allocator = &(*new_it);
    return m_try_this_allocator->alloc();
}

void FixedSizeBlocksAllocator::free(void *p) {
    // Find allocator where p belongs to
    auto it = std::lower_bound(
        m_allocators.begin(),
        m_allocators.end(),
        p,
        [](const Allocator & allocator, void *ptr){ return (allocator.begin() < ptr); });

    assert(it != m_allocators.end());
    assert(it->contains(p));

    // Return block to allocator
    it->free(p);

    m_try_this_allocator = &(*it);
}

} // namespace util
