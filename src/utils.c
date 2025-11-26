#include <stdio.h>
#include <stdlib.h>

const char *readFileToEnd(const char *path, int *n) {
  FILE *f = fopen(path, "r");

  fseek(f, 0, SEEK_END);
  *n = ftell(f);
  rewind(f);

  char *ret = malloc(*n * sizeof(char *));
  int read = fread(ret, *n, 1, f);
  if (read != *n) {
    printf("file partial read: %s", path);
  }

  fclose(f);
  return ret;
}
