#pragma once

#include "adjecent_fixed_size_blocks_allocator.h"
#include "std_allocator.h"

#include <algorithm>
#include <cassert>
#include <vector>

namespace util {

template <std::size_t BLOCK_SIZE>
class FixedSizeBlocksAllocator {
private:
    typedef AdjecentFixedSizeBlocksAllocator<BLOCK_SIZE> Allocator;

public:
    FixedSizeBlocksAllocator() : m_try_this_allocator(0) { }

    void * alloc() {
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
        Allocator new_allocator;
        new_allocator.init();

        // Find a place for the new allocator and insert it
        auto it = std::lower_bound(
            m_allocators.begin(),
            m_allocators.end(),
            new_allocator,
            [](const Allocator &left, const Allocator &right){ return left.begin() < right.begin(); });

        auto new_it = m_allocators.insert(it, new_allocator);
        m_try_this_allocator = &(*new_it);
        return m_try_this_allocator->alloc();
    }

    void * free(void *p) {
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

private:
    Allocator *m_try_this_allocator;
    std::vector<Allocator, impl::StdAllocator<Allocator> > m_allocators;
};

} // namespace util
