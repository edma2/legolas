#include <list.h>
#include <object.h>
#include <storage.h>

/* Where each section will go relative to each other. */
List *storage_layout(ElfObject *elf) {
  List *sections = NULL;
  SectionHeader *text, *data;

  text = ElfObject_sh_by_name(elf, ".text");
  data = ElfObject_sh_by_name(elf, ".data");
  if (text == NULL || data == NULL)
    goto error;

  if (List_append(&sections, text) == NULL) {
    goto error;
  }

  if (List_append(&sections, data) == NULL) {
    goto error;
  }

  return sections;

error:
  List_free(&sections, NULL);
  return NULL;
}
