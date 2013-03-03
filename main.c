#include <object.h>

int main(int argc, char *argv[]) {
  Object obj;
  FILE *in;
  int retval;

  if (argc < 2) {
    printf("usage: %s <input>\n", argv[0]);
    return -1;
  }

  in = fopen(argv[1], "r+");
  if (in == NULL) {
    printf("Failed to open input file\n");
    return -1;
  }

  retval = obj_init(in, &obj);
  fclose(in);

  if (retval < 0) {
    obj_free(&obj);
    printf("obj_init failed\n");
    return -1;
  }

  obj_free(&obj);

  return 0;
}
