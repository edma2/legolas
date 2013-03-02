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

static long file_size(FILE *fp);
static size_t emit(void *data, long offset, size_t size);

static FILE *out;

int main(int argc, char *argv[]) {
  ElfObject elf;
  FILE *in;
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

/*
 * File layout:
 * 0-51:    elf header
 * 52-115:  program header table
 * 128-139: .text
 */
void elf_dump(ElfObject *elf) {
  ElfHeader eh;
  PrgHeader ph;
  SecHeader *sh;
  uint8_t *text;

  sh = elf_find_sh(elf, ".text");
  text = (void *)elf->header + sh->sh_offset;

  ph.p_type   = PT_LOAD;
  ph.p_offset = 0x0;        // why not sh->sh_offset?
  ph.p_vaddr  = 0x8048000;  // not sure what this should be
  ph.p_paddr  = ph.p_vaddr;
  ph.p_filesz = sh->sh_offset + sh->sh_size;
  ph.p_memsz  = ph.p_filesz;
  ph.p_flags  = PF_X | PF_R;
  ph.p_align  = 0x1000;

  eh = *(elf->header); // make a copy
  eh.e_type = ET_EXEC;
  eh.e_entry = ph.p_vaddr + sh->sh_offset;
  eh.e_phoff = eh.e_ehsize; // ph should come right after
  eh.e_phnum = 1; // just .text for now
  eh.e_phentsize = sizeof(ph);
  eh.e_shoff = 0;
  eh.e_shnum = 0;
  eh.e_shentsize = 0;
  eh.e_shstrndx = 0;

  emit(&eh, 0, sizeof(eh));
  emit(&ph, eh.e_phoff, sizeof(ph));
  emit(text, sh->sh_offset, sh->sh_size);
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
