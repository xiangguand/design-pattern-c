#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define COMPONENT_MEMBERS(SELF, TYPE)                                                                                  \
  void (*Add)(SELF, TYPE);                                                                                             \
  void (*Remove)(SELF, TYPE);                                                                                          \
  int (*Operation)(SELF)

typedef struct _component_t
{
  COMPONENT_MEMBERS(struct _component_t*, struct _component_t*);
} component_t;

typedef struct _leaf
{
  COMPONENT_MEMBERS(struct _leaf*, struct _component_t*);
} leaf_t;

typedef struct _composite_t
{
  COMPONENT_MEMBERS(struct _composite_t*, struct _component_t*);
  int count;
  component_t* leaves[10];
} composite_t;

int leaf_operation(leaf_t* leaf)
{
  (void)leaf;
  printf("Leaf\n");
  return 1;
}

void compositeAdd(composite_t* composite, component_t* component)
{
  composite->leaves[composite->count++] = component;
}
void compositeRemove(composite_t* composite, component_t* component)
{
  composite->count--;
}

int compositeOperation(composite_t* composite)
{
  int value = 0;
  for (int i = 0; i < composite->count; i++)
  {
    value += composite->leaves[i]->Operation(composite->leaves[i]);
  }

  return value * 10;
}

void ClientCode(component_t* component)
{
  int result = component->Operation(component);
  printf("result: %d\n", result);
}

int main()
{
  composite_t tree = {
    .Add = compositeAdd,
    .Remove = compositeRemove,
    .Operation = compositeOperation,
    .count = 0,
  };
  composite_t branch1 = {
    .Add = compositeAdd,
    .Remove = compositeRemove,
    .Operation = compositeOperation,
    .count = 0,
  };
  composite_t branch2 = {
    .Add = compositeAdd,
    .Remove = compositeRemove,
    .Operation = compositeOperation,
    .count = 0,
  };
  leaf_t leaf1 = { .Add = NULL, .Operation = leaf_operation, .Remove = NULL };
  leaf_t leaf2 = { .Add = NULL, .Operation = leaf_operation, .Remove = NULL };
  leaf_t leaf3 = { .Add = NULL, .Operation = leaf_operation, .Remove = NULL };

  branch1.Add(&branch1, (component_t *)&leaf1);
  branch1.Add(&branch1, (component_t *)&leaf2);

  branch2.Add(&branch2, (component_t *)&leaf2);

  branch2.Add(&branch2, (component_t *)&branch1);

  tree.Add(&tree, (component_t *)&branch2);

  tree.Add(&tree, (component_t *)&leaf1);
  tree.Add(&tree, (component_t *)&leaf2);
  tree.Add(&tree, (component_t *)&leaf3);

  ClientCode((component_t*)&tree);
  printf("\n===== End of program =====\n");

  return 0;
}
