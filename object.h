#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

typedef Elf32_Shdr SectionHdr;
typedef Elf32_Ehdr ElfHdr;

/* An object file. */
typedef struct {
  /* Offset 0 of file image. */
  ElfHdr *header;

  /* Size of the file in bytes */
  long size;

  /* Array of section headers. */
  SectionHdr *sh_table;

  /* Section header string table. */
  char *sh_names;
} Object;

int obj_init(FILE *fp, Object *obj);
int obj_free(Object *obj);
