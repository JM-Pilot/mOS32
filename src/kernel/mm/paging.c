#include "paging.h"
#include <stdint.h>
#include <libk/stdio.h>
#include "pmm.h"
uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));
uint32_t fb_page_table[1024] __attribute__((aligned(4096)));
void paging_init(uint32_t fb_addr){
	int i;
	uint32_t fb_page = fb_addr / 4096;
    	uint32_t pd_index = fb_addr >> 22;
	for(i = 0; i < 1024; i++) {
		page_directory[i] = 2;
		first_page_table[i] = (i * 0x1000) | 3;
		fb_page_table[i] = ((fb_page + i) * 0x1000) | 3;
	}

	page_directory[0] = (uint32_t)first_page_table | 3;
	page_directory[1023] = (uint32_t)page_directory | 3; 
	page_directory[pd_index] = (uint32_t)fb_page_table | 3;
	load_page_dir(page_directory);
	enable_paging();
}

void vmm_map_page(void *physaddr, void *virtualaddr, unsigned int flags) {
   	uint32_t pdindex = (uint32_t)virtualaddr >> 22;
    	uint32_t ptindex = (uint32_t)virtualaddr >> 12 & 0x03FF;
    	uint32_t *pd = (uint32_t*)0xFFFFF000;

	if (!(pd[pdindex] & 1)) {
		uint32_t new_pt = pmm_alloc();
		pd[pdindex] = new_pt | 3;
	}

    	uint32_t *pt = ((uint32_t*)0xFFC00000) + (0x400 * pdindex);
    	pt[ptindex] = ((uint32_t)physaddr) | (flags & 0xFFF) | 0x01; 

	asm volatile("invlpg (%0)" ::"r" (virtualaddr) : "memory");
}