/* MIT LICENSE (C) 2026 JM-Pilot */
#include <boot/multiboot.h>
#ifndef KERNEL_H
#define KERNEL_H

void hcf();
void kernel_init(multiboot_info_t *mb_info);
#endif