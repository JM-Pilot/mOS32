/* MIT LICENSE (C) 2026 JM-Pilot */
#include <boot/multiboot.h>
#include "mm/heap.h"
#ifndef KERNEL_H
#define KERNEL_H

extern heap_bm_t k_heap;
void hcf();
void kernel_init(multiboot_info_t *mb_info);
#endif