#pragma once

// this allocator is based only on malloc and free -> no new or delete

#include <stdlib.h>
#include <new>
#include <limits>

template <class T> 
struct Mallocator {
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;

    template <class U> struct rebind { typedef Mallocator<U> other; };
    Mallocator() throw() {}
    Mallocator(const Mallocator&) throw() {}

    template <class U> Mallocator(const Mallocator<U>&) throw(){}

    ~Mallocator() throw() {}

    pointer address(reference x) const { return &x; }
    const_pointer address(const_reference x) const { return &x; }

    pointer allocate(size_type s, void const * = 0) {
        if (0 == s)
            return NULL;
        pointer temp = (pointer)malloc(s * sizeof(T)); 
        if (temp == NULL)
            throw std::bad_alloc();
        return temp;
    }

    void deallocate(pointer p, size_type) {
        free(p);
    }

    size_type max_size() const throw() { 
        return std::numeric_limits<size_t>::max() / sizeof(T); 
    }

    void construct(pointer p, const T& val) {
        new((void *)p) T(val);
    }

    void destroy(pointer p) {
        p->~T();
    }
};
