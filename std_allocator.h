#pragma once

#include <cstdlib>
#include <utility>

namespace util {

namespace impl {

template <typename T>
class StdAllocator {
public:
    typedef T value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    template<typename U>
    struct rebind {
        typedef StdAllocator<U> other;
    };

    pointer allocate(size_type count) const {
        return reinterpret_cast<pointer>(::malloc(count * sizeof(T)));
    }

    void deallocate(pointer p, size_type n) const {
        ::free(p);
    }

    void construct(pointer p, const T& t) const {
        new (p) T(t);
    }

    void construct(pointer p, T&& t) const {
        new (p) T(std::forward<T>(t));
    }

    void destroy(pointer p) const {
        p->~T();
    }

    size_type max_size() const {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }
};

} // namespace impl

} // namespace util
