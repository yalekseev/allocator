#pragma once

#include <limits>

namespace util {

class AdjecentFixedSizeBlocksAllocator {
private:
    enum { NUM_BLOCKS = std::numeric_limits<unsigned char>::max() };

public:
    // Forbid defult constructor
    AdjecentFixedSizeBlocksAllocator() = delete;

    explicit AdjecentFixedSizeBlocksAllocator(std::size_t block_size);

    // Forbid copy construction
    AdjecentFixedSizeBlocksAllocator(const AdjecentFixedSizeBlocksAllocator& other) = delete;
    // Allow move construction
    AdjecentFixedSizeBlocksAllocator(AdjecentFixedSizeBlocksAllocator&& other);
    // Forbid copy assignment
    AdjecentFixedSizeBlocksAllocator& operator=(const AdjecentFixedSizeBlocksAllocator& other) = delete;
    // Allow move assignment
    AdjecentFixedSizeBlocksAllocator& operator=(AdjecentFixedSizeBlocksAllocator&& other);

    ~AdjecentFixedSizeBlocksAllocator();

    void * alloc();

    void free(void *p);

    bool full() const;

    bool contains(void *p) const;

    void * begin() const;

    void * end() const;

    void swap(AdjecentFixedSizeBlocksAllocator& other);

private:
    unsigned char *m_start;
    std::size_t m_block_size;
    unsigned char m_free_head;
    unsigned char m_free_blocks;
};

} // namespace util
