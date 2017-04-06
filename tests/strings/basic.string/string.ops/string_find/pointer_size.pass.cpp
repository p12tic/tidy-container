//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <string>

// size_type find(const charT* s, size_type pos = 0) const;

#include <string>
#include "tidystring.h"
#include <cassert>

#include "min_allocator.h"

template <class S>
void
test(const S& s, const typename S::value_type* str, typename S::size_type pos,
     typename S::size_type x)
{
    assert(s.find(str, pos) == x);
    if (x != S::npos)
    {
        typename S::size_type n = S::traits_type::length(str);
        assert(pos <= x && x + n <= s.size());
    }
}

template <class S>
void
test(const S& s, const typename S::value_type* str, typename S::size_type x)
{
    assert(s.find(str) == x);
    if (x != S::npos)
    {
        typename S::size_type n = S::traits_type::length(str);
        assert(0 <= x && x + n <= s.size());
    }
}

template <class S>
void test0()
{
    test(S(""), "", 0, 0);
    test(S(""), "abcde", 0, S::npos);
    test(S(""), "abcdeabcde", 0, S::npos);
    test(S(""), "abcdeabcdeabcdeabcde", 0, S::npos);
    test(S(""), "", 1, S::npos);
    test(S(""), "abcde", 1, S::npos);
    test(S(""), "abcdeabcde", 1, S::npos);
    test(S(""), "abcdeabcdeabcdeabcde", 1, S::npos);
    test(S("abcde"), "", 0, 0);
    test(S("abcde"), "abcde", 0, 0);
    test(S("abcde"), "abcdeabcde", 0, S::npos);
    test(S("abcde"), "abcdeabcdeabcdeabcde", 0, S::npos);
    test(S("abcde"), "", 1, 1);
    test(S("abcde"), "abcde", 1, S::npos);
    test(S("abcde"), "abcdeabcde", 1, S::npos);
    test(S("abcde"), "abcdeabcdeabcdeabcde", 1, S::npos);
    test(S("abcde"), "", 2, 2);
    test(S("abcde"), "abcde", 2, S::npos);
    test(S("abcde"), "abcdeabcde", 2, S::npos);
    test(S("abcde"), "abcdeabcdeabcdeabcde", 2, S::npos);
    test(S("abcde"), "", 4, 4);
    test(S("abcde"), "abcde", 4, S::npos);
    test(S("abcde"), "abcdeabcde", 4, S::npos);
    test(S("abcde"), "abcdeabcdeabcdeabcde", 4, S::npos);
    test(S("abcde"), "", 5, 5);
    test(S("abcde"), "abcde", 5, S::npos);
    test(S("abcde"), "abcdeabcde", 5, S::npos);
    test(S("abcde"), "abcdeabcdeabcdeabcde", 5, S::npos);
    test(S("abcde"), "", 6, S::npos);
    test(S("abcde"), "abcde", 6, S::npos);
    test(S("abcde"), "abcdeabcde", 6, S::npos);
    test(S("abcde"), "abcdeabcdeabcdeabcde", 6, S::npos);
    test(S("abcdeabcde"), "", 0, 0);
    test(S("abcdeabcde"), "abcde", 0, 0);
    test(S("abcdeabcde"), "abcdeabcde", 0, 0);
    test(S("abcdeabcde"), "abcdeabcdeabcdeabcde", 0, S::npos);
    test(S("abcdeabcde"), "", 1, 1);
    test(S("abcdeabcde"), "abcde", 1, 5);
    test(S("abcdeabcde"), "abcdeabcde", 1, S::npos);
    test(S("abcdeabcde"), "abcdeabcdeabcdeabcde", 1, S::npos);
    test(S("abcdeabcde"), "", 5, 5);
    test(S("abcdeabcde"), "abcde", 5, 5);
    test(S("abcdeabcde"), "abcdeabcde", 5, S::npos);
    test(S("abcdeabcde"), "abcdeabcdeabcdeabcde", 5, S::npos);
    test(S("abcdeabcde"), "", 9, 9);
    test(S("abcdeabcde"), "abcde", 9, S::npos);
    test(S("abcdeabcde"), "abcdeabcde", 9, S::npos);
    test(S("abcdeabcde"), "abcdeabcdeabcdeabcde", 9, S::npos);
    test(S("abcdeabcde"), "", 10, 10);
    test(S("abcdeabcde"), "abcde", 10, S::npos);
    test(S("abcdeabcde"), "abcdeabcde", 10, S::npos);
    test(S("abcdeabcde"), "abcdeabcdeabcdeabcde", 10, S::npos);
    test(S("abcdeabcde"), "", 11, S::npos);
    test(S("abcdeabcde"), "abcde", 11, S::npos);
    test(S("abcdeabcde"), "abcdeabcde", 11, S::npos);
    test(S("abcdeabcde"), "abcdeabcdeabcdeabcde", 11, S::npos);
    test(S("abcdeabcdeabcdeabcde"), "", 0, 0);
    test(S("abcdeabcdeabcdeabcde"), "abcde", 0, 0);
    test(S("abcdeabcdeabcdeabcde"), "abcdeabcde", 0, 0);
    test(S("abcdeabcdeabcdeabcde"), "abcdeabcdeabcdeabcde", 0, 0);
    test(S("abcdeabcdeabcdeabcde"), "", 1, 1);
    test(S("abcdeabcdeabcdeabcde"), "abcde", 1, 5);
    test(S("abcdeabcdeabcdeabcde"), "abcdeabcde", 1, 5);
    test(S("abcdeabcdeabcdeabcde"), "abcdeabcdeabcdeabcde", 1, S::npos);
    test(S("abcdeabcdeabcdeabcde"), "", 10, 10);
    test(S("abcdeabcdeabcdeabcde"), "abcde", 10, 10);
    test(S("abcdeabcdeabcdeabcde"), "abcdeabcde", 10, 10);
    test(S("abcdeabcdeabcdeabcde"), "abcdeabcdeabcdeabcde", 10, S::npos);
    test(S("abcdeabcdeabcdeabcde"), "", 19, 19);
    test(S("abcdeabcdeabcdeabcde"), "abcde", 19, S::npos);
    test(S("abcdeabcdeabcdeabcde"), "abcdeabcde", 19, S::npos);
    test(S("abcdeabcdeabcdeabcde"), "abcdeabcdeabcdeabcde", 19, S::npos);
    test(S("abcdeabcdeabcdeabcde"), "", 20, 20);
    test(S("abcdeabcdeabcdeabcde"), "abcde", 20, S::npos);
    test(S("abcdeabcdeabcdeabcde"), "abcdeabcde", 20, S::npos);
    test(S("abcdeabcdeabcdeabcde"), "abcdeabcdeabcdeabcde", 20, S::npos);
    test(S("abcdeabcdeabcdeabcde"), "", 21, S::npos);
    test(S("abcdeabcdeabcdeabcde"), "abcde", 21, S::npos);
    test(S("abcdeabcdeabcdeabcde"), "abcdeabcde", 21, S::npos);
    test(S("abcdeabcdeabcdeabcde"), "abcdeabcdeabcdeabcde", 21, S::npos);
}

