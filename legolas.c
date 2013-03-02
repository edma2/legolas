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

  return 0;
}

Elf32_Shdr *Elf_section_header(Elf *elf, const char *name) {
  return NULL;
}

int Elf_free(Elf *elf) {
  return munmap(elf->header, elf->size);
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

  printf("%d\n", elf.size);

  fclose(fp);
  Elf_free(&elf);

  return 0;
}
