#ifndef MATHFUNCTIONS_H
#define MATHFUNCTIONS_H
//------------------------------------------------------------------------

int Factorial(int n)
{
  int Res = 1;
  for (int i = 2; i <= n; ++i)
    Res *= i;
  //---
  return Res;
}
//------------------------------------------------------------------------

#endif // MATHFUNCTIONS_H
