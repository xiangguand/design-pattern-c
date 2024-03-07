#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct _singleton_t
{
  struct _singleton_t* (*Instance)(struct _singleton_t*);
  int (*Value)(struct _singleton_t*);
  int value_;
} singleton_t;

singleton_t* g_singleton = NULL;

struct _singleton_t* singletonInstance(struct _singleton_t* singleton)
{
  return singleton;
}

int singletonValue(struct _singleton_t* singleton)
{
  return singleton->value_;
}

singleton_t* createSingleton(int value)
{
  if (g_singleton == NULL)
  {
    g_singleton = malloc(sizeof(singleton_t));
    memset(g_singleton, 0, sizeof(singleton_t));
    g_singleton->Instance = singletonInstance;
    g_singleton->Value = singletonValue;
    g_singleton->value_ = value;
  }

  return g_singleton;
}

int main()
{
  printf("If you see the same value, then singleton was reused (yay!\n");
  
  singleton_t* singleton1 = createSingleton(10);
  singleton_t* singleton2 = createSingleton(20);

  printf("1: %d\n", singleton1->Value(singleton1));
  printf("2: %d\n", singleton2->Value(singleton2));

  return 0;
}
