#include <fstream>
#include <iostream>
#include <string>
#include <functional>

#include "assert.h"

#include "treemap.h"
#include "payload.h"

using namespace std;
using my::treemap;

/// @todo Write test, that proves non-owning pointer behavior of iterator
///        - Get iterator of a certain element
///        - Delete element from treemap
///        - Check Payload count (should have been decreased by deletion)
///        - Try to dereference iterator

// helper function for human readable output
string
to_string(Payload p)
{
    return string("("+to_string(p.x)+","+to_string(p.y)+","+to_string(p.z)+")");
}


int main()
{
  {
    treemap<string, int> m;
    m.insert("E", 1);
    m.insert("D", 2);
    m.insert("G", 3);
    m.insert("F", 4);

    for(treemap<string,int>::value_type element : m) {
      std::cout << element.first << " : " << element.second << std::endl;
    }

    export_treemap("test.dot", m);
  }

    {
        cout << "1. treemap basics..." << endl;

        treemap<string, Payload> m;
        assert(m.size() == 0);
        assert(m.count("A") == 0);
        assert(Payload::count() == 0);

        cout << "done..." << endl;
    }

    assert(Payload::count() == 0);
    cout << endl;

    {
        cout << "2. insert, find, count, size, clear ..." << endl;

        // insert()
        treemap<string, Payload> m;
        m.insert("Three", Payload(3,3,3));
        m.insert("Seven", Payload(7,7,7));
        auto [iter_eleven, was_inserted] = m.insert("Eleven", Payload(11,11,11));

        assert(was_inserted == true);
        assert(iter_eleven->first == "Eleven");
        assert(iter_eleven->second == Payload(11,11,11));
        assert(m.size() == 3);
        assert(m.count("Three") == 1);
        assert(m.count("Four") == 0);
        assert(Payload::count() == 3);

        {

            // find()
            auto eleven = m.find("Eleven");
            assert(Payload::count() == 3);

            auto key = eleven->first;
            auto value = eleven->second;
            cout << "found payload Eleven: [ " << key << "," << to_string(value) << " ]" << endl;

            // clear()
            m.clear();
            assert(m.size() == 0);
            assert(Payload::count() == 1); // value contains one Payload obj
        }
        assert(Payload::count() == 0);

        cout << "done." << endl;
    }

    assert(Payload::count() == 0);
    cout << endl;

    {
        cout << "3. operator [] read/write..." << endl;

        treemap<string, Payload> m;

        // [] write operation
        m["B=1"] = Payload(1,1,1);
        m["A=2"] = Payload(2,2,2);
        m["C=3"] = Payload(3,3,3);

        // count()
        assert(m.count("A=2") == 1);
        assert(m.count("B=1") == 1);
        assert(m.count("C=3") == 1);
        assert(m.size() == 3);

        // const version of operator[]
        auto const& const_m = m;
        cout << "A: " << to_string(const_m["A=2"]) << endl;
        assert(const_m["A=2"] == Payload(2,2,2));
        cout << "B: " << to_string(const_m["B=1"]) << endl;
        assert(const_m["B=1"] == Payload(1,1,1));
        cout << "C: " << to_string(const_m["C=3"]) << endl;
        assert(const_m["C=3"] == Payload(3,3,3));

        cout << "done." << endl;

    }

    assert(Payload::count() == 0);
    cout << endl;

    {
        cout << "4. iterator, ++ ..." << endl;

        treemap<string, int> m;

        m["Hartmut"] = 1;
        m["Helmut"] = 2;
        m["Zebra"] = 3;
        m["Anna"] = 4;
        m["Klaus"] = 5;
        assert(m.size() == 5);

        // iterator++, iterator operator->
        cout << "iterating through list in normal order:" << endl;
        for(auto i=m.begin(); i!=m.end(); ++i)
            cout << "  " << i->first << ": " << i->second << endl;

        assert(m.begin()->first == "Anna");
        assert((++m.begin())->first == "Hartmut");
        assert((++++m.begin())->first == "Helmut");
        assert((++++++m.begin())->first == "Klaus");
        assert((++++++++m.begin())->first == "Zebra");

        // iterator++, iterator operator*
        cout << "iterating through list using range-based for loop:" << endl;
        for(auto i : m)
            cout << "  " << i.first << ": " << i.second << endl;

        cout << "done." << endl;

    }

    assert(Payload::count() == 0);
    cout << endl;

    {
        cout << "5. iterator, -- ..." << endl;

        treemap<string, int> m;
        m["Hartmut"] = 1;
        m["Helmut"] = 2;
        m["Zebra"] = 3;
        m["Anna"] = 4;
        m["Klaus"] = 5;
        assert(m.size() == 5);

        // iterator--
        cout << "iterating through list in reverse order:" << endl;
        // todo
        // write this loop yourself; careful, one can easily screw up this test
        // auto i = m.end();

        // some bit of automatic testing in addition
        assert((--m.end())->first == "Zebra");
        assert((----m.end())->first == "Klaus");

        cout << "done." << endl;

    }

    assert(Payload::count() == 0);
    cout << endl;

    {
        cout << "6. move ..." << endl;
        assert(Payload::count() == 0); // just for sanity's sake

        treemap<string, Payload> m;
        m["Hartmut"] = Payload(1,1,1);
        m["Helmut"]  = Payload(1,2,1);
        m["Zebra"]   = Payload(1,3,1);
        m["Anna"]    = Payload(1,4,1);
        m["Klaus"]   = Payload(1,5,1);
        assert(m.size() == 5);
        assert(Payload::count() == 5);

        // move it
        auto m2 = std::move(m);

        // no duplicates?
        assert(Payload::count() == 5);

        // keys and values still intact?
        assert(m2["Anna"] == Payload(1,4,1));
        assert(m2["Klaus"] == Payload(1,5,1));

        cout << "done." << endl;
    }        
    assert(Payload::count() == 0);
    cout << endl;

    {
        cout << "7. deep copy ..." << endl;

        assert(Payload::count() == 0); // just for sanity's sake

        treemap<string, Payload> m;
        m["Hartmut"] = Payload(1,1,1);
        m["Helmut"]  = Payload(1,2,1);
        m["Zebra"]   = Payload(1,3,1);
        m["Anna"]    = Payload(1,4,1);
        m["Klaus"]   = Payload(1,5,1);
        assert(m.size() == 5);
        assert(Payload::count() == 5);

        // copy it
        auto m2 = m;

        // exactly twice as many objects now?
        assert(Payload::count() == 10);

        // keys and values still intact?
        assert(m2["Anna"] == m["Anna"]);
        assert(m2["Klaus"] == m["Klaus"]);

        // two independent copies?
        auto old_anna = m["Anna"];
        m["Anna"] = Payload(99,100,101);
        assert(m2["Anna"] == old_anna);

        cout << "done." << endl;
    }
    assert(Payload::count() == 0);
    cout << endl;

    return 0;
}
