#include "allocator.h"

int main() {
    util::Allocator allocator;

    for (int size = 1; size < 1024; ++size) {
        void *p = allocator.alloc(size);
        allocator.free(p, size);
    }

    return 0;
}
