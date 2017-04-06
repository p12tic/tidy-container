//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <string>

// template<class charT, class traits, class Allocator>
//   bool operator<(const charT* lhs, const basic_string<charT,traits,Allocator>& rhs);

#include <string>
#include "tidystring.h"
#include <cassert>

#include "min_allocator.h"

template <class S>
void
test(const typename S::value_type* lhs, const S& rhs, bool x)
{
    assert((lhs < rhs) == x);
}

int main()
{
    {
    typedef tidy::string S;
    test("", S(""), false);
    test("", S("abcde"), true);
    test("", S("abcdefghij"), true);
    test("", S("abcdefghijklmnopqrst"), true);
    test("abcde", S(""), false);
    test("abcde", S("abcde"), false);
    test("abcde", S("abcdefghij"), true);
    test("abcde", S("abcdefghijklmnopqrst"), true);
    test("abcdefghij", S(""), false);
    test("abcdefghij", S("abcde"), false);
    test("abcdefghij", S("abcdefghij"), false);
    test("abcdefghij", S("abcdefghijklmnopqrst"), true);
    test("abcdefghijklmnopqrst", S(""), false);
    test("abcdefghijklmnopqrst", S("abcde"), false);
    test("abcdefghijklmnopqrst", S("abcdefghij"), false);
    test("abcdefghijklmnopqrst", S("abcdefghijklmnopqrst"), false);
    }
#if __cplusplus >= 201103L
    {
    typedef tidy::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test("", S(""), false);
    test("", S("abcde"), true);
    test("", S("abcdefghij"), true);
    test("", S("abcdefghijklmnopqrst"), true);
    test("abcde", S(""), false);
    test("abcde", S("abcde"), false);
    test("abcde", S("abcdefghij"), true);
    test("abcde", S("abcdefghijklmnopqrst"), true);
    test("abcdefghij", S(""), false);
    test("abcdefghij", S("abcde"), false);
    test("abcdefghij", S("abcdefghij"), false);
    test("abcdefghij", S("abcdefghijklmnopqrst"), true);
    test("abcdefghijklmnopqrst", S(""), false);
    test("abcdefghijklmnopqrst", S("abcde"), false);
    test("abcdefghijklmnopqrst", S("abcdefghij"), false);
    test("abcdefghijklmnopqrst", S("abcdefghijklmnopqrst"), false);
    }
#endif
}
