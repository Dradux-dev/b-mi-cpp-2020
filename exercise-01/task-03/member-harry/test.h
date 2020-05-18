#pragma once

#include <iostream>
#include <cassert>
#include <typeinfo>
using namespace std;

#include "vec.h"
using namespace my;


template <typename T, int N>
void test_Vec() {

  using DUT = Vec<T,N>;
#ifndef NDEBUG
    cout << "======================" << endl;
    cout << "     Testing Vec<" << typeid (T).name() << "," << N << ">"  << endl;
    cout << "======================" << endl;

    {
        // do not tolerate any memory overhead
        cout << "  sizeof(DUT) == " << N*sizeof(T) << " bytes: ";
        assert( sizeof(DUT) == N*sizeof(T) );
        cout << "passed." << endl;
    }

    {
        cout << "  constructor & index operator: ";
        DUT a;
        assert( std::find_if_not(a.begin(),a.end(), [](const T& x) -> bool {
            return x == T();
          }) == a.end());

        DUT b({1, 2, 3});
        assert( b[0] == 1 && (N < 2 || b[1] == 2 ) && (N < 3 || b[2] == 3) );
        cout << "passed." << endl;
    }

    {
        cout << "  read-only access to const object: ";
        const DUT a({1,2,3});
        // the next line will throw a compiler error if there is no proper "operator[] const"
        assert( a[1] == 2 );
        cout << "passed." << endl;
    }

    {
        cout << "  write access to a non-const object: ";
        DUT a({1,2,3});
        a[1] = 4;
        assert( a[0] == 1 && (N < 2 || a[1] == 4 ) && (N < 3 || a[2] == 3) );
        cout << "passed." << endl;
    }

    {
        cout << "  comparison: ";
        DUT a({1,2,3}), b({1,2,3}), c({4,3,9}), d({4,5,6});
        assert( a == b );
        assert( a == a );
        assert( a != c );
        assert( b != d );
        assert( !(a != b) );
        cout << "passed." << endl;
    }

    {
        // should work out of the box when using std container (!)
        cout << "  assignment: ";
        DUT a({1,2,3});
        cout << a[0] << " "; // to make sure these values are not optimized away!
        a = DUT({4,5,6});
        assert( a[0] == 4 && (N < 2 || a[1] == 5 ) && (N < 3 || a[2] == 6) );
        cout << "passed." << endl;
    }

    {
        cout << "  addition: ";
        DUT a({1,2,3}), b({4,5,6});
        a += b;
        assert( a == DUT({5,7,9}) );
        auto c = a + DUT({1,1,1});
        assert( c == DUT({6,8,10}) );
        DUT one({1,1,1}), four({4,4,4});
        one += one+one+one;
        assert( one == four);
        DUT x({0,0,0}), y({1,1,1}), z({2,2,2});
        x += y += z;
        assert( y == DUT({3,3,3}) );
        assert( x == y );
        cout << "passed." << endl;
    }

    {
        cout << "  unary minus: ";
        DUT a({1,2,3});
        assert( -a == DUT({-1,-2,-3}) );
        cout << "passed." << endl;
    }

    {
        cout << "  dot product: ";
        DUT a({1,2,3});
        assert( dot(a,a) == 1*1 + (N >= 2 ? 2*2 : 0) +(N >= 3 ? 3*3 : 0) );
        cout << "passed." << endl;
    }

    {
        // these tests will not compile if you forgot to declare
        // some methods const
        cout << "  constness: ";
        const DUT a({1,2,3});
        assert( a[1] == 2 );
        assert( a == a );
        assert( ! (a != a) );
        assert( a+a == DUT({2,4,6}) );
        assert( -a == DUT({-1,-2,-3}) );
       assert( dot(a,a) == 1*1 + (N >= 2 ? 2*2 : 0) +(N >= 3 ? 3*3 : 0) );
        cout << "passed." << endl;
    }

    cout << "all Vec<" << typeid (T).name() << "," << N << "> tests passed." << endl << endl;

#endif // not NDEBUG

}
