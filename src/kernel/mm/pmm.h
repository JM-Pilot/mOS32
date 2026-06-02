#include <stdint.h>
#include <boot/multiboot.h>
#ifndef PMM_H
#define PMM_H

#define BLOCK_SIZE 512
extern uint32_t total_memory; 
void pmm_init(multiboot_info_t *mb_info);
uint32_t pmm_alloc();
void pmm_free(uint32_t addr);
uint32_t get_total_mem();
#endif