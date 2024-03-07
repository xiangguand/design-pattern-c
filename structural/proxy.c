#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define SUBJECT_MEMBERS(TYPE) void (*Request)(TYPE);

typedef struct _subject_t
{
  SUBJECT_MEMBERS(struct _subject_t*);
} subject_t;

typedef struct _realSubject_t
{
  SUBJECT_MEMBERS(struct _realSubject_t*);
  char* desc;
} realSubject_t;

typedef struct _proxy_t
{
  SUBJECT_MEMBERS(struct _proxy_t*);
  realSubject_t* real_subject;
  void (*logging)(struct _proxy_t*);
} proxy_t;

void realSubjectRequest(realSubject_t* realSubject)
{
  printf("Real subject handler, desc: %s\n", realSubject->desc);
}

void proxyLogging(proxy_t* proxy)
{
  printf("Proxy logging...\n");
}

void proxyRequest(proxy_t* proxy)
{
  printf("Start proxy\n");
  proxy->real_subject->Request(proxy->real_subject);
  proxy->logging(proxy);
  printf("End proxy\n");
}

realSubject_t* g_real_subject = NULL;
proxy_t* createProxy()
{
  proxy_t* proxy = malloc(sizeof(proxy_t));
  proxy->logging = proxyLogging;
  proxy->Request = proxyRequest;

  //! real subject is a singleton, see creational design pattern
  if (g_real_subject == NULL)
  {
    g_real_subject = malloc(sizeof(realSubject_t));
    g_real_subject->Request = realSubjectRequest;
    g_real_subject->desc = malloc(100);
    memset(g_real_subject->desc, 0, 100);
    memcpy(g_real_subject->desc, "This is real subject", sizeof("This is real subject"));
  }
  proxy->real_subject = g_real_subject;

  return proxy;
}

int main()
{
  printf("===== Proxy =====\n");
  proxy_t* proxy = createProxy();
  proxy->Request(proxy);

  printf("===== Proxy2 =====\n");
  proxy_t* proxy2 = createProxy();
  proxy2->Request(proxy2);

  assert(proxy != proxy2);
  assert(proxy->real_subject == proxy2->real_subject);

  printf("===== Force running realSubject =====\n");
  g_real_subject->Request(g_real_subject);

  return 0;
}
