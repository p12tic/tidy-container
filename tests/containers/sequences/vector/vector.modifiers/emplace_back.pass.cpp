//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>

// template <class... Args> void emplace_back(Args&&... args);

#include <vector>
#include "tidyvector.h"
#include <cassert>
#include "../../../stack_allocator.h"
#include "min_allocator.h"
#include "asan_testing.h"

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

class A
{
    int i_;
    double d_;

    A(const A&);
    A& operator=(const A&);
public:
    A(int i, double d)
        : i_(i), d_(d) {}

    A(A&& a)
        : i_(a.i_),
          d_(a.d_)
    {
        a.i_ = 0;
        a.d_ = 0;
    }

    A& operator=(A&& a)
    {
        i_ = a.i_;
        d_ = a.d_;
        a.i_ = 0;
        a.d_ = 0;
        return *this;
    }

    int geti() const {return i_;}
    double getd() const {return d_;}
};

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

int main()
{
#if TIDY_EXPECTED_FAIL
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    {
        tidy::vector<A> c;
        c.emplace_back(2, 3.5);
        assert(c.size() == 1);
        assert(c.front().geti() == 2);
        assert(c.front().getd() == 3.5);
        assert(is_contiguous_container_asan_correct(c)); 
        c.emplace_back(3, 4.5);
        assert(c.size() == 2);
        assert(c.front().geti() == 2);
        assert(c.front().getd() == 3.5);
        assert(c.back().geti() == 3);
        assert(c.back().getd() == 4.5);
        assert(is_contiguous_container_asan_correct(c)); 
    }
    {
        tidy::vector<A, stack_allocator<A, 4> > c;
        c.emplace_back(2, 3.5);
        assert(c.size() == 1);
        assert(c.front().geti() == 2);
        assert(c.front().getd() == 3.5);
        assert(is_contiguous_container_asan_correct(c)); 
        c.emplace_back(3, 4.5);
        assert(c.size() == 2);
        assert(c.front().geti() == 2);
        assert(c.front().getd() == 3.5);
        assert(c.back().geti() == 3);
        assert(c.back().getd() == 4.5);
        assert(is_contiguous_container_asan_correct(c)); 
    }
#if __cplusplus >= 201103L
    {
        tidy::vector<A, min_allocator<A>> c;
        c.emplace_back(2, 3.5);
        assert(c.size() == 1);
        assert(c.front().geti() == 2);
        assert(c.front().getd() == 3.5);
        assert(is_contiguous_container_asan_correct(c)); 
        c.emplace_back(3, 4.5);
        assert(c.size() == 2);
        assert(c.front().geti() == 2);
        assert(c.front().getd() == 3.5);
        assert(c.back().geti() == 3);
        assert(c.back().getd() == 4.5);
        assert(is_contiguous_container_asan_correct(c)); 
    }
#endif
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
#endif // TIDY_EXPECTED_FAIL
}
