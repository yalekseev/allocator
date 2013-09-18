#pragma once

#include "adjecent_fixed_size_blocks_allocator.h"
#include "std_allocator.h"

#include <vector>

namespace util {

class FixedSizeBlocksAllocator {
private:
    typedef AdjecentFixedSizeBlocksAllocator Allocator;

public:
    explicit FixedSizeBlocksAllocator(std::size_t block_size)
            : m_block_size(block_size)
            , m_try_this_allocator(0) { }

    void * alloc();

    void free(void *p);

private:
    std::size_t m_block_size;
    Allocator *m_try_this_allocator;
    std::vector<Allocator, impl::StdAllocator<Allocator> > m_allocators;
};

} // namespace util
