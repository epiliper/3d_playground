#ifndef G_DYNARRAY
#define G_DYNARRAY

#include <stdint.h>

// destructor types for items?
typedef struct {
  char *data;
  uint64_t cap;
  uint64_t len;
  uint64_t stride;
} DynArray;

#define DYNARRAY_RESIZE_FACTOR 2

#define DynArrayInit(init_cap, type) _DynArrayInit(init_cap, sizeof(type))

#define DynArrayGet(arr, idx) _DynArrayGet(arr, idx)

DynArray _DynArrayInit(int cap, int stride);
void DynArrayGrow(DynArray *d);
DynArray DynArrayClone(DynArray *src);
void DynArrayClear(DynArray *d);
void DynArrayAdd(DynArray *d, void *item);
void *_DynArrayGet(DynArray *d, int idx);
void DynArraySwapRemove(DynArray *d, int idx, void *dest);
void DynArrayDestroy(DynArray *d);
void DynArrayTest();

#endif // DYNARRAY.H
