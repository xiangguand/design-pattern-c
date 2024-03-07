#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define OPERATION_MEMBERS(TYPE)      void (*operation)(TYPE);
#define OPERATION_IMPL_MEMBERS(TYPE) void (*operationImpl)(TYPE);
#define ABSTRATION_MEMBERS(TYPE)      void (*operation)(TYPE);

typedef struct _implementor_t
{
  OPERATION_IMPL_MEMBERS(struct _implementor_t*);
} implementor_t;

typedef struct _concreteImplementorA_t
{
  OPERATION_IMPL_MEMBERS(struct _concreteImplementorA_t*);
} concreteImplementorA_t;

typedef struct _concreteImplementorB_t
{
  OPERATION_IMPL_MEMBERS(struct _concreteImplementorB_t*);
} concreteImplementorB_t;

typedef struct _abstraction_t
{
  OPERATION_MEMBERS(struct _abstraction_t*);
  implementor_t *impl;
} abstraction_t;

typedef struct _extAbstraction_t
{
  OPERATION_MEMBERS(struct _extAbstraction_t*);
  implementor_t *impl;
  int val_12345;
} extAbstraction_t;

void ClientCode(abstraction_t* abstraction)
{
  abstraction->operation(abstraction);
}

void impleA(concreteImplementorA_t *obj)
{
  printf("impleA\n");
}

void impleB(concreteImplementorA_t *obj)
{
  printf("impleB\n");
}

void abstractionOper(abstraction_t *abstraction)
{
  printf("Abstraction operation\n");
  abstraction->impl->operationImpl(abstraction->impl);
}

void extAbstractionOper(extAbstraction_t *extAbstraction)
{
  printf("ExtendAbstraction operation, %d\n", extAbstraction->val_12345);
  extAbstraction->impl->operationImpl(extAbstraction->impl);
}

int main()
{
  //! implemantation decalare
  implementor_t a_impl = {.operationImpl = impleA};
  implementor_t b_impl = {.operationImpl = impleB};

  printf("Abstraction\n");
  //! new one
  abstraction_t abstraction = {.operation=abstractionOper,.impl=&a_impl};  
  ClientCode(&abstraction);
  
  printf("\nExtended abstraction\n");
  //! new one
  extAbstraction_t ext_abstraction = {.operation=extAbstractionOper,.impl=&b_impl,.val_12345=12345};  
  ClientCode(&ext_abstraction);


  printf("\n===== End of program =====\n");

  return 0;
}
