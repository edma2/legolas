#include <object.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <assert.h>
#include <string.h>

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

SectionHeader *ElfObject_sh(ElfObject *elf, const char *name) {
  int i;
  SectionHeader *sh;

  for (i = 0; i < elf->header->e_shnum; i++) {
    sh = &(elf->sh_table[i]);
    char *found = elf->sh_names + sh->sh_name;
    if (strcmp(found, name) == 0) {
      return sh;
    }
  }

  return NULL;
}

/* Initialize object file by memory mapping file contents.
 * You can safely close fp after calling this function. */
int ElfObject_init(FILE *fp, ElfObject *elf) {
  int fd;
  SectionHeader *sh;

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

  sh = ElfObject_sh(elf, ".symtab");
  elf->symbols.size = sh->sh_size / sh->sh_entsize;
  elf->symbols.sym_table = (void *)elf->header + sh->sh_offset;

  sh = ElfObject_sh(elf, ".strtab");
  elf->symbols.sym_names = (void *)elf->header + sh->sh_offset;

  return 0;
}

/* Unmap file image. */
int ElfObject_free(ElfObject *elf) {
  return munmap(elf->header, elf->size);
}

static void Symbols_print(Symbols *symbols) {
  int i;
  Elf32_Sym *sym;

  for (i = 0; i < symbols->size; i++) {
    sym = &(symbols->sym_table[i]);
    char *name = symbols->sym_names + sym->st_name;
    printf("%s\n", name);
  }
}

void ElfObject_test(ElfObject *elf) {
  SectionHeader *sh;

  unsigned char *magic = elf->header->e_ident;
  assert(magic[0] == 0x7f);
  assert(magic[1] == 'E');
  assert(magic[2] == 'L');
  assert(magic[3] == 'F');

  sh = ElfObject_sh(elf, ".text");
  assert(strcmp(elf->sh_names + sh->sh_name, ".text") == 0);

  printf("listing symbols...\n");
  Symbols_print(&(elf->symbols));
  printf("%s: All tests pass.\n", __FILE__);
}
