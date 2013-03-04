#include <object.h>
#include <stdlib.h>
#include <sys/mman.h>

/* Get file size given file pointer. */
static long file_size(FILE *fp) {
  long size;

  if (fseek(fp, 0, SEEK_END) < 0) {
    return -1;
  }
  size = ftell(fp); // could be negative
  rewind(fp);

  return size;
}

/* Initialize object file by memory mapping file contents.
 * You can safely close fp after calling this function. */
int ElfObject_init(FILE *fp, ElfObject *elf) {
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

/* Unmap file image. */
int ElfObject_free(ElfObject *elf) {
  return munmap(elf->header, elf->size);
}
