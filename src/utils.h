#ifndef G_UTILS
#define G_UTILS
/// Read a file's contents to end and store in a freshly-allocated string.
const char *readFileToEnd(const char *path, int *len);
#endif
