/* MIT LICENSE (C) 2026 JM-Pilot */
#ifndef PS2_H
#define PS2_H

/* get a char from 0x60 then add it to tty_current->input_buf*/
char ps2_getch();
void ps2_init();
#endif