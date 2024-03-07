#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define STRATEGY_MEMBERS(TYPE) int (*algorithmInterface)(TYPE, int, int)

typedef struct _strategy_t strategy_t;
typedef struct _context_t
{
  int (*contextInterface)(struct _context_t*, int a, int b);
  void (*contextSetStrategy)(struct _context_t*, strategy_t *strategy);
  strategy_t* strategy;
} context_t;

typedef struct _strategy_t
{
  STRATEGY_MEMBERS(struct _strategy_t*);
} strategy_t;

typedef struct _concreteStrategA_t
{
  STRATEGY_MEMBERS(struct _concreteStrategA_t*);
} concreteStrategA_t;

typedef struct _concreteStrategB_t
{
  STRATEGY_MEMBERS(struct _concreteStrategB_t*);
} concreteStrategB_t;

typedef struct _concreteStrategC_t
{
  STRATEGY_MEMBERS(struct _concreteStrategC_t*);
} concreteStrategC_t;


int contextAlgorithmInterface(context_t* context, int a, int b)
{
  return context->strategy->algorithmInterface(context->strategy, a, b);
}

void contextSetStrategy(context_t* context, strategy_t *strategy)
{
  context->strategy = strategy;
}

int algorithmStrategyA(concreteStrategA_t *strategy, int a, int b)
{
  return a + b;
}

int algorithmStrategyB(concreteStrategB_t *strategy, int a, int b)
{
  return a - b;
}

int algorithmStrategyC(concreteStrategC_t *strategy, int a, int b)
{
  return a * b;
}

context_t *createContext()
{
  context_t *context = malloc(sizeof(context_t));
  memset(context, 0, sizeof(context_t));
  context->strategy = (strategy_t *)algorithmStrategyA;
  context->contextInterface = contextAlgorithmInterface;
  context->contextSetStrategy = contextSetStrategy;

  return context;
}


concreteStrategA_t *createConcreteStratygyA()
{
  concreteStrategA_t *strategy = malloc(sizeof(concreteStrategA_t));
  memset(strategy, 0, sizeof(concreteStrategA_t));
  strategy->algorithmInterface = algorithmStrategyA;

  return strategy;
}

concreteStrategB_t *createConcreteStratygyB()
{
  concreteStrategB_t *strategy = malloc(sizeof(concreteStrategB_t));
  memset(strategy, 0, sizeof(concreteStrategB_t));
  strategy->algorithmInterface = algorithmStrategyB;

  return strategy;
}

concreteStrategC_t *createConcreteStratygyC()
{
  concreteStrategC_t *strategy = malloc(sizeof(concreteStrategC_t));
  memset(strategy, 0, sizeof(concreteStrategC_t));
  strategy->algorithmInterface = algorithmStrategyC;

  return strategy;
}


int main()
{
  int a = 30;
  int b = 15;

  context_t *context = createContext();

  context->contextSetStrategy(context, (strategy_t *)createConcreteStratygyA());
  printf("Algorithm A, input = 30, 15, output: %d\n", context->contextInterface(context, a, b));

  context->contextSetStrategy(context, (strategy_t *)createConcreteStratygyB());
  printf("Algorithm A, input = 30, 15, output: %d\n", context->contextInterface(context, a, b));
  
  context->contextSetStrategy(context, (strategy_t *)createConcreteStratygyC());
  printf("Algorithm A, input = 30, 15, output: %d\n", context->contextInterface(context, a, b));

  return 0;
}
