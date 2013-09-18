#include "adjecent_fixed_size_blocks_allocator.h"

int main() {
    for (std::size_t block_size = 1; block_size < 1024; ++block_size) {
        util::AdjecentFixedSizeBlocksAllocator allocator;
        allocator.init(block_size);
    }
    return 0;
}
