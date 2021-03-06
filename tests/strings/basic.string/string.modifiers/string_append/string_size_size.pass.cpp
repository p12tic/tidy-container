//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <string>

// basic_string<charT,traits,Allocator>&
//   append(const basic_string<charT,traits>& str, size_type pos, size_type n = npos);
//  the "= npos" was added for C++14

#include <string>
#include "tidystring.h"
#include <stdexcept>
#include <cassert>

#include "min_allocator.h"

template <class S>
void
test(S s, S str, typename S::size_type pos, typename S::size_type n, S expected)
{
    try
    {
        s.append(str, pos, n);
        assert(s.__invariants());
        assert(pos <= str.size());
        assert(s == expected);
    }
    catch (std::out_of_range&)
    {
        assert(pos > str.size());
    }
}

template <class S>
void
test_npos(S s, S str, typename S::size_type pos, S expected)
{
    try
    {
        s.append(str, pos);
        assert(s.__invariants());
        assert(pos <= str.size());
        assert(s == expected);
    }
    catch (std::out_of_range&)
    {
        assert(pos > str.size());
    }
}

int main()
{
    {
    typedef tidy::string S;
    test(S(), S(), 0, 0, S());
    test(S(), S(), 1, 0, S());
    test(S(), S("12345"), 0, 3, S("123"));
    test(S(), S("12345"), 1, 4, S("2345"));
    test(S(), S("12345"), 3, 15, S("45"));
    test(S(), S("12345"), 5, 15, S(""));
    test(S(), S("12345"), 6, 15, S("not happening"));
    test(S(), S("12345678901234567890"), 0, 0, S());
    test(S(), S("12345678901234567890"), 1, 1, S("2"));
    test(S(), S("12345678901234567890"), 2, 3, S("345"));
    test(S(), S("12345678901234567890"), 12, 13, S("34567890"));
    test(S(), S("12345678901234567890"), 21, 13, S("not happening"));

    test(S("12345"), S(), 0, 0, S("12345"));
    test(S("12345"), S("12345"), 2, 2, S("1234534"));
    test(S("12345"), S("1234567890"), 0, 100, S("123451234567890"));

    test(S("12345678901234567890"), S(), 0, 0, S("12345678901234567890"));
    test(S("12345678901234567890"), S("12345"), 1, 3, S("12345678901234567890234"));
    test(S("12345678901234567890"), S("12345678901234567890"), 5, 10,
         S("123456789012345678906789012345"));
    }
#if __cplusplus >= 201103L
    {
    typedef tidy::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(), S(), 0, 0, S());
    test(S(), S(), 1, 0, S());
    test(S(), S("12345"), 0, 3, S("123"));
    test(S(), S("12345"), 1, 4, S("2345"));
    test(S(), S("12345"), 3, 15, S("45"));
    test(S(), S("12345"), 5, 15, S(""));
    test(S(), S("12345"), 6, 15, S("not happening"));
    test(S(), S("12345678901234567890"), 0, 0, S());
    test(S(), S("12345678901234567890"), 1, 1, S("2"));
    test(S(), S("12345678901234567890"), 2, 3, S("345"));
    test(S(), S("12345678901234567890"), 12, 13, S("34567890"));
    test(S(), S("12345678901234567890"), 21, 13, S("not happening"));

    test(S("12345"), S(), 0, 0, S("12345"));
    test(S("12345"), S("12345"), 2, 2, S("1234534"));
    test(S("12345"), S("1234567890"), 0, 100, S("123451234567890"));

    test(S("12345678901234567890"), S(), 0, 0, S("12345678901234567890"));
    test(S("12345678901234567890"), S("12345"), 1, 3, S("12345678901234567890234"));
    test(S("12345678901234567890"), S("12345678901234567890"), 5, 10,
         S("123456789012345678906789012345"));
    }
#endif
    {
    typedef tidy::string S;
    test_npos(S(), S(), 0, S());
    test_npos(S(), S(), 1, S());
    test_npos(S(), S("12345"), 0, S("12345"));
    test_npos(S(), S("12345"), 1, S("2345"));
    test_npos(S(), S("12345"), 3, S("45"));
    test_npos(S(), S("12345"), 5, S(""));
    test_npos(S(), S("12345"), 6, S("not happening"));
    }
}
