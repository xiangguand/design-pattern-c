#include <stdio.h>
#include "encapsulation.h"

// Priavate
typedef struct _info_t
{
  int a;
  int b;
} info_t;

static int encapsulationGet(struct _encapsulation_t* encap)
{
  return ((info_t*)encap->info)->a + (++((info_t*)encap->info)->b);
}

static int encapsulationSet(struct _encapsulation_t* encap, int val)
{
  ((info_t*)encap->info)->a = val;
}

//! constructor
encapsulation_t* createEncapsulation(void)
{
  encapsulation_t* encap = malloc(sizeof(encapsulation_t));
  encap->get = encapsulationGet;
  encap->set = encapsulationSet;
  encap->info = malloc(sizeof(info_t));
  ((info_t*)encap->info)->a = 0;
  ((info_t*)encap->info)->b = 0;

  return encap;
}
