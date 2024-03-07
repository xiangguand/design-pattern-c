#include <stdio.h>

#define typecheck(T) _Generic((T), char : 1, int : 2, long long : 3, float : 4, default : 0)

char foo_char(char a, char b)
{
  return a + b;
}

int foo_int(int a, int b)
{
  return a + b * 100;
}

#define foo_b_char(b) _Generic((b), char : foo_char, int : foo_int)
#define foo_b_int(b)  _Generic((b), char : foo_char, int : foo_int)

#define foo(a, b) _Generic((a), char : foo_b_char(b), int : foo_b_int(b))((a), (b))

int main(void)
{
  //! Example1
  printf("passing a long value to the macro, result is %d\n", typecheck(235346345216356465));
  printf("passing a float value to the macro, result is %d\n", typecheck(4.32f));
  printf("passing a int value to the macro, result is %d\n", typecheck(324));
  printf("passing a string value to the macro, result is %d\n", typecheck("Hello"));

  //! Example2
  char ch_a = 10;
  char ch_b = 30;
  int int_a = 10;
  int int_b = 30;

  printf("\nExpect char is %d, int is %d\n", 40, 3010);
  printf("char input: %d\n", foo(ch_a, ch_b));
  printf("int input: %d\n", foo(int_a, int_b));

  return 0;
}
