//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>

// void push_back(value_type&& x);

#include <vector>
#include "tidyvector.h"
#include <cassert>
#include "MoveOnly.h"
#include "../../../stack_allocator.h"
#include "min_allocator.h"
#include "asan_testing.h"

int main()
{
// the container supports only trivial types
#if TIDY_EXPECTED_FAIL
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    {
        tidy::vector<MoveOnly> c;
        c.push_back(MoveOnly(0));
        assert(c.size() == 1);
        assert(is_contiguous_container_asan_correct(c)); 
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(1));
        assert(c.size() == 2);
        assert(is_contiguous_container_asan_correct(c)); 
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(2));
        assert(c.size() == 3);
        assert(is_contiguous_container_asan_correct(c)); 
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(3));
        assert(c.size() == 4);
        assert(is_contiguous_container_asan_correct(c)); 
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(4));
        assert(c.size() == 5);
        assert(is_contiguous_container_asan_correct(c)); 
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
    }
    {
        tidy::vector<MoveOnly, stack_allocator<MoveOnly, 15> > c;
        c.push_back(MoveOnly(0));
        assert(c.size() == 1);
        assert(is_contiguous_container_asan_correct(c)); 
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(1));
        assert(c.size() == 2);
        assert(is_contiguous_container_asan_correct(c)); 
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(2));
        assert(c.size() == 3);
        assert(is_contiguous_container_asan_correct(c)); 
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(3));
        assert(c.size() == 4);
        assert(is_contiguous_container_asan_correct(c)); 
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(4));
        assert(c.size() == 5);
        assert(is_contiguous_container_asan_correct(c)); 
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
    }
#if __cplusplus >= 201103L
    {
        tidy::vector<MoveOnly, min_allocator<MoveOnly>> c;
        c.push_back(MoveOnly(0));
        assert(c.size() == 1);
        assert(is_contiguous_container_asan_correct(c)); 
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(1));
        assert(c.size() == 2);
        assert(is_contiguous_container_asan_correct(c)); 
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(2));
        assert(c.size() == 3);
        assert(is_contiguous_container_asan_correct(c)); 
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(3));
        assert(c.size() == 4);
        assert(is_contiguous_container_asan_correct(c)); 
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(4));
        assert(c.size() == 5);
        assert(is_contiguous_container_asan_correct(c)); 
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
    }
#endif
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
#endif // TIDY_EXPECTED_FAIL
}
