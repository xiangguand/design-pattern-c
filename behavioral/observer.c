#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define OBSERVER_MEMBERS(TYPE) void (*update)(TYPE)
#define SUBJECT_MEMBERS(TYPE, OBV_TYPE)                                                                                \
  void (*attach)(TYPE, OBV_TYPE);                                                                                      \
  void (*detech)(TYPE, OBV_TYPE);                                                                                      \
  void (*notify)(TYPE)

typedef struct _observer_t
{
  OBSERVER_MEMBERS(struct _observer_t*);
} observer_t;

typedef struct _concreteObserver_t
{
  OBSERVER_MEMBERS(struct _concreteObserver_t*);
  char* observer_string;
} concreteObserver_t;

typedef struct _subject_t
{
  SUBJECT_MEMBERS(struct _subject_t*, observer_t*);
} subject_t;

typedef struct _concreteSubject_t
{
  SUBJECT_MEMBERS(struct _concreteSubject_t*, concreteObserver_t* observer);
  char subject_state[20];
  concreteObserver_t* observer[20];
  int count;
} concreteSubject_t;

void concreteSubjectAttach(concreteSubject_t* subject, concreteObserver_t* observer)
{
  for (int i = 0; i < 20; i++)
  {
    if (subject->observer[i] == NULL)
    {
      subject->observer[i] = observer;
      subject->count++;
      break;
    }
  }
}
void concreteSubjectDetech(concreteSubject_t* subject, concreteObserver_t* observer)
{
  for (int i = 0; i < 20; i++)
  {
    if (subject->observer[i] == observer)
    {
      subject->observer[i] = NULL;
      subject->count--;
      break;
    }
  }
}
void concreteSubjectNotify(concreteSubject_t* subject)
{
  printf("Subject notify !!!\n");
  for (int i = 0; i < 20; i++)
  {
    if (subject->observer[i] != NULL)
    {
      subject->observer[i]->update(subject->observer[i]);
    }
  }
}

void concreteObserverUpdateHandle(concreteObserver_t* observer)
{
  printf("Observer update: %s\n", observer->observer_string);
}

void freeObserver(concreteObserver_t* observer)
{
  free(observer->observer_string);
  free(observer);
}

concreteObserver_t* createConcreteObserver(const char* observer_string)
{
  concreteObserver_t* observer = malloc(sizeof(concreteObserver_t));
  memset(observer, 0, sizeof(concreteObserver_t));
  observer->update = concreteObserverUpdateHandle;
  observer->observer_string = malloc(strlen(observer_string) + 1);
  memset(observer->observer_string, 0, strlen(observer_string) + 1);
  memcpy(observer->observer_string, observer_string, strlen(observer_string));

  return observer;
}

concreteSubject_t* createConcreteSubject(const char* subject_state)
{
  concreteSubject_t* subject = malloc(sizeof(concreteSubject_t));
  memset(subject, 0, sizeof(concreteSubject_t));
  subject->attach = concreteSubjectAttach;
  subject->detech = concreteSubjectDetech;
  subject->notify = concreteSubjectNotify;
  memcpy(subject->subject_state, subject_state, strlen(subject_state));

  return subject;
}

int main()
{
  concreteObserver_t* obv1 = createConcreteObserver("I am observer 1. I received !");
  concreteObserver_t* obv2 = createConcreteObserver("I am observer 2. I received !");
  concreteObserver_t* obv3 = createConcreteObserver("I am observer 3. I received !");

  concreteSubject_t* subject = createConcreteSubject("I am subject\n");

  subject->attach(subject, obv1);
  subject->attach(subject, obv2);
  printf("===== Attach observer1 observer2 =====\n");
  subject->notify(subject);

  subject->attach(subject, obv3);
  printf("\n===== Attach observer1 observer2 observer3 =====\n");
  subject->notify(subject);

  subject->detech(subject, obv2);
  printf("\n===== Attach observer1 observer3 =====\n");
  subject->notify(subject);

  freeObserver(obv1);
  freeObserver(obv2);
  freeObserver(obv3);

  return 0;
}
