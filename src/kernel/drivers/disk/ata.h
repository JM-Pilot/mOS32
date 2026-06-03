/* MIT LICENSE (C) 2026 JM-Pilot */
#include <stdint.h>
#ifndef ATA_H
#define ATA_H

void ata_read(uint32_t lba, uint8_t count, uint8_t slave, uint16_t *buf);
void ata_write(uint32_t lba, uint8_t count, uint8_t slave, uint16_t *buf);
#endif