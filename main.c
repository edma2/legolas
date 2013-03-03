#include <object.h>

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

  retval = ElfObject_init(in, &elf);
  fclose(in);

  if (retval < 0) {
    ElfObject_free(&elf);
    printf("ElfObject_init failed\n");
    return -1;
  }

  printf("%s\n", elf.header->e_ident);

  ElfObject_free(&elf);

  return 0;
}
