#include "elfobject.h"

int main(int argc, char *argv[]) {
  ElfObject elf;
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

  retval = elf_init(in, &elf);
  fclose(in);

  if (retval < 0) {
    elf_free(&elf);
    printf("elf_init failed\n");
    return -1;
  }

  elf_free(&elf);

  return 0;
}
