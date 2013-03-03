#include "elfobject.h"

static size_t emit(void *data, long offset, size_t size);
static long file_size(FILE *fp);

FILE *out;

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

static size_t emit(void *data, long offset, size_t size) {
  if (fseek(out, offset, SEEK_SET) < 0) {
    return -1;
  }
  return fwrite(data, size, 1, out);
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
