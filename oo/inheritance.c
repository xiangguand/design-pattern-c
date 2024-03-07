#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define CAR_MEMBER(TYPE)                                                                                               \
  char* type;                                                                                                          \
  char* brand;                                                                                                         \
  char* (*getInfo)(TYPE)

#define CAR_INIT .type = "car type", .brand = "Telsla", .getInfo = carGetInfo

typedef struct _car_t
{
  CAR_MEMBER(struct _car_t*);
} car_t;

typedef struct _taxi_t
{
  CAR_MEMBER(struct _car_t*);
  char* driver_name;
} taxi_t;

char* carGetInfo(car_t* car)
{
  return car->type;
}

int main()
{
  car_t car = { CAR_INIT };
  taxi_t taxi = { CAR_INIT, .driver_name = "Danny" };

  printf("%s\n", car.getInfo(&car));
  printf("%s\n", taxi.getInfo((car_t *)&taxi));
}
