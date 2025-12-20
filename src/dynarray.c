#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dynarray.h"

// initialize to a particular item size and capacity
void _DynArrayInit(DynArray *dest, int init_cap, int stride) {
  dest->data = malloc(init_cap * stride);
  dest->len = 0;
  dest->cap = init_cap;
  dest->stride = stride;
}

// grow to accomodate more elements.
void DynArrayGrow(DynArray *d) {
  d->data = realloc(d->data, d->cap * DYNARRAY_RESIZE_FACTOR * d->stride);
  d->cap *= DYNARRAY_RESIZE_FACTOR;
}

// add an element to array via memcpy and
void DynArrayAdd(DynArray *d, void *item) {
  if (d->len >= d->cap) {
    DynArrayGrow(d);
  }

  memcpy(&d->data[d->len * d->stride], item, d->stride);
  d->len++;
}

void _DynArrayGet(DynArray *d, int idx, void **dest) {
  if (idx >= d->len) {
    *dest = NULL;
    return;
  }

  *dest = &d->data[idx * d->stride];
}

// replace the element at idx with the one at the very end.
// O(1) time complexity.
void DynArraySwapRemove(DynArray *d, int idx, void *dest) {
  if (idx > d->len) {
    return;
  }

  if (d->len == 1) {
    d->len = 0;
    return;
  }

  if (dest != NULL) {
    memcpy(dest, &d->data[idx], d->stride);
  }

  memcpy(&d->data[idx * d->stride], &d->data[(d->len - 1) * d->stride],
         d->stride);
  d->len--;
}

void DynArrayClear(DynArray *d) { d->len = 0; }

void DynArrayDestroy(DynArray *d) {
  // TODO: store destructor func for items that need them.
  free(d->data);
}

void DynArrayTest() {
  DynArray test;
  DynArrayInit(&test, 1, int);

  int element = 1;
  DynArrayAdd(&test, &element);
  DynArrayAdd(&test, &element);
  element = 2;
  DynArrayAdd(&test, &element);

  int *data = (int *)test.data;
  for (int i = 0; i < test.len; i++) {
    printf("element %d \n", data[i]);
  }

  DynArraySwapRemove(&test, 1, NULL);

  printf("Array after swap-remove\n");

  data = (int *)test.data;
  for (int i = 0; i < test.len; i++) {
    printf("element %d \n", data[i]);
  }

  int *got;

  DynArrayGet(&test, 0, &got);
  printf("Element at index 0: %d\n", *got);
  DynArrayGet(&test, 1, &got);
  printf("Element at index 1: %d\n", *got);
}
