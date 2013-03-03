#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

typedef Elf32_Shdr SecHeader;
typedef Elf32_Ehdr ElfHeader;

/* An object file. */
typedef struct {
  /* Offset 0 of file image. */
  ElfHeader *header;

  /* Size of the file in bytes */
  long size;

  /* Array of section headers. */
  SecHeader *sh_table;

  /* Section header string table. */
  char *sh_names;
} Object;

int obj_init(FILE *fp, Object *obj);
int obj_free(Object *obj);
