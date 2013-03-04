#include <list.h>
#include <object.h>
#include <storage.h>

int main(void) {
  ElfObject elf;
  FILE *in;

  in = fopen("example.o", "r+");
  ElfObject_init(in, &elf);
  fclose(in);

  List_test();
  ElfObject_test(&elf);
  storage_test(&elf);

  ElfObject_free(&elf);

  return 0;
}
