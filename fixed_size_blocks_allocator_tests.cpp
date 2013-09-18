#include "fixed_size_blocks_allocator.h"

int main() {
    for (std::size_t block_size = 1; block_size < 1024; ++block_size) {
        util::FixedSizeBlocksAllocator allocator(block_size);

        void *p = allocator.alloc();
        allocator.free(p);
    }

    return 0;
}
