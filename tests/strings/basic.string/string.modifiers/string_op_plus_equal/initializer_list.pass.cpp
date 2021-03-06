//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <string>

// basic_string& operator+=(initializer_list<charT> il);

#include <string>
#include "tidystring.h"
#include <cassert>

#include "min_allocator.h"

int main()
{
#ifndef _LIBCPP_HAS_NO_GENERALIZED_INITIALIZERS
    {
        tidy::string s("123");
        s += {'a', 'b', 'c'};
        assert(s == "123abc");
    }
#if __cplusplus >= 201103L
    {
        typedef tidy::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
        S s("123");
        s += {'a', 'b', 'c'};
        assert(s == "123abc");
    }
#endif
#endif  // _LIBCPP_HAS_NO_GENERALIZED_INITIALIZERS
}
