#pragma once


template <std::size_t BLOCK_SIZE>
class FixedSizeBlocksAllocator {
public:
    void * alloc() {

    }

    void * free(void *p) {

    }

private:
    AdjecentFixedSizeBlocksAllocator<BLOCK_SIZE> *m_allocators;
};
