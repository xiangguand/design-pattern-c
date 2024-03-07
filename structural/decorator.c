#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define COMPONENT_MEMBERS(TYPE) void (*Operation)(TYPE)

typedef struct _component_t
{
  COMPONENT_MEMBERS(struct _component_t*);
} component_t;

typedef struct _concreteComponent_t
{
  COMPONENT_MEMBERS(struct _concreteComponent_t*);
} concreteComponent_t;

typedef struct _decorator_t
{
  COMPONENT_MEMBERS(struct _decorator_t*);
  void (*setComponent)(struct _decorator_t*, component_t* conponent);
  component_t* component;
} decorator_t;

typedef struct _concreteDecoratorA_t
{
  COMPONENT_MEMBERS(struct _concreteDecoratorA_t*);
  void (*setComponent)(struct _decorator_t*, component_t* conponent);
  component_t* component;
  char addedState[30];
} concreteDecoratorA_t;

typedef struct _concreteDecoratorB_t
{
  COMPONENT_MEMBERS(struct _concreteDecoratorB_t*);
  void (*setComponent)(struct _decorator_t*, component_t* conponent);
  component_t* component;
  void (*AddedBahavior)(struct _concreteDecoratorB_t *);
} concreteDecoratorB_t;

void concreteOperation(component_t* component)
{
  printf("simple component run ~\n");
}

void decoratorSetCompnent(decorator_t* decorator, component_t* component)
{
  decorator->component = component;
}

void decoratorAOperation(concreteDecoratorA_t* dec_a)
{
  printf("decorator A start\n");
  dec_a->component->Operation(dec_a->component);
  printf("decorator A state string: %s\n", dec_a->addedState);
  printf("decorator A end\n");
}

void decoratorBOperation(concreteDecoratorB_t* dec_b)
{
  printf("decorator B start\n");
  dec_b->component->Operation(dec_b->component);
  dec_b->AddedBahavior(dec_b);
  printf("decorator B end\n");
}

void decoratorBAddedBahavior(concreteDecoratorB_t dec_b)
{
  printf("decoratorBAddedBahavior\n");
}

void ClientCode(component_t* component)
{
  component->Operation(component);
}

int main()
{
  printf("===== Simple component =====\n");
  concreteComponent_t simple = { .Operation = concreteOperation };
  ClientCode(&simple);

  printf("\n===== Decorated component =====\n");
  concreteDecoratorA_t dec_a = {
    .Operation = decoratorAOperation,
    .setComponent = decoratorSetCompnent,
    .addedState = "Hello A",
  };
  concreteDecoratorB_t dec_b = {
    .Operation = decoratorBOperation,
    .setComponent = decoratorSetCompnent,
    .AddedBahavior = decoratorBAddedBahavior,
  };

  dec_a.setComponent((decorator_t *)&dec_a, (component_t *)&simple);
  dec_b.setComponent((decorator_t *)&dec_b, (component_t *)&dec_a);

  ClientCode((component_t *)&dec_b);

  printf("\n===== End of program =====\n");

  return 0;
}
