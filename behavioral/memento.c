#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMENTO_MENBER(TYPE) const char* (*getState)(TYPE)

typedef struct _memento_t
{
  MEMENTO_MENBER(struct _memento_t*);
} memento_t;

typedef struct _concreteMemento_t
{
  MEMENTO_MENBER(struct _concreteMemento_t*);
  char* state;
} concreteMemento_t;

const char* mementoGetState(concreteMemento_t* mem)
{
  return mem->state;
}

const char* genRandomString()
{
  int length = 10;
  char* random_string = malloc((length + 1) * sizeof(char));
  random_string[length] = '\0';

  for (int i = 0; i < length; i++)
  {
    if (rand() & 1)
    {
      random_string[i] = (char)(rand() % 10) + '0';
    }
    else
    {
      random_string[i] = (char)(rand() % 26) + 'a';
    }
  }

  return random_string;
}

concreteMemento_t* createMemento(const char* state)
{
  int length = strlen(state);
  concreteMemento_t* mem = malloc(sizeof(concreteMemento_t));
  mem->state = malloc((length + 1) * sizeof(char));
  memset(mem->state, 0, (length + 1) * sizeof(char));
  memcpy(mem->state, state, length * sizeof(char));
  mem->getState = mementoGetState;

  return mem;
}

typedef struct _originator_t
{
  MEMENTO_MENBER(concreteMemento_t*);
  char* state;
  concreteMemento_t* (*save)(struct _originator_t*);
  void (*doSomthingImportant)(struct _originator_t*);
} originator_t;

typedef struct _careTaker_t
{
  concreteMemento_t* mementos[10];
  int count;
  originator_t* originator;
  void (*printAllMementos)(struct _careTaker_t*);
  void (*backup)(struct _careTaker_t*);
  void (*undo)(struct _careTaker_t*);
} careTaker_t;

concreteMemento_t* oringinatorSave(struct _originator_t* originator)
{
  return createMemento(originator->state);
}

void oringinatorDoSomthingImportant(struct _originator_t* originator)
{
  // change state
  originator->state = (char*)genRandomString();
  printf("Do %s\n", originator->state);
}

void careTakerPrintAllState(struct _careTaker_t* careTaker)
{
  for (int i = 0; i < careTaker->count; i++)
  {
    printf("state [%2d]: %s\n", i, careTaker->mementos[i]->getState(careTaker->mementos[i]));
  }
}

void careTakerBackup(struct _careTaker_t* careTaker)
{
  careTaker->mementos[careTaker->count++] = careTaker->originator->save(careTaker->originator);
}

void careTakerUndo(struct _careTaker_t* careTaker)
{
  if (careTaker->count > 0)
  {
    careTaker->count--;
    free(careTaker->mementos[careTaker->count]);
  }
}

//! construtor
originator_t* createOriginator()
{
  originator_t* originator = malloc(sizeof(originator_t));
  memset(originator, 0, sizeof(originator_t));
  originator->doSomthingImportant = oringinatorDoSomthingImportant;
  originator->getState = mementoGetState;
  originator->save = oringinatorSave;

  return originator;
}

careTaker_t* createCareTaker(originator_t* originator)
{
  careTaker_t* careTaker = malloc(sizeof(careTaker_t));
  memset(careTaker, 0, sizeof(careTaker_t));
  careTaker->originator = originator;
  careTaker->backup = careTakerBackup;
  careTaker->printAllMementos = careTakerPrintAllState;
  careTaker->undo = careTakerUndo;

  return careTaker;
}

int main()
{
  originator_t* originator = createOriginator();
  careTaker_t* careTaker = createCareTaker(originator);

  originator->doSomthingImportant(originator);
  careTaker->backup(careTaker);
  originator->doSomthingImportant(originator);
  careTaker->backup(careTaker);
  originator->doSomthingImportant(originator);
  careTaker->backup(careTaker);

  printf("===== Show =====\n");
  careTaker->printAllMementos(careTaker);

  printf("\n===== Undo =====\n");
  careTaker->undo(careTaker);
  careTaker->printAllMementos(careTaker);

  printf("\n===== Undo =====\n");
  careTaker->undo(careTaker);
  careTaker->printAllMementos(careTaker);

  return 0;
}
