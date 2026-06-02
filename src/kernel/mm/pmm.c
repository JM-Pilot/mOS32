#include <stdint.h>
#include "pmm.h"
#include <boot/multiboot.h>
#include <libk/stdio.h>
#include <libk/string.h>
uint32_t total_memory; 
uint32_t total_pages;
uint8_t *bitmap;

extern char _kernel_end;
extern char _kernel_start;

void pmm_set_used(uint32_t addr){
	uint32_t page = addr / 4096;
	bitmap[page / 8] |= (1 << (page % 8));
}

void pmm_free(uint32_t addr) {
	uint32_t page = addr / 4096;
	bitmap[page / 8] &= ~(1 << (page % 8));
}

void pmm_alloc_region(uint32_t addr, uint32_t len){
	uint32_t pages = len / 4096;
	for (uint32_t i = 0; i < pages; i++){
		pmm_set_used(addr + i * 4096);
	}
}
void pmm_init(multiboot_info_t *mb_info){
	/* gotta love the 80 char limit */
	multiboot_memory_map_t *mmap = (multiboot_memory_map_t*)
					mb_info->mmap_addr;

	multiboot_memory_map_t *mmap_end = (multiboot_memory_map_t*) 
			((uint32_t)mb_info->mmap_addr + mb_info->mmap_length);

	/* Get size of total_memory*/
	while (mmap < mmap_end){
		if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE)
			total_memory += mmap->len;
		mmap = (multiboot_memory_map_t *) ((uint32_t)mmap + 
					       mmap->size + sizeof(mmap->size));
	}
	total_pages = total_memory / 4096;
	bitmap = (uint8_t*)&_kernel_end;
	k_memset(bitmap, 0xFF, total_pages / 8);


	mmap = (multiboot_memory_map_t*)mb_info->mmap_addr;
	while (mmap < mmap_end){
		if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE){
			for (uint32_t i = 0; i < mmap->len / 4096; i++)
    				pmm_free(mmap->addr + i * 4096);
		}
		mmap = (multiboot_memory_map_t*)((uint32_t)mmap + 
			mmap->size + sizeof(mmap->size));
	}
	/* Ensure that the kernel region is protected */
	pmm_alloc_region(0, 0x100000);
	pmm_alloc_region((uint32_t)&_kernel_start, 
		(uint32_t)&_kernel_end - (uint32_t)&_kernel_start);
	pmm_alloc_region((uint32_t)bitmap, total_pages / 8);
}

uint32_t pmm_alloc(){
	for (uint32_t i = 0; i < total_pages; i++){
		if (!(bitmap[i / 8] & (1 << (i % 8)))){
			bitmap[i / 8] |= (1 << (i % 8));
			return i * 4096;
		}
	}
	return 0;
}

uint32_t get_total_mem(){
	return total_memory;
}