//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>

// pointer data();

#include <vector>
#include "tidyvector.h"
#include <cassert>

#include "min_allocator.h"
#include "asan_testing.h"

int main()
{
    {
        tidy::vector<int> v;
        assert(v.data() == 0);
        assert(is_contiguous_container_asan_correct(v)); 
    }
    {
        tidy::vector<int> v(100);
        assert(v.data() == &v.front());
        assert(is_contiguous_container_asan_correct(v)); 
    }
#if __cplusplus >= 201103L
    {
        tidy::vector<int, min_allocator<int>> v;
        assert(v.data() == 0);
        assert(is_contiguous_container_asan_correct(v)); 
    }
    {
        tidy::vector<int, min_allocator<int>> v(100);
        assert(v.data() == &v.front());
        assert(is_contiguous_container_asan_correct(v)); 
    }
#endif
}
