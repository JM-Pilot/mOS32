/* MIT LICENSE (C) 2026 JM-Pilot */
#include <stdint.h>
#ifndef FAT32_H
#define FAT32_H

struct fat32_bpb {
	uint8_t nop[3];
	uint8_t oem[8];
	uint16_t bytes_per_sect;
	uint8_t sect_per_clust;
	uint16_t rsvd_sect_count;
	uint8_t num_fats;
	uint16_t root_entries_cnt;
	uint16_t total_sects_16;
	uint8_t media;
	uint16_t sect_per_fat_16;
	uint16_t sect_per_track;
	uint16_t head_count;
	uint32_t hidden_sects;
	uint32_t total_sects_32;
}__attribute__((packed));

struct fat32_ebpb {
	uint32_t sect_per_fat_32;
	uint16_t ext_flags;
	uint16_t fs_version;
	uint32_t root_cluster;
	uint16_t fs_info;
	uint16_t bk_root_sect;
	uint8_t reserved_1[12];
	uint8_t drive_num;
	uint8_t reserved_2;
	uint16_t boot_signature;
	uint32_t volume_id;
	uint8_t volume_label[11];
	uint8_t filesys_type[8];
}__attribute__((packed));

struct fat32_bpbwe {
	struct fat32_bpb bpb;
	struct fat32_ebpb ebpb;
} __attribute__((packed));

struct fat32_info {
	uint32_t bytes_per_sect;
	uint8_t sects_per_clust;
	uint32_t rsvd_sect_count;
	uint32_t fat_size;
	uint32_t first_data_sect;
	uint32_t root_clust;
	uint32_t total_sect;
};

struct fat32_dir_entry {
	uint8_t filename[11];
	uint8_t attribute;
	uint8_t ntrsvd;
	uint8_t creation_time_th;
	uint16_t creation_time;
	uint16_t creation_date;
	uint16_t last_accessed;
	uint16_t clust_high;
	uint16_t last_mod_time;
	uint16_t last_mod_date;
	uint16_t clust_low;
	uint32_t size;
}__attribute__((packed));

struct fat32_long_file_name {
	uint8_t order;
	uint16_t first_bytes[5];
	uint8_t attributes;
	uint8_t le_type;	/* Long entry type, * le type * sounds french*/
	uint8_t checksum;
	uint16_t next_bytes[6];
	uint16_t zero;
	uint16_t final_bytes[2];
}__attribute__((packed));
extern struct fat32_info fat_info;
void fat32_init();
uint32_t fat32_cluster_to_lba(uint32_t cluster);
struct fat32_dir_entry *fat32_read_dir(uint32_t lba);
uint32_t fat32_next_clust(uint32_t cluster);
void fat32_read_file(uint32_t cluster, char *buf);

void fat32_write_file(char *file_name, char *buf, uint32_t size);
uint32_t fat32_find_free_clust();

uint8_t fat32_lfn_checksum(uint8_t *short_name);

struct fat32_dir_entry *fat32_find_file(const char *filename);
#endif