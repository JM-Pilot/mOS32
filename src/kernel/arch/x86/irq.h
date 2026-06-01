/* MIT LICENSE (C) 2026 JM-Pilot */
#include <stdint.h>
#include "idt.h"
#ifndef IRQ_H
#define IRQ_H

void irq_install_handler(int irq, void (*handler)(struct register_frame *r));

void irq_uninstall_handler(int irq);

void irq_handler(struct register_frame *reg);
#endif