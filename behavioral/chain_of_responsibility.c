#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define ID1 (1)
#define ID2 (2)
#define ID3 (3)

#define HANDLER_MEMBERS(TYPE)                                                                                          \
  TYPE (*HandlerRequest)(TYPE, int id);                                                                                \
  void (*SetSuccesssor)(TYPE, TYPE);                                                                                   \
  TYPE next_handler

typedef struct _handler_t
{
  HANDLER_MEMBERS(struct _handler_t*);
} handler_t;

//! Handler

handler_t* handlerRequest(handler_t* handler, int id)
{
  if (handler->next_handler)
  {
    return handler->next_handler->HandlerRequest(handler->next_handler, id);
  }

  return NULL;
}

void setSuccesssor(handler_t* handler1, handler_t* handler2)
{
  handler1->next_handler = handler2;
}

typedef struct _concreteHandler1_t
{
  HANDLER_MEMBERS(struct _concreteHandler1_t*);
} concreteHandler1_t;

typedef struct _concreteHandler2_t
{
  HANDLER_MEMBERS(struct _concreteHandler2_t*);
} concreteHandler2_t;

typedef struct _concreteHandler3_t
{
  HANDLER_MEMBERS(struct _concreteHandler3_t*);
} concreteHandler3_t;

handler_t* concreteHandler1Request(handler_t* handler, int id)
{
  printf("Handle1\n");
  handler_t* ret = NULL;
  if (ID1 == id)
  {
    ret = handler;
  }
  else if(handler != NULL)
  {
    ret = handlerRequest(handler, id);
  }

  return ret;
}

handler_t* concreteHandler2Request(handler_t* handler, int id)
{
  printf("Handle2\n");
  handler_t* ret = NULL;
  if (ID2 == id)
  {
    ret = handler;
  }
  else if(handler != NULL)
  {
    ret = handlerRequest(handler, id);
  }
  
  return ret;
}

handler_t* concreteHandler3Request(handler_t* handler, int id)
{
  printf("Handle3\n");
  handler_t* ret = NULL;
  if (ID3 == id)
  {
    ret = handler;
  }
  else if(handler != NULL)
  {
    ret = handlerRequest(handler, id);
  }
  
  return ret;
}


void ClientCode(handler_t* handler)
{
  handler_t* ret = NULL;
  for (int i = 0; i <= 5; i++)
  {
    printf("===== With ID %d =====\n", i);
    ret = handler->HandlerRequest(handler, i);
    if (ret != NULL)
    {
      printf("Get id %d\n", i);
    }
    else
    {
      printf("Fail to get id %d\n", i);
    }
  }
}

int main()
{
  concreteHandler1_t handle1 = { .HandlerRequest = concreteHandler1Request,
                                 .SetSuccesssor = setSuccesssor,
                                 .next_handler = NULL };
  concreteHandler2_t handle2 = { .HandlerRequest = concreteHandler2Request,
                                 .SetSuccesssor = setSuccesssor,
                                 .next_handler = NULL };
  concreteHandler3_t handle3 = { .HandlerRequest = concreteHandler3Request,
                                 .SetSuccesssor = setSuccesssor,
                                 .next_handler = NULL };

  handle3.SetSuccesssor(&handle3, &handle2);
  handle2.SetSuccesssor(&handle2, &handle1);

  printf("===== handler3 =====\n");
  ClientCode(&handle3);

  printf("\n===== handler2 =====\n");
  ClientCode(&handle2);

  return 0;
}
