#pragma once

#include <cassert>
#include <cstdlib>
#include <limits>
#include <new>

namespace util {

class AdjecentFixedSizeBlocksAllocator {
private:
    enum { NUM_BLOCKS = std::numeric_limits<unsigned char>::max() };

public:
    void * alloc() {
        if (0 == m_free_blocks) {
            return 0;
        }

        unsigned char *ptr = m_start + (m_free_head * m_block_size);
        m_free_head = *ptr;
        --m_free_blocks;

        return static_cast<void *>(ptr);
    }

    void free(void *p) {
        unsigned char *block_start = static_cast<unsigned char *>(p);

        assert(contains(p));

        std::size_t block_index = (block_start - m_start) / m_block_size;

        *block_start = m_free_head;
        m_free_head = block_index;
    }

    bool contains(void *p) const {
        unsigned char * ptr = static_cast<unsigned char *>(p);

        return (m_start <= ptr && ptr < m_start + NUM_BLOCKS * m_block_size);
    }

    void * begin() const {
        return m_start;
    }

    void * end() const {
        return m_start + NUM_BLOCKS * m_block_size;
    }

    void init(std::size_t block_size) {
        m_block_size = block_size;

        m_start = static_cast<unsigned char *>(::malloc(m_block_size * NUM_BLOCKS));
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

    bool full() {
        return (0 == m_free_blocks);
    }

private:
    unsigned char *m_start;
    std::size_t m_block_size;
    unsigned char m_free_head;
    unsigned char m_free_blocks;
};

} // namespace util
