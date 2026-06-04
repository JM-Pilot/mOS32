/* MIT LICENSE (C) 2026 JM-Pilot */
#include <stdint.h>
#include "fat32.h"
#include "../drivers/disk/ata.h"
#include <libk/stdio.h>
#include <libk/stdlib.h>
#include <libk/string.h>
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

	fat_info.total_sect = (bpb.total_sects_16 == 0)? bpb.total_sects_32 :
		bpb.total_sects_16;
	
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
	while (cluster < 0x0FFFFFF8){
		uint32_t lba = fat32_cluster_to_lba(cluster);
		ata_read(lba, 1, 0, (uint16_t*)buf);
		buf += fat_info.bytes_per_sect;
		cluster = fat32_next_clust(cluster);
	}
}

void fat32_write_file(char *file_name, char *buf, uint32_t size){
	uint32_t free_clust = fat32_find_free_clust();
	uint32_t lba = fat32_cluster_to_lba(free_clust);
	uint32_t fat_sect = fat_info.rsvd_sect_count + (free_clust * 4) /
			    fat_info.bytes_per_sect;
	uint32_t offset = (free_clust * 4) % fat_info.bytes_per_sect;

	ata_write(lba, 1, 0, (uint16_t*)buf);
	uint32_t *tmp_buf = kmalloc(512);
	ata_read(fat_sect, 1, 0, (uint16_t*)tmp_buf);

	*(uint32_t*)((uint8_t*)tmp_buf + offset) = 0x0FFFFFFF;
	ata_write(fat_sect, 1, 0, (uint16_t*)tmp_buf);
	kfree(tmp_buf);


	struct fat32_dir_entry *entry = fat32_read_dir(fat32_cluster_to_lba(
		fat_info.root_clust));
	
	for (int i = 0; i < 512 / 32; i++){
		if (entry[i].filename[0] == 0x0){
			entry[i].size = size;
			entry[i].clust_high = (free_clust >> 16) & 0xFFFF;
			entry[i].clust_low = free_clust & 0xFFFF;
			entry[i].attribute = 0x20;
			entry[i].creation_date = 0;
			entry[i].creation_time = 0;
			entry[i].creation_time_th = 0;
			k_memcpy(entry[i].filename, file_name, 11);
			entry[i].last_accessed = 0;
			entry[i].last_mod_date = 0;
			entry[i].last_mod_time = 0;
			entry[i].ntrsvd = 0;
			break;
		}
	}
	uint32_t dir_lba = fat32_cluster_to_lba(fat_info.root_clust);
	ata_write(dir_lba, 1, 0, (uint16_t*)entry);
	kfree(entry);
}

void fat32_override_data_file(char *filename, char *buf, uint32_t size){
	struct fat32_dir_entry *dir = fat32_read_dir(fat32_cluster_to_lba(fat_info.root_clust));

	for (int i = 0; i < 512 / 32; i++){
		if (k_memcmp(dir[i].filename, "HELLO   TXT", 11) == 0){
			uint32_t cluster = dir[i].clust_low | (dir[i].clust_high << 16);
			uint32_t offset = 0;
			
			while (offset < size && cluster < 0x0FFFFFF8){
				uint32_t lba = fat32_cluster_to_lba(cluster);
				ata_write(lba, 1, 0, (uint16_t*)(buf + offset));
				offset += fat_info.bytes_per_sect;
				cluster = fat32_next_clust(cluster);
			}
			
			dir[i].size = size;
			break;
		}
	}

	ata_write(fat32_cluster_to_lba(fat_info.root_clust), 1, 0, (uint16_t*)dir);
	kfree(dir);
}

uint32_t fat32_find_free_clust(){
	uint32_t max = (fat_info.total_sect - fat_info.first_data_sect) / fat_info.sects_per_clust + 2;
	uint8_t *buf = kmalloc(512);
	for (uint32_t clust = 2; clust < max; clust++){
		uint32_t fat_sect = fat_info.rsvd_sect_count + (clust * 4) / fat_info.bytes_per_sect;
		uint32_t offset = (clust * 4) % fat_info.bytes_per_sect;
		if (offset == 0)
			ata_read(fat_sect, 1, 0, (uint16_t*)buf);
		if (*(uint32_t*)(buf + offset) == 0x0) {
			kfree(buf);
			return clust;
		}
	}
	kfree(buf);
	return 0;
}


uint8_t fat32_lfn_checksum(uint8_t *short_name) {
	uint8_t sum = 0;
	for (int i = 11; i; i--)
		sum = ((sum & 1) << 7) + (sum >> 1) + *short_name++;
	return sum;
}

/* LFN so hard :sob: */
/* Im too lazy to download emoji on my system ;)*/
void fat32_conv_lf2sf(const char *long_name, char *short_name){
	k_memset(short_name, ' ', 11);
	int i = 0, j = 0;
	while (long_name[i] && long_name[i] != '.' && j < 6){
		short_name[j++] = long_name[i++] >= 'a' ? long_name[i++] - 32 : 
			long_name[i++];
	}

	short_name[6] = '~';
	short_name[7] = '1';
	while (long_name[i] && long_name[i] != '.') i++;
	if (long_name[i] == '.') i++;
	j = 8;
	while (long_name[i] && j < 11){
		short_name[j++] = long_name[i++] >= 'a' ? long_name[i++] - 32 : 
			long_name[i++];
	}
		
}

struct fat32_dir_entry *fat32_find_file(const char *filename) {
	struct fat32_dir_entry *dir = fat32_read_dir(
		fat32_cluster_to_lba(fat_info.root_clust)
	);
	for (int i = 0; i < 512 / 32; i++) {
		if (dir[i].filename[0] == 0x00) break;
		if (dir[i].filename[0] == 0xE5) continue;
		if (k_memcmp(dir[i].filename, filename, 11) == 0)
		return &dir[i];
	}
	kfree(dir);
	return 0;
}