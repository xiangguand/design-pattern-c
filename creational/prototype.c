#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define PROTO_1 (1)
#define PROTO_2 (2)

typedef struct _prototype_t prototype_t;
#define PROTOTYPE_MEMBERS(TYPE)                                                                                        \
  prototype_t* (*clone)(TYPE);                                                                                         \
  void (*printValue)(TYPE);                                                                                            \
  int val

typedef struct _prototype_t
{
  PROTOTYPE_MEMBERS(struct _prototype_t*);
} prototype_t;

typedef struct _concretePrototype1_t
{
  PROTOTYPE_MEMBERS(struct _concretePrototype1_t*);
} concretePrototype1_t;

typedef struct _concretePrototype2_t
{
  PROTOTYPE_MEMBERS(struct _concretePrototype2_t*);
} concretePrototype2_t;

typedef struct _factoryPrototype_t
{
  prototype_t* (*createPrototype)(struct _factoryPrototype_t*, int);
  concretePrototype1_t* prototype1;
  concretePrototype2_t* prototype2;
} factoryPrototype_t;

prototype_t* concretePrototypeClone1(concretePrototype1_t* prototype)
{
  concretePrototype1_t* clone_prototype = malloc(sizeof(concretePrototype1_t));
  memcpy(clone_prototype, prototype, sizeof(concretePrototype1_t));

  return (prototype_t*)clone_prototype;
}

prototype_t* concretePrototypeClone2(concretePrototype2_t* prototype)
{
  concretePrototype2_t* clone_prototype = malloc(sizeof(concretePrototype2_t));
  memcpy(clone_prototype, prototype, sizeof(concretePrototype2_t));

  return (prototype_t*)clone_prototype;
}

void concreteProtoTypePrintValue1(concretePrototype1_t* prototype)
{
  printf("Prototype1: %d\n", prototype->val);
}

void concreteProtoTypePrintValue2(concretePrototype2_t* prototype)
{
  printf("Prototype2: %d\n", prototype->val);
}

prototype_t* factoryPrototypCreatePrototype(factoryPrototype_t* factory, int id)
{
  prototype_t* prototype = NULL;
  if (id == PROTO_1)
  {
    prototype = (prototype_t*)factory->prototype1;
  }
  else if (id == PROTO_2)
  {
    prototype = (prototype_t*)factory->prototype2;
  }

  return prototype;
}

factoryPrototype_t* createFactoryPrototype()
{
  factoryPrototype_t* factory = malloc(sizeof(factoryPrototype_t));
  memset(factory, 0, sizeof(factoryPrototype_t));
  factory->prototype1 = malloc(sizeof(concretePrototype1_t));
  factory->prototype2 = malloc(sizeof(concretePrototype2_t));
  memset(factory->prototype1, 0, sizeof(factoryPrototype_t));
  memset(factory->prototype2, 0, sizeof(factoryPrototype_t));
  factory->prototype1->clone = concretePrototypeClone1;
  factory->prototype2->clone = concretePrototypeClone2;
  factory->prototype1->val = 20;
  factory->prototype2->val = 50;
  factory->prototype1->printValue = concreteProtoTypePrintValue1;
  factory->prototype2->printValue = concreteProtoTypePrintValue2;
  factory->createPrototype = factoryPrototypCreatePrototype;

  return factory;
}

void ClientCode(factoryPrototype_t* factory)
{
  prototype_t* prototpye = factory->createPrototype(factory, PROTO_1);
  prototpye->printValue(prototpye);
  free(prototpye);

  prototpye = factory->createPrototype(factory, PROTO_2);
  prototpye->printValue(prototpye);
  free(prototpye);
}

int main()
{
  factoryPrototype_t* factory = createFactoryPrototype();
  ClientCode(factory);

  return 0;
}
