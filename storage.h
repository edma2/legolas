#ifndef STORAGE_H
#define STORAGE_H

#include <object.h>
#include <list.h>

List *sections_to_load(ElfObject *elf);
void storage_test(ElfObject *elf);

#endif
