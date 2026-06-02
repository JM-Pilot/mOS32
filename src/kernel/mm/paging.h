#include <stdint.h>
#ifndef PAGING_H
#define PAGING_H

extern void load_page_dir(uint32_t *dir);
extern void enable_paging();
void paging_init(uint32_t fb_addr);
void vmm_map_page(void *physaddr, void *virtualaddr, unsigned int flags);
#endif