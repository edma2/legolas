#ifndef OBJECT_H
#define OBJECT_H

#include <elf.h>
#include <stdio.h>

typedef Elf32_Shdr SectionHeader;
typedef Elf32_Phdr ProgramHeader;
typedef Elf32_Ehdr ElfHeader;

typedef struct {
  uint32_t size;
  Elf32_Sym *sym_table;
  char *sym_names;
} Symbols;

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

  Symbols symbols;

} ElfObject;

int ElfObject_init(FILE *fp, ElfObject *elf);
int ElfObject_free(ElfObject *elf);
SectionHeader *ElfObject_sh(ElfObject *elf, const char *name);

void ElfObject_test(ElfObject *elf);

#endif
