#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define SHARE_ID1  (0)
#define SHARE_ID2  (1)
#define UNSHARE_ID (2)

#define FLYWEIGHT_MEMBERS(TYPE)                                                                                        \
  void (*Operation)(TYPE);                                                                                             \
  char color[10]

typedef struct _flyweight_t
{
  FLYWEIGHT_MEMBERS(struct _flyweight_t*);
} flyweight_t;

typedef struct _unsharedConcreteFlyweight_t
{
  FLYWEIGHT_MEMBERS(struct _unsharedConcreteFlyweight_t*);
  int value99;
} unsharedConcreteFlyweight_t;

typedef struct _concreteFlyweight_t
{
  FLYWEIGHT_MEMBERS(struct _concreteFlyweight_t*);
  int value123;
} concreteFlyweight_t;

typedef struct _flyweightFactory_t
{
  flyweight_t* (*getFlyweight)(struct _flyweightFactory_t*, int id);
  flyweight_t* flyweights[10];
} flyweightFactory_t;

void concreteFlywieghtOperation(concreteFlyweight_t* concreteFlyweight)
{
  printf("concreteFlywieghtOperation\n");
  printf("val123: %d\n", concreteFlyweight->value123);
  printf("color: %s\n", concreteFlyweight->color);
}

void unsharedConcreteFlywieghtOperation(unsharedConcreteFlyweight_t* unsharedConcreteFlyweight)
{
  printf("unsharedConcreteFlywieghtOperation\n");
  printf("val99: %d\n", unsharedConcreteFlyweight->value99);
  printf("color: %s\n", unsharedConcreteFlyweight->color);
}

concreteFlyweight_t* createConcreteFlyweight()
{
  concreteFlyweight_t* fly = malloc(sizeof(concreteFlyweight_t));
  memset(fly, 0, sizeof(concreteFlyweight_t));
  static const char shared_color[] = "Black";
  memcpy(fly->color, shared_color, sizeof(shared_color));
  fly->value123 = 123;
  fly->Operation = concreteFlywieghtOperation;

  return fly;
}

concreteFlyweight_t* createConcreteFlyweight2()
{
  concreteFlyweight_t* fly = malloc(sizeof(concreteFlyweight_t));
  memset(fly, 0, sizeof(concreteFlyweight_t));
  static const char shared_color[] = "White";
  memcpy(fly->color, shared_color, sizeof(shared_color));
  fly->value123 = 123;
  fly->Operation = concreteFlywieghtOperation;

  return fly;
}

unsharedConcreteFlyweight_t* createUnsharedConcreteFlyweight()
{
  unsharedConcreteFlyweight_t* fly = malloc(sizeof(unsharedConcreteFlyweight_t));
  memset(fly, 0, sizeof(unsharedConcreteFlyweight_t));
  static const char unshared_color[] = "Yellow";
  memcpy(fly->color, unshared_color, sizeof(unshared_color));
  fly->value99 = 99;
  fly->Operation = unsharedConcreteFlywieghtOperation;

  return fly;
}

typedef concreteFlyweight_t* (*createFlyeightFunc_t)(void);

static const createFlyeightFunc_t createFlyweightArray[2] = {
  createConcreteFlyweight,
  createConcreteFlyweight2,
};

flyweight_t* getFlyweight(flyweightFactory_t* flyweightFactory, int id)
{
  flyweight_t* flyweight = NULL;

  if ((id >= 0) && (id <= 1))
  {
    if (NULL == flyweightFactory->flyweights[id])
    {
      flyweightFactory->flyweights[id] = (flyweight_t*)createFlyweightArray[id]();
    }
    flyweight = flyweightFactory->flyweights[id];
  }
  else if (id == 2)  // UNSHARED ID
  {
    flyweight = (flyweight_t *)createUnsharedConcreteFlyweight();
  }
  else
  {
    printf("ID is error \n");
  }

  return flyweight;
}

void freeFlyWeight(flyweightFactory_t* flyweightFactory)
{
  for (int i = 0; i < 10; i++)
  {
    if (flyweightFactory->flyweights[i])
    {
      free(flyweightFactory->flyweights[i]);
    }
  }
}

int main()
{
  flyweightFactory_t flyweightFactory = { .flyweights = { 0 }, .getFlyweight = getFlyweight };
  flyweight_t* flyweight = NULL;

  printf("===== flyweight with shared 1 =====\n");
  flyweight = flyweightFactory.getFlyweight(&flyweightFactory, SHARE_ID1);
  assert(NULL != flyweight);
  flyweight->Operation(flyweight);

  printf("===== flyweight with shared 2 =====\n");
  flyweight = flyweightFactory.getFlyweight(&flyweightFactory, SHARE_ID2);
  assert(NULL != flyweight);
  flyweight->Operation(flyweight);

  printf("===== Shared2 change color =====\n");
  memset(flyweight->color, 0, sizeof(flyweight->color));
  memcpy(flyweight->color, "Purple", 6);

  printf("===== Run getFlyweight and operation again =====\n");
  printf("===== flyweight with shared 2 =====\n");
  flyweight = flyweightFactory.getFlyweight(&flyweightFactory, SHARE_ID2);
  assert(NULL != flyweight);
  flyweight->Operation(flyweight);

  printf("===== flyweight with unshared =====\n");
  flyweight = flyweightFactory.getFlyweight(&flyweightFactory, UNSHARE_ID);
  assert(NULL != flyweight);
  flyweight->Operation(flyweight);
  
  printf("===== Unshared change color =====\n");
  memset(flyweight->color, 0, sizeof(flyweight->color));
  memcpy(flyweight->color, "Blue", 4);

  printf("===== Run getFlyweight and operation again =====\n");
  printf("===== flyweight with unshared =====\n");
  flyweight = flyweightFactory.getFlyweight(&flyweightFactory, UNSHARE_ID);
  assert(NULL != flyweight);
  flyweight->Operation(flyweight);

  printf("\nNow you will find that unshared color doesn't change, but shared one channges\n");

  printf("\n===== End of program =====\n");

  freeFlyWeight(&flyweightFactory);
  return 0;
}
