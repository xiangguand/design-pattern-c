#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// A -> B
// B -> C
// C -> D
// C -> A
// D -> A

#define STATE_MEMBERS(TYPE, CONTEXT) void (*handle)(TYPE, CONTEXT)

typedef struct _state_t state_t;
typedef struct _context_t
{
  void (*request)(struct _context_t*);
  void (*transition)(struct _context_t*, state_t*);
  state_t* state;
} context_t;

typedef struct _state_t
{
  STATE_MEMBERS(struct _state_t*, context_t*);
} state_t;

typedef struct _stateA_t
{
  STATE_MEMBERS(struct _stateA_t*, context_t*);
  int a;
} stateA_t;

typedef struct _stateB_t
{
  STATE_MEMBERS(struct _stateB_t*, context_t*);
  int b;
} stateB_t;

typedef struct _stateC_t
{
  STATE_MEMBERS(struct _stateC_t*, context_t*);
  int c;
} stateC_t;

typedef struct _stateD_t
{
  STATE_MEMBERS(struct _stateD_t*, context_t*);
  int d;
} stateD_t;

void handleA(stateA_t* state, context_t* context);
void handleB(stateB_t* state, context_t* context);
void handleC(stateC_t* state, context_t* context);
void handleD(stateD_t* state, context_t* context);

void contextRequset(context_t* context)
{
  context->state->handle(context->state, context);
}

void contextTransition(struct _context_t* context, state_t* state)
{
  context->state = state;
}

stateA_t* createStateAInstance(void)
{
  stateA_t* state = malloc(sizeof(stateA_t));
  memset(state, 0, sizeof(stateA_t));
  state->handle = handleA;

  return state;
}

stateB_t* createStateBInstance(void)
{
  stateB_t* state = malloc(sizeof(stateB_t));
  memset(state, 0, sizeof(stateB_t));
  state->handle = handleB;

  return state;
}

stateC_t* createStateCInstance(void)
{
  stateC_t* state = malloc(sizeof(stateC_t));
  memset(state, 0, sizeof(stateC_t));
  state->handle = handleC;

  return state;
}

stateD_t* createStateDInstance(void)
{
  stateD_t* state = malloc(sizeof(stateD_t));
  memset(state, 0, sizeof(stateD_t));
  state->handle = handleD;

  return state;
}

void handleA(stateA_t* state, context_t* context)
{
  printf("StateA, transition when a equal 3, A value: %d\n", ++state->a);
  if (state->a >= 3)
  {
    printf("Transition to stateB\n");
    free(state);
    context->transition(context, (state_t*)createStateBInstance());
  }
}

void handleB(stateB_t* state, context_t* context)
{
  printf("StateB, transition when b equal 2, A value: %d\n", ++state->b);
  if (state->b >= 2)
  {
    printf("Transition to stateC\n");
    free(state);
    context->transition(context, (state_t*)createStateCInstance());
  }
}

void handleC(stateC_t* state, context_t* context)
{
  printf("StateC, random transition to A or D\n");

  state->c = rand();
  if (state->c & 1)
  {
    printf("Transition to stateA\n");
    free(state);
    context->transition(context, (state_t*)createStateAInstance());
  }
  else
  {
    printf("Transition to stateD\n");
    free(state);
    context->transition(context, (state_t*)createStateDInstance());
  }
}

void handleD(stateD_t* state, context_t* context)
{
  printf("StateD, transition to A\n");

  printf("Transition to stateA\n");
  free(state);
  context->transition(context, (state_t*)createStateAInstance());
}

context_t* createContext()
{
  context_t* context = malloc(sizeof(context_t));
  context->request = contextRequset;
  context->transition = contextTransition;
  context->state = (state_t*)createStateAInstance();
}

int main()
{
  // A -> B
  // B -> C
  // C -> D
  // C -> A
  // D -> A
  context_t* context = createContext();
  for (int i = 0; i < 20; i++)
  {
    context->request(context);
  }

  return 0;
}
