//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>

// Call erase(const_iterator position) with end()

#if _LIBCPP_DEBUG >= 1

#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))

#include <vector>
#include "tidyvector.h"
#include <cassert>
#include <cstdlib>
#include <exception>

#include "min_allocator.h"

int main()
{
    {
    int a1[] = {1, 2, 3};
    tidy::vector<int> l1(a1, a1+3);
    tidy::vector<int>::const_iterator i = l1.end();
    l1.erase(i);
    assert(false);
    }
#if __cplusplus >= 201103L
    {
    int a1[] = {1, 2, 3};
    tidy::vector<int, min_allocator<int>> l1(a1, a1+3);
    tidy::vector<int, min_allocator<int>>::const_iterator i = l1.end();
    l1.erase(i);
    assert(false);
    }
#endif
}

#else

int main()
{
}

#endif
