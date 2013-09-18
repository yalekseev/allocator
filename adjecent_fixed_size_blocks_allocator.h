#pragma once

#include <limits>

namespace util {

class AdjecentFixedSizeBlocksAllocator {
private:
    enum { NUM_BLOCKS = std::numeric_limits<unsigned char>::max() };

public:
    void * alloc();

    void free(void *p);

    bool full() const;

    bool contains(void *p) const;

    void * begin() const;

    void * end() const;

    void init(std::size_t block_size);

private:
    unsigned char *m_start;
    std::size_t m_block_size;
    unsigned char m_free_head;
    unsigned char m_free_blocks;
};

} // namespace util
