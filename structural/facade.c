#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct _subSystem1_t
{
  void (*method1)(void);
} subSystem1_t;

typedef struct _subSystem2_t
{
  void (*method2)(void);
} subSystem2_t;

typedef struct _subSystem3_t
{
  void (*method3)(void);
} subSystem3_t;

typedef struct _subSystem4_t
{
  void (*method4)(void);
} subSystem4_t;

void subSystemMethod1(void)
{
  printf("subSystemMethod1\n");
}

void subSystemMethod2(void)
{
  printf("subSystemMethod2\n");
}

void subSystemMethod3(void)
{
  printf("subSystemMethod3\n");
}

void subSystemMethod4(void)
{
  printf("subSystemMethod4\n");
}

typedef struct _facade_t
{
  void (*Operation)(struct _facade_t*);
  subSystem1_t* sub1;
  subSystem2_t* sub2;
  subSystem3_t* sub3;
  subSystem4_t* sub4;
} facade_t;

void ClientCode(facade_t* facade)
{
  facade->Operation(facade);
}

void facadeInit(facade_t* facade, subSystem1_t* sub1, subSystem2_t* sub2, subSystem3_t* sub3, subSystem4_t* sub4)
{
  facade->sub1 = sub1;
  facade->sub2 = sub2;
  facade->sub3 = sub3;
  facade->sub4 = sub4;
}

void facadeOperation(facade_t* facade)
{
  facade->sub1->method1();
  facade->sub2->method2();
  facade->sub3->method3();
  facade->sub4->method4();
}

int main()
{
  facade_t facade = { .Operation = facadeOperation };

  subSystem1_t sub1 = {.method1 = subSystemMethod1};
  subSystem2_t sub2 = {.method2 = subSystemMethod2};
  subSystem3_t sub3 = {.method3 = subSystemMethod3};
  subSystem4_t sub4 = {.method4 = subSystemMethod4};
  
  facadeInit(&facade, &sub1, &sub2, &sub3, &sub4);

  ClientCode(&facade);
  printf("\n===== End of program =====\n");

  return 0;
}
