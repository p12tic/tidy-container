//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <string>

// basic_string(basic_string&&)
//        noexcept(is_nothrow_move_constructible<allocator_type>::value);

// This tests a conforming extension

#include <string>
#include "tidystring.h"
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"

template <class T>
struct some_alloc
{
    typedef T value_type;
    some_alloc(const some_alloc&);
};

int main()
{
#if __has_feature(cxx_noexcept)
    {
        typedef tidy::string C;
        static_assert(std::is_nothrow_move_constructible<C>::value, "");
    }
    {
        typedef tidy::basic_string<char, std::char_traits<char>, test_allocator<char>> C;
        static_assert(std::is_nothrow_move_constructible<C>::value, "");
    }
    {
        typedef tidy::basic_string<char, std::char_traits<char>, some_alloc<char>> C;
#if TEST_STD_VER <= 14
        static_assert(!std::is_nothrow_move_constructible<C>::value, "");
#else
        static_assert( std::is_nothrow_move_constructible<C>::value, "");
#endif
    }
#endif
}
