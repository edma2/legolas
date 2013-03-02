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
} ElfObject;

int         elf_init(FILE *fp, ElfObject *elf);
SecHeader * elf_find_sh(ElfObject *elf, const char *name);
int         elf_free(ElfObject *elf);
void        elf_dump(ElfObject *elf);

static long file_size(FILE *fp);

int main(int argc, char *argv[]) {
  ElfObject elf;
  FILE *in, *out;
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

  elf_dump(&elf);

  elf_free(&elf);
  fclose(out);

  return 0;
}

/* Initialize elf by memory mapping file contents.
 * You can safely close fp after calling this function. */
int elf_init(FILE *fp, ElfObject *elf) {
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
SecHeader *elf_find_sh(ElfObject *elf, const char *name) {
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

int elf_free(ElfObject *elf) {
  return munmap(elf->header, elf->size);
}

void elf_dump(ElfObject *elf) {
  SecHeader *sh;

  printf("%ld bytes\n", elf->size);
  sh = elf_find_sh(elf, ".text");
  if (sh != NULL) {
    printf(".text offset: 0x%x\n", sh->sh_offset);
  }

  printf("entry: 0x%x\n", elf->header->e_entry);
}

static long file_size(FILE *fp) {
  long size;

  if (fseek(fp, 0, SEEK_END) < 0) {
    return -1;
  }
  size = ftell(fp); // could be negative
  rewind(fp);

  return size;
}
