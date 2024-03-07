#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define MEDIATOR_MEMBERS(TYPE) void (*Send)(TYPE, const char**)
#define COLLEAGURE_MEMBERS(TYPE)                                                                                       \
  void (*send)(TYPE, const char**);                                                                                           \
  void (*notify)(TYPE, char*);                                                                                         \
  const char* message;

#define container_of(ptr, type, member)                                                                                \
  ({                                                                                                                    \
    const typeof(((type*)0)->member)* __pmember = (ptr);                                                               \
    (type*)((char*)__pmember - offsetof(type, member));                                                                \
  })

typedef struct _colleague_t colleagure_t;
typedef struct _concreteColleague1_t concreteColleague1_t;
typedef struct _concreteColleague2_t concreteColleague2_t;

typedef struct _mediator_t
{
  MEDIATOR_MEMBERS(struct _mediator_t*);
} mediator_t;

typedef struct _concreteMediator_t
{
  MEDIATOR_MEMBERS(struct _concreteMediator_t*);
  concreteColleague1_t* c1;
  concreteColleague2_t* c2;
} concreteMediator_t;

typedef struct _colleague_t
{
  mediator_t* mediator;
  COLLEAGURE_MEMBERS(struct _colleague_t*);
} colleague_t;

typedef struct _concreteColleague1_t
{
  concreteMediator_t* mediator;
  COLLEAGURE_MEMBERS(struct _concreteColleague1_t*);
  void (*joba)(struct _concreteColleague1_t*, const char* message);
  void (*jobb)(struct _concreteColleague1_t*, const char* message);
} concreteColleague1_t;

typedef struct _concreteColleague2_t
{
  concreteMediator_t* mediator;
  COLLEAGURE_MEMBERS(struct _concreteColleague2_t*);
  void (*jobc)(struct _concreteColleague2_t*, const char* message);
  void (*jobd)(struct _concreteColleague2_t*, const char* message);
} concreteColleague2_t;

void colleagueJoba(struct _concreteColleague1_t* c1, const char* message)
{
  printf("JobA! send to mediator\n");
  c1->message = message;
  c1->mediator->Send(c1->mediator, &c1->message);
}

void colleagueJobb(struct _concreteColleague1_t* c1, const char *message)
{
  printf("JobB! notify from mediator\n");
  printf("C1 receive: %s\n", message);
}

void colleagueJobc(struct _concreteColleague2_t* c2, const char* message)
{
  printf("JobC! send to mediator\n");
  c2->message = message;
  c2->mediator->Send(c2->mediator, &c2->message);
}

void colleagueJobd(struct _concreteColleague2_t* c2, const char *message)
{
  printf("JobD! notify from mediator\n");
  printf("C2 receive: %s\n", message);
}

void mediatorSendHandler(concreteMediator_t* mediator, const char** message)
{
  colleague_t* c = container_of(message, colleague_t, message);

  if (mediator->c1 == (concreteColleague1_t *)c)
  {
    // call jobd
    printf("Mediator call colleage2 JobD\n");
    mediator->c2->jobd(mediator->c2, *message);
  }
  else
  {
    printf("Mediator call colleage1 JobB\n");
    mediator->c1->jobb(mediator->c1, *message);
  }
}

concreteColleague1_t* createConcreteColleague1(concreteMediator_t* mediator)
{
  concreteColleague1_t* colleague = malloc(sizeof(concreteColleague1_t));
  memset(colleague, 0, sizeof(concreteColleague1_t));
  colleague->mediator = mediator;
  colleague->joba = colleagueJoba;
  colleague->jobb = colleagueJobb;

  return colleague;
}

concreteColleague2_t* createConcreteColleague2(concreteMediator_t* mediator)
{
  concreteColleague2_t* colleague = malloc(sizeof(concreteColleague2_t));
  memset(colleague, 0, sizeof(concreteColleague2_t));
  colleague->mediator = mediator;
  colleague->jobc = colleagueJobc;
  colleague->jobd = colleagueJobd;

  return colleague;
}

concreteMediator_t* createConcreteMediator()
{
  concreteMediator_t* mediator = malloc(sizeof(concreteMediator_t));
  memset(mediator, 0, sizeof(concreteMediator_t));
  mediator->Send = mediatorSendHandler;

  return mediator;
}

void createConcreteMediatorSetColleage(concreteMediator_t* mediator, concreteColleague1_t* c1, concreteColleague2_t* c2)
{
  mediator->c1 = c1;
  mediator->c2 = c2;
}

int main()
{
  concreteMediator_t* mediator = createConcreteMediator();
  concreteColleague1_t* c1 = createConcreteColleague1(mediator);
  concreteColleague2_t* c2 = createConcreteColleague2(mediator);
  createConcreteMediatorSetColleage(mediator, c1, c2);

  printf("===== Call colleage1 c1->joba() =====\n");
  c1->joba(c1, "I am c1.\n");

  printf("\n===== Call colleage2 c2->jobc() =====\n");
  c2->jobc(c2, "I am c2\n");

  return 0;
}
