//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>
// class vector
// vector();

#include <vector>
#include "tidyvector.h"

struct X
{
    tidy::vector<X> q;
};

int main()
{
}
