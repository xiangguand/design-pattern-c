#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _encapsulation_t
{
  void* info;
  int (*get)(struct _encapsulation_t*);
  int (*set)(struct _encapsulation_t*, int);
} encapsulation_t;

//! constructor
encapsulation_t* createEncapsulation(void);

int main()
{
  encapsulation_t* encap = createEncapsulation();
  printf("Should be ==> 1, 2, 3, 14, 15, 16\n");

  printf("%d\n", encap->get(encap));
  printf("%d\n", encap->get(encap));
  printf("%d\n", encap->get(encap));

  encap->set(encap, 10);
  printf("%d\n", encap->get(encap));
  printf("%d\n", encap->get(encap));
  printf("%d\n", encap->get(encap));
}
