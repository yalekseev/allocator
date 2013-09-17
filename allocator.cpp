#include "allocator.h"

#include <iostream>

int main() {
    util::Allocator allocator;

    for (int size = 1; size < 65; ++size) {
        void *p = allocator.alloc(size);
        allocator.free(p, size);
    }

    return 0;
}
