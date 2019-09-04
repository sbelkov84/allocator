#include "customallocator.h"
#include "customcontainer.h"
#include "mathfunctions.h"
//------------------------------------------------------------------------
#include <iostream>
#include <map>
//------------------------------------------------------------------------

int main(int, char**)
{ 
  // std::map with TCustomAllocator
  std::map<int, int, std::less<int>, TCustomAllocator<std::pair<int, int>>> SomeMap;
  for (int i = 0; i < 10; ++i)
  {
    SomeMap[i] = Factorial(i);
  }

  for (auto it = SomeMap.cbegin(); it != SomeMap.end(); ++it)
  {
    std::cout << it->second << ' ';
  }
  std::cout << std::endl;

  // TCustomContainer with standart allocator
  TCustomContainer<int, std::allocator<int>> CustCont;
  for (int i = 0; i < 10; ++i)
  {
    CustCont.push_back(i + 1);
  }

  /*for (auto it = CustCont.begin(); it != CustCont.end(); ++it)
  {
    std::cout << *it << ' ';
  }
  std::cout << std::endl;*/

  // TCustomContainer with TCustomAllocator
  TCustomContainer<int> CustContMyAlloc;
  for (int i = 0; i < 10; ++i)
  {
    CustContMyAlloc.push_back(i + 1);
  }

  for (auto it = CustContMyAlloc.begin(); it != CustContMyAlloc.end(); ++it)
  {
    std::cout << *it << ' ';
  }
  std::cout << std::endl;

  return 0;
}
