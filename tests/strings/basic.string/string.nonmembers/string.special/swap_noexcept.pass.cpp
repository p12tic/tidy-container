//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <string>

// void swap(basic_string& c)
//     noexcept(!allocator_type::propagate_on_container_swap::value ||
//              __is_nothrow_swappable<allocator_type>::value);
//
//  In C++17, the standard says that swap shall have:
//     noexcept(allocator_traits<Allocator>::propagate_on_container_swap::value ||
//              allocator_traits<Allocator>::is_always_equal::value);

// This tests a conforming extension

#include <string>
#include "tidystring.h"
#include <cassert>

#include "test_allocator.h"

template <class T>
struct some_alloc
{
    typedef T value_type;
    
    some_alloc() {}
    some_alloc(const some_alloc&);
    void deallocate(void*, unsigned) {}

    typedef std::true_type propagate_on_container_swap;
};

template <class T>
struct some_alloc2
{
    typedef T value_type;
    
    some_alloc2() {}
    some_alloc2(const some_alloc2&);
    void deallocate(void*, unsigned) {}

    typedef std::false_type propagate_on_container_swap;
    typedef std::true_type is_always_equal;
};

int main()
{
#if __has_feature(cxx_noexcept)
    {
        typedef tidy::string C;
        C c1, c2;
        static_assert(noexcept(swap(c1, c2)), "");
    }
    {
        typedef tidy::basic_string<char, std::char_traits<char>, test_allocator<char>> C;
        C c1, c2;
        static_assert(noexcept(swap(c1, c2)), "");
    }
    {
        typedef tidy::basic_string<char, std::char_traits<char>, some_alloc<char>> C;
        C c1, c2;
#if TEST_STD_VER >= 14
    //  In c++14, if POCS is set, swapping the allocator is required not to throw
        static_assert( noexcept(swap(c1, c2)), "");
#else
        static_assert(!noexcept(swap(c1, c2)), "");
#endif
    }
#if TEST_STD_VER >= 14
    {
        typedef tidy::basic_string<char, std::char_traits<char>, some_alloc2<char>> C;
        C c1, c2;
    //  if the allocators are always equal, then the swap can be noexcept
        static_assert( noexcept(swap(c1, c2)), "");
    }
#endif

#endif
}
