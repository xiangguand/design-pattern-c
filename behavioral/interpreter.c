#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _context_t context_t;

#define ABSTRACTEXPRESSION_MENBER(TYPE) int (*interpreter)(TYPE, context_t);

typedef struct _context_t
{
  char value[100];
} context_t;

typedef struct _abstractExpression_t
{
  ABSTRACTEXPRESSION_MENBER(struct _abstractExpression_t*);
} abstractExpression_t;

typedef struct _terminalExpression_t
{
  ABSTRACTEXPRESSION_MENBER(struct _terminalExpression_t*);
  int (*get_result)(struct _terminalExpression_t*);
  int res;
} terminalExpression_t;

typedef struct _nonTerminalExpression_t
{
  ABSTRACTEXPRESSION_MENBER(struct _nonTerminalExpression_t*);
  int (*get_result)(struct _nonTerminalExpression_t*);
  int res;
} nonTerminalExpression_t;

int terminalExpression(terminalExpression_t* expression, context_t context)
{
  return expression->res = ((int)context.value[0] + (int)context.value[1]);
}

int nonTerminalExpression(nonTerminalExpression_t* expression, context_t context)
{
  return expression->res = ((int)context.value[0] * (int)context.value[1]);
}

terminalExpression_t* createTerminalExpress()
{
  terminalExpression_t* express = malloc(sizeof(terminalExpression_t));
  express->interpreter = terminalExpression;

  return express;
}

nonTerminalExpression_t* createNonTerminalExpress()
{
  nonTerminalExpression_t* express = malloc(sizeof(nonTerminalExpression_t));
  express->interpreter = nonTerminalExpression;

  return express;
}

context_t* createTestContext()
{
  context_t* context = malloc(sizeof(context_t));
  memset(context, 0, sizeof(context_t));
  context->value[0] = 'A';
  context->value[1] = 'B';

  return context;
}

int main()
{
  context_t* context = createTestContext();
  terminalExpression_t* terminal_express = createTerminalExpress();
  nonTerminalExpression_t* non_terminal_express = createNonTerminalExpress();

  abstractExpression_t* express = (abstractExpression_t*)terminal_express;
  for (int i = 0; i < 2; i++)
  {
    if (context->value[i] == 'A')
    {
      express = (abstractExpression_t*)terminal_express;
    }
    else
    {
      express = (abstractExpression_t*)non_terminal_express;
    }

    int res = express->interpreter(express, *context);

    printf("input: %c, res: %d\n", context->value[i], res);
  }

  printf("A expect: %d\n", 'A' + 'B');
  printf("B expect: %d\n", 'A' * 'B');

  return 0;
}
