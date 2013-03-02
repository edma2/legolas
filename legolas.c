#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

/* Represents an ELF relocatable object file. */
typedef struct {
  /* Offset 0 of file image. */
  Elf32_Ehdr *header;

  /* Size of the file in bytes */
  long size;

  /* Array of section headers. */
  Elf32_Shdr *section_headers;

  /* Section header string table. */
  char *section_header_names;
} Elf;

static long file_size(FILE *fp) {
  long size;

  if (fseek(fp, 0, SEEK_END) < 0) {
    return -1;
  }
  size = ftell(fp); // could be negative
  rewind(fp);

  return size;
}

/* Initialize elf with file contents.
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

  elf->section_headers = (void *)elf->header + elf->header->e_shoff;
  elf->section_header_names = (void *)elf->header +
    elf->section_headers[elf->header->e_shstrndx].sh_offset;

  return 0;
}

Elf32_Shdr *Elf_section_header(Elf *elf, const char *name) {
  return NULL;
}

int Elf_free(Elf *elf) {
  return munmap(elf->header, elf->size);
}

void Elf_dump(Elf *elf) {
  int i;
  Elf32_Shdr *shdr;

  printf("%d bytes\n", elf->size);
  for (i = 0; i < elf->header->e_shnum; i++) {
    char *name = elf->section_header_names + elf->section_headers[i].sh_name;
    printf("%s\n", name);
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

  Elf_dump(&elf);

  fclose(fp);
  Elf_free(&elf);

  return 0;
}
