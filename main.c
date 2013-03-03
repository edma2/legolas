#include "elfobject.h"

int main(int argc, char *argv[]) {
  ElfObject elf;
  FILE *in;
  int retval;

  if (argc < 3) {
    printf("usage: %s <input> <output>\n", argv[0]);
    return -1;
  }

  in = fopen(argv[1], "r+");
  if (in == NULL) {
    printf("Failed to open input file\n");
    return -1;
  }

  out = fopen(argv[2], "w");
  if (out == NULL) {
    printf("Failed to open output file\n");
    fclose(in);
    return -1;
  }

  retval = elf_init(in, &elf);
  fclose(in);

  if (retval < 0) {
    printf("elf_init failed\n");
    fclose(out);
    return -1;
  }

  //elf_dump(&elf);

  elf_free(&elf);
  fclose(out);

  return 0;
}
