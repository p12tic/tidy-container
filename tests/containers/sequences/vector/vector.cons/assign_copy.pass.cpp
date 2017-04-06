//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>

// vector& operator=(const vector& c);

#include <vector>
#include "tidyvector.h"
#include <cassert>
#include "test_allocator.h"
#include "min_allocator.h"

int main()
{
    {
        tidy::vector<int, test_allocator<int> > l(3, 2, test_allocator<int>(5));
        tidy::vector<int, test_allocator<int> > l2(l, test_allocator<int>(3));
        l2 = l;
        assert(l2 == l);
        assert(l2.get_allocator() == test_allocator<int>(3));
    }
    {
        tidy::vector<int, other_allocator<int> > l(3, 2, other_allocator<int>(5));
        tidy::vector<int, other_allocator<int> > l2(l, other_allocator<int>(3));
        l2 = l;
        assert(l2 == l);
        assert(l2.get_allocator() == other_allocator<int>(5));
    }
#if __cplusplus >= 201103L
    {
        tidy::vector<int, min_allocator<int> > l(3, 2, min_allocator<int>());
        tidy::vector<int, min_allocator<int> > l2(l, min_allocator<int>());
        l2 = l;
        assert(l2 == l);
        assert(l2.get_allocator() == min_allocator<int>());
    }
#endif
}
