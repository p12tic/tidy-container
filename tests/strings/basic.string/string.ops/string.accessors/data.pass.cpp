//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <string>

// const charT* data() const;

#include <string>
#include "tidystring.h"
#include <cassert>

#include "min_allocator.h"

template <class S>
void
test(const S& s)
{
    typedef typename S::traits_type T;
    const typename S::value_type* str = s.data();
    if (s.size() > 0)
    {
        assert(T::compare(str, &s[0], s.size()) == 0);
        assert(T::eq(str[s.size()], typename S::value_type()));
    }
    else
        assert(T::eq(str[0], typename S::value_type()));
}

int main()
{
    {
    typedef tidy::string S;
    test(S(""));
    test(S("abcde"));
    test(S("abcdefghij"));
    test(S("abcdefghijklmnopqrst"));
    }
#if __cplusplus >= 201103L
    {
    typedef tidy::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(""));
    test(S("abcde"));
    test(S("abcdefghij"));
    test(S("abcdefghijklmnopqrst"));
    }
#endif
}
