#include <stdint.h>
#include "fat32.h"
#include "../drivers/disk/ata.h"
#include <libk/stdio.h>
#include <libk/stdlib.h>

struct fat32_info fat_info;
void fat32_init(){
	uint8_t *buf = kmalloc(512);
	ata_read(0, 1, 0, (uint16_t*)buf);
	struct fat32_bpbwe *bpbwe = (struct fat32_bpbwe *)buf;
	struct fat32_bpb bpb = bpbwe->bpb;
	struct fat32_ebpb ebpb = bpbwe->ebpb;
	fat_info.bytes_per_sect = bpb.bytes_per_sect;
	fat_info.rsvd_sect_count = bpb.rsvd_sect_count;
	fat_info.root_clust = ebpb.root_cluster;
	fat_info.sects_per_clust = bpb.sect_per_clust;

	fat_info.fat_size = (bpb.total_sects_16 == 0)? ebpb.sect_per_fat_32 :
		bpb.sect_per_fat_16;

	fat_info.first_data_sect = bpb.rsvd_sect_count + (bpb.num_fats
		* fat_info.fat_size);

	kfree(buf);
}

uint32_t fat32_cluster_to_lba(uint32_t cluster){
	return fat_info.first_data_sect + (cluster - 2) * 
		fat_info.sects_per_clust;
}

struct fat32_dir_entry *fat32_read_dir(uint32_t lba){
	uint8_t *buf = kmalloc(512);
	ata_read(lba, 1, 0, (uint16_t*)buf);
	return (struct fat32_dir_entry*)buf;
}

uint32_t fat32_next_clust(uint32_t cluster){
	uint32_t rsvd = fat_info.rsvd_sect_count;
	uint32_t bps = fat_info.bytes_per_sect;
	uint32_t fat_sect = rsvd + (cluster * 4) / bps;
	uint32_t offset = (cluster * 4) % bps;
	uint8_t *buf = kmalloc(512);
	ata_read(fat_sect, 1, 0, (uint16_t*)buf);
	uint32_t next = *(uint32_t*)(buf + offset) & 0x0FFFFFFF;
	kfree(buf);
	return next;
}

void fat32_read_file(uint32_t cluster, char *buf){
	while (cluster <= 0x0FFFFFF8){
		uint32_t lba = fat32_cluster_to_lba(cluster);
		ata_read(lba, 1, 0, (uint16_t*)buf);
		buf += fat_info.bytes_per_sect;
		cluster = fat32_next_clust(cluster);
	}
}

/* THESE FUNCTIONS ARE BROKEN, TODO: FIX THIS */
/*
void fat32_write_file(uint32_t cluster, char *buf, uint32_t size){
	uint32_t bytes_written = 0;
	uint32_t bps = fat_info.bytes_per_sect;
	uint32_t spc = fat_info.sects_per_clust;
	uint32_t bpc = bps * spc;
	uint32_t current_clust = cluster;
	uint32_t next_clust;

	while (bytes_written < size){
		uint32_t lba = fat32_cluster_to_lba(current_clust);
		uint32_t to_write = (size - bytes_written > bpc) ?
			bpc : (size - bytes_written);
		ata_write(lba, spc, 0, (uint16_t*)(buf + bytes_written));

		bytes_written += to_write;
        
		if (bytes_written < size) {
			next_clust=  fat32_find_free_cluster();
			fat32_set_fat_entry(current_clust, next_clust);
			current_clust = next_clust;
		} else {
			fat32_set_fat_entry(current_clust, 0x0FFFFFF8);
		}
	}
}

uint32_t fat32_find_free_cluster() {
	uint32_t bps = fat_info.bytes_per_sect;
	uint32_t fat_size = fat_info.fat_size;
	uint32_t rsvd = fat_info.rsvd_sect_count;
	uint32_t entries_per_sector = bps / 4;
	
	for (uint32_t fat_sect = 0; fat_sect < fat_size; fat_sect++) {
		uint8_t *buf = kmalloc(512);
		ata_read(rsvd + fat_sect, 1, 0, (uint16_t*)buf);
		
		for (uint32_t i = 0; i < entries_per_sector; i++) {
			uint32_t cluster = fat_sect * entries_per_sector + i;
			if (cluster < 2) continue;
			
			uint32_t entry = *(uint32_t*)(buf + i * 4) & 0x0FFFFFFF;
			if (entry == 0) {
				kfree(buf);
				return cluster;
			}
		}
		kfree(buf);
	}
	return 0;
}

uint32_t fat32_get_fat_entry(uint32_t cluster) {
	uint32_t rsvd = fat_info.rsvd_sect_count;
	uint32_t bps = fat_info.bytes_per_sect;
	uint32_t fat_sect = rsvd + (cluster * 4) / bps;
	uint32_t offset = (cluster * 4) % bps;
	uint8_t *buf = kmalloc(512);
	ata_read(fat_sect, 1, 0, (uint16_t*)buf);
	uint32_t entry = *(uint32_t*)(buf + offset) & 0x0FFFFFFF;
	kfree(buf);
	return entry;
}

void fat32_set_fat_entry(uint32_t cluster, uint32_t value) {
	uint32_t rsvd = fat_info.rsvd_sect_count;
	uint32_t bps = fat_info.bytes_per_sect;
	uint32_t fat_sect = rsvd + (cluster * 4) / bps;
	uint32_t offset = (cluster * 4) % bps;
	uint8_t *buf = kmalloc(512);
	
	ata_read(fat_sect, 1, 0, (uint16_t*)buf);
	
	uint32_t *entry = (uint32_t*)(buf + offset);
	*entry = (*entry & 0xF0000000) | (value & 0x0FFFFFFF);
	
	ata_write(fat_sect, 1, 0, (uint16_t*)buf);
	kfree(buf);
}
*/