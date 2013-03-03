#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

typedef Elf32_Shdr SecHeader;
typedef Elf32_Phdr PrgHeader;
typedef Elf32_Ehdr ElfHeader;

/* Represents an ELF relocatable object file. */
typedef struct {
  /* Offset 0 of file image. */
  ElfHeader *header;

  /* Size of the file in bytes */
  long size;

  /* Array of section headers. */
  SecHeader *sh_table;

  /* Section header string table. */
  char *sh_names;
} ElfObject;

int         elf_init(FILE *fp, ElfObject *elf);
SecHeader * elf_find_sh(ElfObject *elf, const char *name);
int         elf_free(ElfObject *elf);
void        elf_dump(ElfObject *elf);
