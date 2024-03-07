#include <stdio.h>

//! Polymorphism
int cal1(int a)
{
  return a + 1;
}
int cal2(int a)
{
  return a + 2;
}

int (*calculation)(int a);

void main()
{
  //! Use cal1 method
  calculation = cal1;
  printf("%d\n", calculation(1));

  //! Use cal2 method
  calculation = cal2;
  printf("%d\n", calculation(1));
}
