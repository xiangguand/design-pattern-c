#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define REQ_MEMBERS(TYPE) void (*Request)(TYPE);

typedef struct _adaptee_t
{
  void (*SpecificRequest)(void);
} adaptee_t;
void adapteeSpecificRequest(adaptee_t* adaptee)
{
  printf("adapteeSpecificRequest\n");
}

typedef struct _target_t
{
  REQ_MEMBERS(struct _target_t* obj);
} target_t;
void targetRequest(target_t* target)
{
  printf("targetRequest\n");
}

typedef struct _adapter_t
{
  REQ_MEMBERS(struct _adapter_t* obj);
  adaptee_t* adaptee;
} adapter_t;

void adapterRequest(adapter_t* adapter)
{
  adapter->adaptee->SpecificRequest();
}

void clientCode(target_t* target)
{
  target->Request(target);
}

int main()
{
  //! Target
  target_t target = { .Request = targetRequest };

  //! Adaptee
  adaptee_t adaptee = { .SpecificRequest = adapteeSpecificRequest };

  //! Adapter
  adapter_t adapter = { .Request = adapterRequest, .adaptee = &adaptee };

  printf("Test target can work\n");
  clientCode(&target);

  printf("\nTest adapter can work as adaptee\n");
  clientCode(&adapter);

  printf("\n===== End of program =====\n");

  return 0;
}
