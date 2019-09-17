#include "customallocator.h"
#include "customcontainer.h"
#include "mathfunctions.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <map>

TEST(Allocator, OnMap)
{
  std::map<int, int, std::less<int>, TCustomAllocator<std::pair<int, int>>> SomeMap;
  for (int i = 0; i < 10; ++i)
  {
    SomeMap[i] = Factorial(i);
  }

  int i = 0;
  for (auto it = SomeMap.cbegin(); it != SomeMap.end(); ++it)
  {
    EXPECT_EQ(it->second, Factorial(i));    
    ++i;
  }
}

TEST(Allocator, OnCustomContainer)
{
  TCustomContainer<int, TCustomAllocator<std::pair<int, int>>> CustContMyAlloc;
  for (int i = 0; i < 10; ++i)
  {
    CustContMyAlloc.push_back(i + 1);
  }

  int i = 1;
  for (auto it = CustContMyAlloc.begin(); it != CustContMyAlloc.end(); ++it)
  {
    EXPECT_EQ(*it, i);    
    ++i;
  }
}

TEST(Allocator, OnMapMoreThan10)
{
  std::map<int, int, std::less<int>, TCustomAllocator<std::pair<int, int>>> SomeMap;
  for (int i = 0; i < 11; ++i)
  {
    SomeMap[i] = Factorial(i);
  }

  int i = 0;
  for (auto it = SomeMap.cbegin(); it != SomeMap.end(); ++it)
  {
    EXPECT_EQ(it->second, Factorial(i));    
    ++i;
  }
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  int Res = RUN_ALL_TESTS();
  return Res;
}