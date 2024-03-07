#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct _visitor_t visitor_t;
typedef struct _concreteElementA_t concreteElementA_t;
typedef struct _concreteElementB_t concreteElementB_t;
#define VISITOR_MEMBERS(TYPE)                                                                                          \
  void (*VisitConcreteElementA)(TYPE, concreteElementA_t*);                                                            \
  void (*VisitConcreteElementB)(TYPE, concreteElementB_t*);

#define ELEMENT_MEMBERS(TYPE) void (*Accept)(TYPE, visitor_t*);

typedef struct _visitor_t
{
  VISITOR_MEMBERS(struct _visitor_t*);
} visitor_t;

typedef struct _element_t
{
  ELEMENT_MEMBERS(struct _element_t*);
} element_t;

typedef struct _concreteVisitorA_t
{
  VISITOR_MEMBERS(struct _concreteVisitorA_t*);
} concreteVisitorA_t;

typedef struct _concreteVisitorB_t
{
  VISITOR_MEMBERS(struct _concreteVisitorB_t*);
} concreteVisitorB_t;

typedef struct _concreteElementA_t
{
  ELEMENT_MEMBERS(struct _concreteElementA_t*);
  char A;
} concreteElementA_t;

typedef struct _concreteElementB_t
{
  ELEMENT_MEMBERS(struct _concreteElementB_t*);
  char B;
} concreteElementB_t;

typedef struct _objectStructure_t
{
  void (*acceptElementA)(struct _objectStructure_t* obj, visitor_t* visitor);
  void (*acceptElementB)(struct _objectStructure_t* obj, visitor_t* visitor);
  concreteElementA_t* element_a;
  concreteElementB_t* element_b;
} objectStructure_t;

void objAcceptElementA(objectStructure_t* obj, visitor_t* visitor)
{
  visitor->VisitConcreteElementA(visitor, obj->element_a);
}

void objAcceptElementB(objectStructure_t* obj, visitor_t* visitor)
{
  visitor->VisitConcreteElementB(visitor, obj->element_b);
}

void AVisitConcreteElementA(concreteVisitorA_t* visitor, concreteElementA_t* element)
{
  printf("Visitor A visit element %c, method: %p\n", element->A, element->Accept);
}

void AVisitConcreteElementB(concreteVisitorA_t* visitor, concreteElementB_t* element)
{
  printf("Visitor A visit element %c, method: %p\n", element->B, element->Accept);
}

void BVisitConcreteElementA(concreteVisitorB_t* visitor, concreteElementA_t* element)
{
  printf("Visitor B visit element %c, method: %p\n", element->A, element->Accept);
}

void BVisitConcreteElementB(concreteVisitorB_t* visitor, concreteElementB_t* element)
{
  printf("Visitor B visit element %c, method: %p\n", element->B, element->Accept);
}

objectStructure_t* createObjectStructure(void)
{
  objectStructure_t* obj = malloc(sizeof(objectStructure_t));
  obj->acceptElementA = objAcceptElementA;
  obj->acceptElementB = objAcceptElementB;
  obj->element_a = malloc(sizeof(concreteElementA_t));
  memset(obj->element_a, 0, sizeof(concreteElementA_t));
  obj->element_b = malloc(sizeof(concreteElementB_t));
  memset(obj->element_b, 0, sizeof(concreteElementB_t));
  obj->element_a->A = 'A';
  obj->element_b->B = 'B';

  return obj;
}

concreteVisitorA_t* createConcreteVisitorA(void)
{
  concreteVisitorA_t* visitor = malloc(sizeof(concreteVisitorA_t));
  visitor->VisitConcreteElementA = AVisitConcreteElementA;
  visitor->VisitConcreteElementB = AVisitConcreteElementB;

  return visitor;
}

concreteVisitorB_t* createConcreteVisitorB(void)
{
  concreteVisitorB_t* visitor = malloc(sizeof(concreteVisitorB_t));
  visitor->VisitConcreteElementA = BVisitConcreteElementA;
  visitor->VisitConcreteElementB = BVisitConcreteElementB;

  return visitor;
}

void ClientCode(objectStructure_t* object, visitor_t* visitor)
{
  object->acceptElementA(object, visitor);
  object->acceptElementB(object, visitor);
}

int main()
{
  objectStructure_t* obj = createObjectStructure();
  concreteVisitorA_t* visitor_a = createConcreteVisitorA();
  concreteVisitorB_t* visitor_b = createConcreteVisitorB();

  printf("The client code works with all visitors via the base Visitor interface:\n");

  printf("\n===== Visitor A visites element A and B =====\n");
  ClientCode(obj, (visitor_t*)visitor_a);

  printf("\n===== Visitor B visites element A and B =====\n");
  ClientCode(obj, (visitor_t*)visitor_b);

  return 0;
}
