#include "adjecent_fixed_size_blocks_allocator.h"

#include <cassert>
#include <cstdlib>
#include <limits>
#include <new>

namespace util {

void * AdjecentFixedSizeBlocksAllocator::alloc() {
    if (0 == m_free_blocks) {
        return 0;
    }

    unsigned char *ptr = m_start + (m_free_head * m_block_size);
    m_free_head = *ptr;
    --m_free_blocks;

    return static_cast<void *>(ptr);
}

void AdjecentFixedSizeBlocksAllocator::free(void *p) {
    unsigned char *block_start = static_cast<unsigned char *>(p);

    assert(contains(p));

    std::size_t block_index = (block_start - m_start) / m_block_size;

    *block_start = m_free_head;
    m_free_head = block_index;
}

bool AdjecentFixedSizeBlocksAllocator::full() const {
    return (0 == m_free_blocks);
}

bool AdjecentFixedSizeBlocksAllocator::contains(void *p) const {
    unsigned char *ptr = static_cast<unsigned char *>(p);

    return (m_start <= ptr && ptr < m_start + NUM_BLOCKS * m_block_size);
}

void * AdjecentFixedSizeBlocksAllocator::begin() const {
    return m_start;
}

void * AdjecentFixedSizeBlocksAllocator::end() const {
    return m_start + NUM_BLOCKS * m_block_size;
}

void AdjecentFixedSizeBlocksAllocator::init(std::size_t block_size) {
    m_block_size = block_size;

    m_start = static_cast<unsigned char *>(std::malloc(m_block_size * NUM_BLOCKS));
    if (0 == m_start) {
        throw std::bad_alloc();
    }

    m_free_head = 0;
    m_free_blocks = NUM_BLOCKS;
    for (int i = 0; i < NUM_BLOCKS; ++i) {
        unsigned char *ptr = m_start + (i * m_block_size);
        *ptr = i + 1;
    }
}

} // namespace util