template <class S>
void test1()
{
    test(S(""), "", 0);
    test(S(""), "abcde", S::npos);
    test(S(""), "abcdeabcde", S::npos);
    test(S(""), "abcdeabcdeabcdeabcde", S::npos);
    test(S("abcde"), "", 0);
    test(S("abcde"), "abcde", 0);
    test(S("abcde"), "abcdeabcde", S::npos);
    test(S("abcde"), "abcdeabcdeabcdeabcde", S::npos);
    test(S("abcdeabcde"), "", 0);
    test(S("abcdeabcde"), "abcde", 0);
    test(S("abcdeabcde"), "abcdeabcde", 0);
    test(S("abcdeabcde"), "abcdeabcdeabcdeabcde", S::npos);
    test(S("abcdeabcdeabcdeabcde"), "", 0);
    test(S("abcdeabcdeabcdeabcde"), "abcde", 0);
    test(S("abcdeabcdeabcdeabcde"), "abcdeabcde", 0);
    test(S("abcdeabcdeabcdeabcde"), "abcdeabcdeabcdeabcde", 0);
}

int main()
{
    {
    typedef tidy::string S;
    test0<S>();
    test1<S>();
    }
#if __cplusplus >= 201103L
    {
    typedef tidy::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test0<S>();
    test1<S>();
    }
#endif
}
