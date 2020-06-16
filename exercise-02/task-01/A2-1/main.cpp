#include <iostream>
#include <my_vector.h>
using namespace std;

int main()
{


  vector<int> test(8,77);

  if(test.empty()) {
      cout << "Empty" << endl;
    }
  cout<<"Vector size() = " << test.size() << endl;


  std::cout<< test[5] << endl;



  return 0;
}
