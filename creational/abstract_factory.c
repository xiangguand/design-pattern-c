#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define PRODUCT_MEMBER(TYPE) void (*operation)(TYPE)
#define FACTORY_MEMBER(PRODUCTA_TYPE, PRODUCTB_TYPE, TYPE)                                                             \
  PRODUCTA_TYPE (*createProductA)(TYPE);                                                                               \
  PRODUCTB_TYPE (*createProductB)(TYPE)

typedef struct _abstractProductA_t
{
  PRODUCT_MEMBER(struct _abstractProductA_t*);
} abstractProductA_t;

typedef struct _abstractProductB_t
{
  PRODUCT_MEMBER(struct _abstractProductB_t*);
} abstractProductB_t;

typedef struct _concreteProductA_t
{
  PRODUCT_MEMBER(struct _concreteProductA_t*);
} concreteProductA_t;

typedef struct _concreteProductB_t
{
  PRODUCT_MEMBER(struct _concreteProductB_t*);
} concreteProductB_t;

typedef struct _abstractFactory_t
{
  FACTORY_MEMBER(abstractProductA_t*, abstractProductB_t*, struct _abstractFactory_t*);
} abstractFactory_t;

typedef struct _concreteFactory1_t
{
  FACTORY_MEMBER(abstractProductA_t*, abstractProductB_t*, struct _concreteFactory1_t*);
} concreteFactory1_t;

typedef struct _concreteFactory2_t
{
  FACTORY_MEMBER(abstractProductA_t*, abstractProductB_t*, struct _concreteFactory2_t*);
} concreteFactory2_t;

void operationA1(concreteProductA_t* product)
{
  printf("Run operationA1\n");
}

void operationB1(concreteProductB_t* product)
{
  printf("Run operationB1\n");
}

void operationA2(concreteProductA_t* product)
{
  printf("Run operationA2\n");
}

void operationB2(concreteProductB_t* product)
{
  printf("Run operationB2\n");
}

abstractProductA_t* createAbstractProductA(abstractFactory_t* factory)
{
  concreteProductA_t* product = malloc(sizeof(concreteProductA_t));
  product->operation = operationA1;

  return (abstractProductA_t*)product;
}

abstractProductB_t* createAbstractProductB(abstractFactory_t* factory)
{
  concreteProductB_t* product = malloc(sizeof(concreteProductB_t));
  product->operation = operationB1;

  return (abstractProductB_t*)product;
}

abstractProductA_t* createAbstractProductA1(abstractFactory_t* factory)
{
  concreteProductA_t* product = malloc(sizeof(concreteProductA_t));
  product->operation = operationA1;

  return (abstractProductA_t*)product;
}

abstractProductB_t* createAbstractProductB1(abstractFactory_t* factory)
{
  concreteProductB_t* product = malloc(sizeof(concreteProductB_t));
  product->operation = operationB1;

  return (abstractProductB_t*)product;
}

abstractProductA_t* createAbstractProductA2(abstractFactory_t* factory)
{
  concreteProductA_t* product = malloc(sizeof(concreteProductA_t));
  product->operation = operationA2;

  return (abstractProductA_t*)product;
}

abstractProductB_t* createAbstractProductB2(abstractFactory_t* factory)
{
  concreteProductB_t* product = malloc(sizeof(concreteProductB_t));
  product->operation = operationB2;

  return (abstractProductB_t*)product;
}

concreteFactory1_t* createFactory1()
{
  concreteFactory1_t* factory = malloc(sizeof(concreteFactory1_t));
  factory->createProductA = createAbstractProductA1;
  factory->createProductB = createAbstractProductB1;

  return factory;
}

concreteFactory2_t* createFactory2()
{
  concreteFactory2_t* factory = malloc(sizeof(concreteFactory2_t));
  factory->createProductA = createAbstractProductA2;
  factory->createProductB = createAbstractProductB2;

  return factory;
}

void ClientCode(abstractFactory_t* factory)
{
  abstractProductA_t* productA = factory->createProductA(factory);
  abstractProductB_t* productB = factory->createProductB(factory);

  productA->operation(productA);
  productB->operation(productB);
}

int main()
{
  concreteFactory1_t* factory1 = createFactory1();
  concreteFactory2_t* factory2 = createFactory2();

  ClientCode((abstractFactory_t*)factory1);
  ClientCode((abstractFactory_t*)factory2);

  return 0;
}
