#pragma once

#include <cassert>
#include <cstdlib>
#include <limits>
#include <new>

template <std::size_t BLOCK_SIZE>
class AdjecentFixedSizeBlocksAllocator {
private:
    enum { NUM_BLOCKS = std::numeric_limits<unsigned char>::max() };

public:
    void * alloc() {
        if (0 == m_free_blocks) {
            return 0;
        }

        unsigned char *ptr = m_start + (m_free_head * BLOCK_SIZE);
        m_free_head = *ptr;
        --m_free_blocks;

        return static_cast<void *>(ptr);
    }

    void free(void *p) {
        unsigned char *block_start = static_cast<unsigned char *>(p);

        assert(contains(p));

        std::size_t block_index = (block_start - m_start) / BLOCK_SIZE;

        *block_start = m_free_head;
        m_free_head = block_index;
    }

    bool contains(void *p) const {
        unsigned char * ptr = static_cast<unsigned char *>(p);

        return (m_start <= ptr && ptr < m_start + NUM_BLOCKS * BLOCK_SIZE);
    }

    void * begin() const {
        return m_start;
    }

    void * end() const {
        return m_start + NUM_BLOCKS * BLOCK_SIZE;
    }

    void init() {
        m_start = static_cast<unsigned char *>(::malloc(BLOCK_SIZE * NUM_BLOCKS));
        if (0 == m_start) {
            throw std::bad_alloc();
        }

        m_free_head = 0;
        m_free_blocks = NUM_BLOCKS;
        for (int i = 0; i < NUM_BLOCKS; ++i) {
            unsigned char *ptr = m_start + (i * BLOCK_SIZE);
            *ptr = i + 1;
        }
    }

private:
    unsigned char *m_start;
    unsigned char m_free_head;
    unsigned char m_free_blocks;
};
