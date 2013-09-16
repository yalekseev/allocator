#include "fixed_size_blocks_allocator.h"

#include <iostream>

int main() {
    util::FixedSizeBlocksAllocator<2> allocator;

    void *p = allocator.alloc();
    std::cout << p << std::endl;
    allocator.free(p);

    return 0;
}
