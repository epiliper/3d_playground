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

#define DynArrayInit(dest, init_cap, type)                                     \
  _DynArrayInit(dest, init_cap, sizeof(type))

#define DynArrayGet(dest, init_cap, single_ptr)                                \
  _DynArrayGet(dest, init_cap, (void **)single_ptr)

void _DynArrayInit(DynArray *d, int cap, int stride);
void DynArrayGrow(DynArray *d);
void DynArrayAdd(DynArray *d, void *item);
void _DynArrayGet(DynArray(*d), int idx, void **dest);
void DynArraySwapRemove(DynArray *d, int idx, void *dest);
void DynArrayDestroy(DynArray *d);
void DynArrayTest();

#endif // DYNARRAY.H
