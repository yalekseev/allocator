#include "std_allocator.h"

#include <vector>

int main() {
    std::vector<int, util::impl::StdAllocator<int> > v;

    return 0;
}
