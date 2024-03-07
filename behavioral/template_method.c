#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define ABSTRACT_MEMBERS(TYPE)                                                                                         \
  void (*primitiveOperation1)(TYPE);                                                                                   \
  void (*primitiveOperation2)(TYPE);                                                                                   \
  void (*templateMethod)(TYPE)

typedef struct _abstractClass_t
{
  ABSTRACT_MEMBERS(struct _abstractClass_t*);
} abstractClass_t;

typedef struct _concreteClass1_t
{
  ABSTRACT_MEMBERS(struct _concreteClass1_t*);
  int val100;
} concreteClass1_t;

typedef struct _concreteClass2_t
{
  ABSTRACT_MEMBERS(struct _concreteClass2_t*);
  float f3_14;
} concreteClass2_t;

void abstractPrimitiveOperation1(abstractClass_t* absctract)
{
  printf("abstractPrimitiveOperation1\n");
}
void abstractPrimitiveOperation2(abstractClass_t* absctract)
{
  printf("abstractPrimitiveOperation2\n");
}
void abstractTemplateMethod(abstractClass_t* absctract)
{
  absctract->primitiveOperation1(absctract);
  absctract->primitiveOperation2(absctract);
  printf("abstractTemplateMethod\n");
}

void concreteClass1PrimitiveOperation1(concreteClass1_t* class)
{
  printf("concreteClass1PrimitiveOperation1\n");
}
void concreteClass1PrimitiveOperation2(concreteClass1_t* class)
{
  printf("concreteClass1PrimitiveOperation2\n");
}
void concreteClass1TemplateMethod(concreteClass1_t* class)
{
  class->primitiveOperation1(class);
  class->primitiveOperation2(class);
  printf("concreteClass1TemplateMethod: %d\n", class->val100);
}

void concreteClass2PrimitiveOperation1(concreteClass2_t* class)
{
  printf("concreteClass2PrimitiveOperation1\n");
}
void concreteClass2PrimitiveOperation2(concreteClass2_t* class)
{
  printf("concreteClass2PrimitiveOperation2\n");
}
void concreteClass2TemplateMethod(concreteClass2_t* class)
{
  class->primitiveOperation1(class);
  class->primitiveOperation2(class);
  printf("concreteClass2TemplateMethod: %.2f\n", class->f3_14);
}

abstractClass_t* createAbsctractClass()
{
  abstractClass_t* class = malloc(sizeof(abstractClass_t));
  class->primitiveOperation1 = abstractPrimitiveOperation1;
  class->primitiveOperation2 = abstractPrimitiveOperation2;
  class->templateMethod = abstractTemplateMethod;

  return class;
}

concreteClass1_t* createConcreteClass1()
{
  concreteClass1_t* class = malloc(sizeof(concreteClass1_t));
  class->primitiveOperation1 = concreteClass1PrimitiveOperation1;
  class->primitiveOperation2 = concreteClass1PrimitiveOperation2;
  class->templateMethod = concreteClass1TemplateMethod;
  class->val100 = 100;

  return class;
}

concreteClass2_t* createConcreteClass2()
{
  concreteClass2_t* class = malloc(sizeof(concreteClass2_t));
  class->primitiveOperation1 = concreteClass2PrimitiveOperation1;
  class->primitiveOperation2 = concreteClass2PrimitiveOperation2;
  class->templateMethod = concreteClass2TemplateMethod;
  class->f3_14 = 3.14;

  return class;
}

void ClientCode(void* class)
{
  abstractClass_t* class_ptr = (abstractClass_t*)class;
  class_ptr->templateMethod(class_ptr);
}

int main()
{
  printf("The same client code, can feed and run differnt subclass\n");
  abstractClass_t* class = createAbsctractClass();
  concreteClass1_t *concrete_class1 = createConcreteClass1();
  concreteClass2_t *concrete_class2 = createConcreteClass2();

  printf("===== Feed abstract class =====\n");
  ClientCode((void*)class);
  printf("\n===== Feed concrete class 1 =====\n");
  ClientCode((void*)concrete_class1);
  printf("\n===== Feed concrete class 2 =====\n");
  ClientCode((void*)concrete_class2);

  return 0;
}
