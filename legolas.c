#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct {
  /* Offset 0 of file image. */
  Elf32_Ehdr *header;

  /* Size of the file in bytes */
  long size;
} ObjectFile;

static long file_size(FILE *fp) {
  long size;

  if (fseek(fp, 0, SEEK_END) < 0) {
    return -1;
  }
  size = ftell(fp); // could be negative
  rewind(fp);

  return size;
}

/* Return -1 on failure and set error string. */
int ObjectFile_init(FILE *fp, ObjectFile *elf) {
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

int main(int argc, char *argv[]) {
  ObjectFile elf;
  FILE *fp;

  fp = fopen("/tmp/test", "r+");
  if (fp == NULL) {
    printf("fopen failed\n");
    return -1;
  }

  if (ObjectFile_init(fp, &elf) < 0) {
    printf("init failed\n");
    fclose(fp);
    return -1;
  }

  fclose(fp);

  return 0;
}
