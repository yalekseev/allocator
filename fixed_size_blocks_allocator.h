#pragma once

#include "adjecent_fixed_size_blocks_allocator.h"
#include "std_allocator.h"

#include <vector>

namespace util {

class FixedSizeBlocksAllocator {
private:
    typedef AdjecentFixedSizeBlocksAllocator Allocator;

public:
    // Forbid default construction
    FixedSizeBlocksAllocator() = delete;

    explicit FixedSizeBlocksAllocator(std::size_t block_size)
            : m_block_size(block_size)
            , m_try_this_allocator(0) { }

    // Forbid copy construction
    FixedSizeBlocksAllocator(const FixedSizeBlocksAllocator& other) = delete;
    // Allow move construction
    FixedSizeBlocksAllocator(FixedSizeBlocksAllocator&& other);
    // Forbid copy assignment
    FixedSizeBlocksAllocator& operator=(const FixedSizeBlocksAllocator& other) = delete;
    // Allow move assignment
    FixedSizeBlocksAllocator& operator=(FixedSizeBlocksAllocator&& other);
    // Use compiler generated destructor
    ~FixedSizeBlocksAllocator() = default;

    /*! \brief Allocate a memory block of size m_block_size. */
    void * alloc();
    /*! \brief Return memory block back to the free pool. */
    void free(void *p);

private:
    std::size_t m_block_size;
    Allocator *m_try_this_allocator;
    std::vector<Allocator, impl::StdAllocator<Allocator> > m_allocators;
};

} // namespace util
