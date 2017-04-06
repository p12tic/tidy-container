//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <string>

// Compare iterators from different containers with <.

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
    S s1;
    S s2;
    bool b = s1.begin() < s2.begin();
    assert(false);
    }
#if __cplusplus >= 201103L
    {
    typedef tidy::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    S s1;
    S s2;
    bool b = s1.begin() < s2.begin();
    assert(false);
    }
#endif
}

#else

int main()
{
}

#endif
