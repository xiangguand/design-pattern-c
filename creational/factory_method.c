#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define PRODUCT_MEMBER(TYPE)               char* (*operation)(TYPE)
#define CREATOR_MEMBER(PRODUCT_TYPE, TYPE) PRODUCT_TYPE (*factoryMethod)(TYPE)

typedef struct _product_t
{
  PRODUCT_MEMBER(struct _product_t*);
} product_t;

typedef struct _concreteProduct_t
{
  PRODUCT_MEMBER(struct _concreteProduct_t*);
  char* name;
} concreteProduct_t;

typedef struct _creator_t
{
  CREATOR_MEMBER(struct _product_t*, struct _creator_t*);
} creator_t;

typedef struct _concreteCreator_t
{
  CREATOR_MEMBER(struct _concreteProduct_t*, struct _concreteCreator_t*);
  void (*createProduct)(struct _concreteCreator_t*);
  char* (*getProductName)(struct _concreteCreator_t*);
  concreteProduct_t* product;
} concreteCreator_t;

char* concreteProductOperation(concreteProduct_t* product)
{
  return product->name;
}

void concreteCreatrorCreateProduct(concreteCreator_t* creator)
{
  creator->product = creator->factoryMethod(creator);
}

char* concreteCreatorGetProductName(concreteCreator_t* creator)
{
  return creator->product->operation(creator->product);
}

concreteProduct_t* concreteFactoryMethod1(concreteCreator_t* creator)
{
  concreteProduct_t* product = malloc(sizeof(concreteProduct_t));
  product->name = malloc(20);
  product->operation = concreteProductOperation;
  memset(product->name, 0, sizeof(product->name));
  memcpy(product->name, "Method1", sizeof("Method1"));

  return product;
}

concreteProduct_t* concreteFactoryMethod2(concreteCreator_t* creator)
{
  concreteProduct_t* product = malloc(sizeof(concreteProduct_t));
  product->name = malloc(20);
  product->operation = concreteProductOperation;
  memset(product->name, 0, sizeof(product->name));
  memcpy(product->name, "Method2", sizeof("Method2"));

  return product;
}

concreteCreator_t* createConcreteCreator1(void)
{
  concreteCreator_t* creator = malloc(sizeof(concreteCreator_t));
  creator->factoryMethod = concreteFactoryMethod1;
  creator->getProductName = concreteCreatorGetProductName;
  creator->createProduct = concreteCreatrorCreateProduct;

  return creator;
}

concreteCreator_t* createConcreteCreator2(void)
{
  concreteCreator_t* creator = malloc(sizeof(concreteCreator_t));
  creator->factoryMethod = concreteFactoryMethod2;
  creator->getProductName = concreteCreatorGetProductName;
  creator->createProduct = concreteCreatrorCreateProduct;

  return creator;
}

void ClienCode(concreteCreator_t* creator)
{
  creator->createProduct(creator);

  printf("Create product: %s\n", creator->getProductName(creator));
}

concreteCreator_t* createConcreteCreator(char *para)
{
  if(0 == strcmp(para, "factory1"))
  {
    return createConcreteCreator1();
  }
  else if(0 == strcmp(para, "factory2"))
  {
    return createConcreteCreator2();
  }

  return NULL;
}

int main()
{
  concreteCreator_t* creator1 = createConcreteCreator("factory1");
  concreteCreator_t* creator2 = createConcreteCreator("factory2");

  printf("===== Creator1 =====\n");
  ClienCode(creator1);
  printf("\n===== Creator2 =====\n");
  ClienCode(creator2);

  return 0;
}
