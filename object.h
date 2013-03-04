#include <elf.h>
#include <stdio.h>

typedef Elf32_Shdr SectionHeader;
typedef Elf32_Ehdr ElfHeader;

/* An object file. */
typedef struct {
  /* Offset 0 of file image. */
  ElfHeader *header;

  /* Size of the file in bytes */
  long size;

  /* Array of section headers. */
  SectionHeader *sh_table;

  /* Section header string table. */
  char *sh_names;
} ElfObject;

int ElfObject_init(FILE *fp, ElfObject *elf);
int ElfObject_free(ElfObject *elf);
