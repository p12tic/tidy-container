//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <string>

// Call front() on empty container.

#if _LIBCPP_DEBUG >= 1

#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))

#include <string>
#include "tidystring.h"
#include <cassert>
#include <iterator>
#include <exception>
#include <cstdlib>

#include "min_allocator.h"

int main()
{
    {
    typedef tidy::string S;
    S s(1, '\0');
    assert(s.front() == 0);
    s.clear();
    assert(s.front() == 0);
    assert(false);
    }
#if __cplusplus >= 201103L
    {
    typedef tidy::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    S s(1, '\0');
    assert(s.front() == 0);
    s.clear();
    assert(s.front() == 0);
    assert(false);
    }
#endif
}

#else

int main()
{
}

#endif
