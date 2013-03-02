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
} Elf;

int         Elf_init(FILE *fp, Elf *elf);
SecHeader * Elf_find_sh(Elf *elf, const char *name);
int         Elf_free(Elf *elf);
void        Elf_dump(Elf *elf);

static long file_size(FILE *fp) {
  long size;

  if (fseek(fp, 0, SEEK_END) < 0) {
    return -1;
  }
  size = ftell(fp); // could be negative
  rewind(fp);

  return size;
}

/* Initialize elf by memory mapping file contents.
 * You can safely close fp after calling this function. */
int Elf_init(FILE *fp, Elf *elf) {
  int fd;

  elf->size = file_size(fp);
  if (elf->size < 0) {
    return -1;
  }

  fd = fileno(fp);
  if (fd < 0) {
    return -1;
  }

  elf->header = mmap(NULL, elf->size, PROT_READ, MAP_SHARED, fd, 0);
  if (elf->header == MAP_FAILED) {
    return -1;
  }

  elf->sh_table = (void *)elf->header + elf->header->e_shoff;
  elf->sh_names = (void *)elf->header +
    elf->sh_table[elf->header->e_shstrndx].sh_offset;

  return 0;
}

/* Find section header by name, or NULL if didn't find it. */
SecHeader *Elf_find_sh(Elf *elf, const char *name) {
  int i;
  SecHeader *sh;

  for (i = 0; i < elf->header->e_shnum; i++) {
    sh = &(elf->sh_table[i]);
    char *found = elf->sh_names + sh->sh_name;
    if (strcmp(found, name) == 0) {
      return sh;
    }
  }

  return NULL;
}

int Elf_free(Elf *elf) {
  return munmap(elf->header, elf->size);
}

void Elf_dump(Elf *elf) {
  int i;
  SecHeader *sh;

  printf("%d bytes\n", elf->size);
  sh = Elf_find_sh(elf, ".text");
  if (sh != NULL) {
    printf(".text offset: 0x%x\n", sh->sh_offset);
  }
}

int main(int argc, char *argv[]) {
  Elf elf;
  FILE *fp;

  fp = fopen("/tmp/test", "r+");
  if (fp == NULL) {
    printf("fopen failed\n");
    return -1;
  }

  if (Elf_init(fp, &elf) < 0) {
    printf("init failed\n");
    fclose(fp);
    return -1;
  }
  fclose(fp);

  Elf_dump(&elf);
  Elf_free(&elf);

  return 0;
}
