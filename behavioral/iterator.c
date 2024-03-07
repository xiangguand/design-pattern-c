#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define AGGREGATE_MEMBERS(TYPE) TYPE (*createIterator)(void);
#define ITERATOR_MEMBERS(OBJ)                                                                                          \
  void* (*currentItem)(OBJ);                                                                                           \
  void (*first)(OBJ);                                                                                                  \
  bool (*isDone)(OBJ);                                                                                                 \
  void (*next)(OBJ)
#define ITERATOR_MEMBERS_WITH_TYPE(OBJ, RETTYPE)                                                                       \
  RETTYPE (*currentItem)(OBJ);                                                                                         \
  void (*first)(OBJ);                                                                                                  \
  bool (*isDone)(OBJ);                                                                                                 \
  void (*next)(OBJ)

typedef struct _iterator_t
{
  ITERATOR_MEMBERS(void*);
} iterator_t;

typedef struct _num_t
{
  int val;
} num_t;

typedef struct _concreteIterator_t concreteIterator_t;
typedef struct _nums_t
{
  AGGREGATE_MEMBERS(concreteIterator_t*);
  void (*add)(struct _nums_t* nums, num_t num);
  int it_count;
  num_t* buf;
  int count;
} nums_t;

typedef struct _concreteIterator_t
{
  ITERATOR_MEMBERS_WITH_TYPE(nums_t*, num_t*);
} concreteIterator_t;

//! implement iterator
num_t* concreteCurrentItem(nums_t* nums)
{
  if (nums->count > nums->it_count)
    return nums->buf + nums->it_count;
  else
    return NULL;
}

void concreteFirst(nums_t* nums)
{
  nums->it_count = 0;
}

bool concreteIsDone(nums_t* nums)
{
  bool ret = false;
  if (nums->count <= nums->it_count)
  {
    ret = true;
  }

  return ret;
}

void concreteNext(nums_t* nums)
{
  if (nums->count > nums->it_count)
  {
    nums->it_count++;
  }
}

concreteIterator_t* createConcreteIterator(void)
{
  concreteIterator_t* iterator = (concreteIterator_t*)malloc(sizeof(concreteIterator_t));
  memset(iterator, 0, sizeof(concreteIterator_t));
  iterator->first = concreteFirst;
  iterator->isDone = concreteIsDone;
  iterator->next = concreteNext;
  iterator->currentItem = concreteCurrentItem;
}

//! Nums member functions

void numsAdd(struct _nums_t* nums, num_t num)
{
  nums->buf[nums->count++].val = num.val;
}

nums_t* createNums(void)
{
  nums_t* nums = (nums_t*)malloc(sizeof(nums_t));
  memset(nums, 0, sizeof(nums_t));
  nums->createIterator = createConcreteIterator;
  nums->add = numsAdd;
  nums->buf = malloc(100 * sizeof(num_t));
}

int main()
{
  nums_t* nums = createNums();
  num_t num;
  for (int i = 0; i < 10; i++)
  {
    num.val = i;
    nums->add(nums, num);
  }

  concreteIterator_t* it = nums->createIterator();
  for (it->first(nums); !it->isDone(nums); it->next(nums))
  {
    printf("Get: %d\n", it->currentItem(nums)->val);
  }

  return 0;
}
