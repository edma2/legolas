#include "object.h"

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
int obj_init(FILE *fp, Object *obj) {
  int fd;

  obj->size = file_size(fp);
  if (obj->size < 0) {
    return -1;
  }

  fd = fileno(fp);
  if (fd < 0) {
    return -1;
  }

  obj->header = mmap(NULL, obj->size, PROT_READ, MAP_SHARED, fd, 0);
  if (obj->header == MAP_FAILED) {
    return -1;
  }

  obj->sh_table = (void *)obj->header + obj->header->e_shoff;
  obj->sh_names = (void *)obj->header +
    obj->sh_table[obj->header->e_shstrndx].sh_offset;

  return 0;
}

/* Unmap file image. */
int obj_free(Object *obj) {
  return munmap(obj->header, obj->size);
}
