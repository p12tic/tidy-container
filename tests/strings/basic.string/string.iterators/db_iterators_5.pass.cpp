//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <string>

// Add to iterator out of bounds.

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
    typedef tidy::string C;
    C c(1, '\0');
    C::iterator i = c.begin();
    i += 1;
    assert(i == c.end());
    i = c.begin();
    i += 2;
    assert(false);
    }
#if __cplusplus >= 201103L
    {
    typedef tidy::basic_string<char, std::char_traits<char>, min_allocator<char>> C;
    C c(1, '\0');
    C::iterator i = c.begin();
    i += 1;
    assert(i == c.end());
    i = c.begin();
    i += 2;
    assert(false);
    }
#endif
}

#else

int main()
{
}

#endif
