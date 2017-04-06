//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>

// void resize(size_type sz);

#include <vector>
#include "tidyvector.h"
#include <cassert>
#include "../../../stack_allocator.h"
#include "MoveOnly.h"
#include "min_allocator.h"
#include "asan_testing.h"

int main()
{
#if TIDY_EXPECTED_FAIL
    {
        tidy::vector<MoveOnly> v(100);
        v.resize(50);
        assert(v.size() == 50);
        assert(v.capacity() == 100);
        assert(is_contiguous_container_asan_correct(v)); 
        v.resize(200);
        assert(v.size() == 200);
        assert(v.capacity() >= 200);
        assert(is_contiguous_container_asan_correct(v)); 
    }
    {
        tidy::vector<MoveOnly, stack_allocator<MoveOnly, 300> > v(100);
        v.resize(50);
        assert(v.size() == 50);
        assert(v.capacity() == 100);
        assert(is_contiguous_container_asan_correct(v)); 
        v.resize(200);
        assert(v.size() == 200);
        assert(v.capacity() >= 200);
        assert(is_contiguous_container_asan_correct(v)); 
    }
#endif
    {
        tidy::vector<int> v(100);
        v.resize(50);
        assert(v.size() == 50);
        assert(v.capacity() == 100);
        assert(is_contiguous_container_asan_correct(v)); 
        v.resize(200);
        assert(v.size() == 200);
        assert(v.capacity() >= 200);
        assert(is_contiguous_container_asan_correct(v)); 
    }
    {
        tidy::vector<int, stack_allocator<int, 300> > v(100);
        v.resize(50);
        assert(v.size() == 50);
        assert(v.capacity() == 100);
        assert(is_contiguous_container_asan_correct(v)); 
        v.resize(200);
        assert(v.size() == 200);
        assert(v.capacity() >= 200);
        assert(is_contiguous_container_asan_correct(v)); 
    }
#if __cplusplus >= 201103L
#if TIDY_EXPECTED_FAIL
    {
        tidy::vector<MoveOnly, min_allocator<MoveOnly>> v(100);
        v.resize(50);
        assert(v.size() == 50);
        assert(v.capacity() == 100);
        assert(is_contiguous_container_asan_correct(v)); 
        v.resize(200);
        assert(v.size() == 200);
        assert(v.capacity() >= 200);
        assert(is_contiguous_container_asan_correct(v)); 
    }
#endif
#endif
}
