#include <fstream>
#include <iostream>
#include <string>
#include <functional>
#include <map>

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

        assert(m.begin()->first == "Hartmut");
        assert((++m.begin())->first == "Anna");
        assert((++++m.begin())->first == "Helmut");
        assert((++++++m.begin())->first == "Zebra");
        assert((++++++++m.begin())->first == "Klaus");

        // iterator++, iterator operator*
        cout << "iterating through list using range-based for loop:" << endl;
        for(auto i : m)
            cout << "  " << i.first << ": " << i.second << endl;

        cout << "done." << endl;

    }

    {
        cout << "5. ordered iterator, ++ ..." << endl;

        treemap<string, int> m;

        m["Hartmut"] = 1;
        m["Helmut"] = 2;
        m["Zebra"] = 3;
        m["Anna"] = 4;
        m["Klaus"] = 5;
        assert(m.size() == 5);

        // iterator++, iterator operator->
        cout << "iterating through list in normal order:" << endl;
        for(auto i=m.obegin(); i!=m.oend(); ++i)
            cout << "  " << i->first << ": " << i->second << endl;

        assert(m.obegin()->first == "Anna");
        assert((++m.obegin())->first == "Hartmut");
        assert((++++m.obegin())->first == "Helmut");
        assert((++++++m.obegin())->first == "Klaus");
        assert((++++++++m.obegin())->first == "Zebra");

        // iterator++, iterator operator*
        cout << "iterating through list using range-based for loop:" << endl;
        for(auto i : m)
            cout << "  " << i.first << ": " << i.second << endl;

        cout << "done." << endl;

    }

    assert(Payload::count() == 0);
    cout << endl;

    {
        cout << "6. iterator, -- ..." << endl;

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
        for(auto it = --m.end(); it != m.begin(); --it) {
          cout << " " << (*it).first << ": " << (*it).second << endl;
        }
        cout << " " << m.begin()->first << ": " << m.begin()->second << endl;

        // some bit of automatic testing in addition
        assert((--m.end())->first == "Klaus");
        assert((----m.end())->first == "Zebra");

        cout << "done." << endl;

    }

    {
        cout << "7. ordered iterator, -- ..." << endl;

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
        for(auto it = --m.oend(); it != m.obegin(); --it) {
          cout << " " << (*it).first << ": " << (*it).second << endl;
        }
        cout << " " << m.obegin()->first << ": " << m.obegin()->second << endl;

        // some bit of automatic testing in addition
        assert((--m.oend())->first == "Zebra");
        assert((----m.oend())->first == "Klaus");

        cout << "done." << endl;

    }

    assert(Payload::count() == 0);
    cout << endl;

    {
        cout << "8. move ..." << endl;
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
        cout << "9. deep copy ..." << endl;

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

    {
      cout << "10. Optimize ... ";
      {
        treemap<string, Payload> m;
        m.insert("C", Payload());
        m.insert("B", Payload());
        m.insert("A", Payload());
        export_treemap("optimize-left-pre.dot", m);
        m.optimize();
        export_treemap("optimize-left-post.dot", m);

        treemap<string,Payload>::iterator root = m.begin();
        assert(root->first == "B");

        treemap<string,Payload>::iterator it = root;
        ++it;
        assert(it->first == "A");

        ++it;
        assert(it->first == "C");
        assert(Payload::count() == 3);
      }

      {
        treemap<string, Payload> m;
        m.insert("A", Payload());
        m.insert("B", Payload());
        m.insert("C", Payload());
        export_treemap("optimize-right-pre.dot", m);
        m.optimize();
        export_treemap("optimize-right-post.dot", m);

        treemap<string,Payload>::iterator root = m.begin();
        assert(root->first == "B");

        treemap<string,Payload>::iterator it = root;
        ++it;
        assert(it->first == "A");

        ++it;
        assert(it->first == "C");
        assert(Payload::count() == 3);
      }

      cout << "done" << std::endl;
    }

    {
      cout << "11. Task Example...";
      treemap<string,string> sheetExample;
      sheetExample.insert("J", "J");
      sheetExample.insert("F", "F");
      sheetExample.insert("D", "D");
      sheetExample.insert("C", "C");
      sheetExample.insert("G", "G");
      sheetExample.insert("P", "P");
      sheetExample.insert("L", "L");
      sheetExample.insert("N", "N");
      sheetExample.insert("V", "V");
      sheetExample.insert("S", "S");
      sheetExample.insert("Q", "Q");
      sheetExample.insert("U", "U");
      sheetExample.insert("X", "X");
      export_treemap("sheet-example.dot", sheetExample);

      {
        treemap<string, string> m = sheetExample;
        m.insert("B", "B");
        export_treemap("left-chain-pre.dot", m);
        m.optimize();
        export_treemap("left-chain-post.dot", m);

        m.insert("Y", "Y");
        m.insert("Z", "Z");
        export_treemap("right-chain-pre.dot", m);
        m.optimize();
        export_treemap("right-chain-post.dot", m);
      }
      cout << "done" << std::endl;
    }

    {
      cout << "12. Task Example optimized...";
      treemap<string,string> sheetExample;
      sheetExample.enableOptimization();
      sheetExample.insert("J", "J");
      sheetExample.insert("F", "F");
      sheetExample.insert("D", "D");
      sheetExample.insert("C", "C");
      sheetExample.insert("G", "G");
      sheetExample.insert("P", "P");
      sheetExample.insert("L", "L");
      sheetExample.insert("N", "N");
      sheetExample.insert("V", "V");
      sheetExample.insert("S", "S");
      sheetExample.insert("Q", "Q");
      sheetExample.insert("U", "U");
      sheetExample.insert("X", "X");
      export_treemap("sheet-example-optimized.dot", sheetExample);
    }

    assert(Payload::count() == 0);
    cout << endl;

    return 0;
}
