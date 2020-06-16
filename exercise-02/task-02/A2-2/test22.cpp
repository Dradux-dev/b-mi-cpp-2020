#include <vector>


#include <iostream>
#include <cassert>
#include "my_vector.h"
#include "payload.h"

struct destruct_test {
  inline destruct_test() {
    std::cout << "Destruct_test()" << std::endl;
  }
  inline destruct_test(const destruct_test&) {
    std::cout << "Copy_test()" << std::endl;
  }
  inline virtual ~destruct_test() {
    std::cout << "~Destruct_test()" << std::endl;
  }
};

void test_22()
{
  using my::vector;
  using std::cout;
  using std::endl;


  {
    cout << "Constructor init...";
    vector<Payload> v0(3,Payload(-1,-1,-1));
    assert(v0.size() == 3);
    assert(v0.capacity() == 3);
    assert(Payload::count() == 3);

    cout <<"done" << endl;
  }

  {
    cout << "Constructor init... ";
    vector<Payload> v0(3);
    assert(v0.size() == 0);
    assert(v0.capacity() == 3);
    assert(Payload::count() == 0);

    cout <<"done" << endl;
  }

  {
    cout << "Constructor init... ";
    vector<Payload> v0;
    assert(v0.size() == 0);
    assert(v0.capacity() == 0);
    assert(Payload::count() == 0);

    cout <<"done" << endl;
  }

  {
    cout << "Push back... ";
    vector<Payload> v0(2);
    assert(v0.size() == 0);
    assert(v0.capacity() == 2);
    assert(Payload::count() == 0);

    v0.push_back(Payload(4,4,4));
    v0.push_back(Payload(2,2,2));
    assert(v0.size() == 2);
    assert(v0.capacity() == 2);
    assert(Payload::count() == 2);

    v0.push_back(Payload(2,2,2));
    assert(v0.size() == 3);
    assert(v0.capacity() == 4);
    assert(Payload::count() == 3);


    cout <<"done" << endl;
  }

  {
    cout << "pop back... ";
    vector<Payload> v0(2);
    assert(v0.size() == 0);
    assert(v0.capacity() == 2);
    assert(Payload::count() == 0);

    v0.push_back(Payload(4,4,4));
    v0.push_back(Payload(2,2,2));
    assert(v0.size() == 2);
    assert(v0.capacity() == 2);
    assert(Payload::count() == 2);

    assert(v0.pop_back() == Payload(2,2,2));
    assert(v0.size() == 1);
    assert(v0.capacity() == 2);
    assert(Payload::count() == 1);

    assert(v0.pop_back() == Payload(4,4,4));
    assert(v0.size() == 0);
    assert(v0.capacity() == 2);
    assert(Payload::count() == 0);


    cout <<"done" << endl;
  }

  {
    cout << "emplace back... ";
    vector<Payload> v0(2);
    assert(v0.size() == 0);
    assert(v0.capacity() == 2);
    assert(Payload::count() == 0);

    v0.emplace_back(4,4,4);
    v0.emplace_back(2,2,2);
    assert(v0.size() == 2);
    assert(v0.capacity() == 2);
    assert(Payload::count() == 2);

    v0.emplace_back(2,2,2);
    assert(v0.size() == 3);
    assert(v0.capacity() == 4);
    assert(Payload::count() == 3);


    cout <<"done" << endl;
  }

  {
    cout << "reserve ... ";
    vector<Payload> v1;
    v1.reserve(100);
    assert(v1.size() == 0);
    assert(v1.capacity() == 100);
    assert(Payload::count() == 0);

    v1.reserve(20);
    assert(v1.size() == 0);
    assert(v1.capacity() == 20);
    assert(Payload::count() == 0);


    v1.reserve(200);
    assert(v1.size() == 0);
    assert(v1.capacity() == 200);
    assert(Payload::count() == 0);

    v1.reserve(0);
    assert(v1.size() == 0);
    assert(v1.capacity() == 0);
    assert(Payload::count() == 0);

    v1.reserve(10);
    v1.emplace_back(4,4,4);
    v1.emplace_back(4,4,4);
    assert(v1.size() == 2);
    assert(v1.capacity() == 10);
    assert(Payload::count() == 2);

    v1.reserve(1);
    assert(v1.size() == 1);
    assert(v1.capacity() == 1);
    assert(Payload::count() == 1);

    cout << "done" << endl;

  }

  {
    cout << "shink to fit...";
    vector<Payload> v0(10);
    v0.emplace_back(1,1,1);
    v0.emplace_back();
    v0.shrink_to_fit();
    assert(v0.size() == 2);
    assert(v0.capacity() == 2);
    assert(Payload::count() == 2);

    cout <<"done" << endl;
  }

  {
    cout <<" [] operator...";
    vector<Payload> v0(10);
    v0.emplace_back(1,1,1);
    v0.emplace_back();
    assert(v0[1] == Payload());
    assert(v0.size() == 2);
    assert(v0.capacity() == 10);
    assert(Payload::count() == 2);

    cout <<"done" << endl;

  }

  {
    cout <<" [] exception...";
    vector<Payload> v0(10);
    v0.emplace_back(1,1,1);
    v0.emplace_back();
    assert(v0.at(1) == Payload());
    assert(v0.size() == 2);
    assert(v0.capacity() == 10);
    assert(Payload::count() == 2);


    try {
        v0.at(2) = Payload(0,0,0);
        assert(!"should have thrown exception");
    } catch(std::out_of_range ex) {
        cout << "exception: " << ex.what();
    }

    cout <<"done" << endl;

  }

  {
    cout << "clear...";
    vector<Payload> v0(10);
    v0.emplace_back(1,1,1);
    v0.emplace_back();
    v0.clear();
    assert(v0.size() == 0);
    assert(v0.capacity() == 10);
    assert(Payload::count() == 0);

    cout <<"done" << endl << endl;
  }

  {
    std::cout << "Destruct_test...";
    vector<destruct_test> v1(2);
    v1.push_back(destruct_test());
    v1.pop_back();
    std::cout << "done" << std::endl;
  }



}
