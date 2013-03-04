#include <storage.h>
#include <assert.h>

/* Sections which should appear in a loadable segment.
 * Section data would be loaded in this order. */
List *sections_to_load(ElfObject *elf) {
  List *sh_list = NULL;
  SectionHeader *sh;

  int i;
  for (i = 0; i < elf->header->e_shnum; i++) {
    sh = &(elf->sh_table[i]);
    if ((sh->sh_flags & SHF_ALLOC) != 0) {
      if (List_append(&sh_list, sh) == NULL) {
        List_free(&sh_list, NULL);
        return NULL;
      }
    }
  }

  return sh_list;
}

void storage_test(ElfObject *elf) {
  SectionHeader *sh;
  List *head = sections_to_load(elf);
  List *lp = head;

  sh = lp->data;
  assert(ElfObject_sh(elf, ".data") == sh);
  lp = lp->next;

  sh = lp->data;
  assert(ElfObject_sh(elf, ".text") == sh);
  assert(lp->next == NULL);

  List_free(&head, NULL);
}
