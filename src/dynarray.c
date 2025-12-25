#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dynarray.h"

// initialize to a particular item size and capacity
DynArray _DynArrayInit(int init_cap, int stride) {
  DynArray ret;
  ret.data = malloc(init_cap * stride);
  ret.len = 0;
  ret.cap = init_cap;
  ret.stride = stride;
  return ret;
}

DynArray DynArrayClone(DynArray *src) {
  DynArray ret = _DynArrayInit(src->cap, src->stride);
  memcpy(ret.data, src->data, src->stride * src->len);
  ret.len = src->len;
  return ret;
}

// grow to accomodate more elements.
void DynArrayGrow(DynArray *d) {
  d->cap = d->cap == 0 ? 1 : d->cap;
  d->data = realloc(d->data, d->cap * DYNARRAY_RESIZE_FACTOR * d->stride);
  d->cap *= DYNARRAY_RESIZE_FACTOR;
}

// add an element to array via memcpy and
void DynArrayAdd(DynArray *d, void *item) {
  if (d->len >= d->cap) {
    DynArrayGrow(d);
    // printf("Growing dynarray from length %lu\n", d->len);
  }

  memcpy(&d->data[d->len * d->stride], item, d->stride);
  d->len++;
}

void *_DynArrayGet(DynArray *d, int idx) {
  if (idx >= d->len) {
    return NULL;
  }

  return &d->data[idx * d->stride];
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
  DynArray test = DynArrayInit(1, int);

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

  got = DynArrayGet(&test, 0);
  printf("Element at index 0: %d\n", *got);
  got = DynArrayGet(&test, 1);
  printf("Element at index 1: %d\n", *got);
}
