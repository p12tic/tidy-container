//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>

// void reserve(size_type n);

#include <vector>
#include "tidyvector.h"
#include <cassert>
#include "../../../stack_allocator.h"
#include "min_allocator.h"
#include "asan_testing.h"

int main()
{
    {
        tidy::vector<int> v;
        v.reserve(10);
        assert(v.capacity() >= 10);
        assert(is_contiguous_container_asan_correct(v)); 
    }
    {
        tidy::vector<int> v(100);
        assert(v.capacity() == 100);
        v.reserve(50);
        assert(v.size() == 100);
        assert(v.capacity() == 100);
        v.reserve(150);
        assert(v.size() == 100);
        assert(v.capacity() == 150);
        assert(is_contiguous_container_asan_correct(v)); 
    }
    {
        tidy::vector<int, stack_allocator<int, 250> > v(100);
        assert(v.capacity() == 100);
        v.reserve(50);
        assert(v.size() == 100);
        assert(v.capacity() == 100);
        v.reserve(150);
        assert(v.size() == 100);
        assert(v.capacity() == 150);
        assert(is_contiguous_container_asan_correct(v)); 
    }
#if __cplusplus >= 201103L
    {
        tidy::vector<int, min_allocator<int>> v;
        v.reserve(10);
        assert(v.capacity() >= 10);
        assert(is_contiguous_container_asan_correct(v)); 
    }
    {
        tidy::vector<int, min_allocator<int>> v(100);
        assert(v.capacity() == 100);
        v.reserve(50);
        assert(v.size() == 100);
        assert(v.capacity() == 100);
        v.reserve(150);
        assert(v.size() == 100);
        assert(v.capacity() == 150);
        assert(is_contiguous_container_asan_correct(v)); 
    }
#endif
}
