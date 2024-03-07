#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define BUILDER_MEMBER(TYPE)                                                                                           \
  product_t* (*getResult)(TYPE);                                                                                       \
  TYPE (*producePartA)(TYPE);                                                                                          \
  TYPE (*producePartB)(TYPE);                                                                                          \
  TYPE (*producePartC)(TYPE)

typedef struct _product_t
{
  char* part[10];
  int count;
  void (*listPart)(struct _product_t*);
} product_t;

typedef struct _builder_t
{
  BUILDER_MEMBER(struct _builder_t*);
} builder_t;

typedef struct _concreteBuilder_t
{
  BUILDER_MEMBER(struct _concreteBuilder_t*);
  product_t* product;
} concreteBuilder_t;

typedef struct _director_t
{
  void (*set_builder)(struct _director_t*, builder_t* builder);
  void (*build_partA)(struct _director_t*);
  void (*build_full_feature)(struct _director_t*);
  builder_t* builder;
} director_t;

void productListPart(struct _product_t* product)
{
  for (int i = 0; i < product->count; i++)
  {
    printf("=> %s\n", product->part[i]);
  }
}

product_t* builderGetResult(concreteBuilder_t* builder)
{
  product_t* old = builder->product;
  builder->product = malloc(sizeof(product_t));
  memset(builder->product, 0, sizeof(product_t));
  builder->product->listPart = productListPart;

  return old;
}

concreteBuilder_t* builderProducePartA(concreteBuilder_t* builder)
{
  builder->product->part[builder->product->count++] = "PartA is built";

  return builder;
}
concreteBuilder_t* builderProducePartB(concreteBuilder_t* builder)
{
  builder->product->part[builder->product->count++] = "PartB is built";

  return builder;
}
concreteBuilder_t* builderProducePartC(concreteBuilder_t* builder)
{
  builder->product->part[builder->product->count++] = "PartC is built";

  return builder;
}

void director_set_builder(struct _director_t* director, builder_t* builder)
{
  director->builder = builder;
}

void director_build_partA(struct _director_t* director)
{
  director->builder->producePartA(director->builder);
}

void director_build_full_feature(struct _director_t* director)
{
  builder_t *builder = director->builder;
  builder->producePartA(builder)->producePartB(builder)->producePartC(builder);
}

concreteBuilder_t* createConcreteBuild()
{
  concreteBuilder_t* builder = malloc(sizeof(concreteBuilder_t));
  memset(builder, 0, sizeof(concreteBuilder_t));
  builder->product = malloc(sizeof(product_t));
  memset(builder->product, 0, sizeof(product_t));
  builder->product->listPart = productListPart;
  builder->getResult = builderGetResult;
  builder->producePartA = builderProducePartA;
  builder->producePartB = builderProducePartB;
  builder->producePartC = builderProducePartC;

  return builder;
}

director_t* createConcreteDirector()
{
  director_t* director = malloc(sizeof(director_t));
  memset(director, 0, sizeof(director_t));
  director->set_builder = director_set_builder;
  director->build_full_feature = director_build_full_feature;
  director->build_partA = director_build_partA;

  return director;
}

void ClientCode(director_t* director)
{
  concreteBuilder_t* builder = createConcreteBuild();

  printf("Build only partA\n");
  director->set_builder(director, (builder_t*)builder);
  director->build_partA(director);

  product_t* product = builder->getResult(builder);
  product->listPart(product);

  printf("\nBuild whole parts\n");
  director->build_full_feature(director);

  product = builder->getResult(builder);
  product->listPart(product);

  printf("\nCustom build\n");
  builder->producePartA(builder)->producePartC(builder);
  product = builder->getResult(builder);
  product->listPart(product);
}

int main()
{
  director_t* director = createConcreteDirector();
  ClientCode(director);

  return 0;
}
