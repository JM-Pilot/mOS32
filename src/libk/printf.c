/* MIT LICENSE (C) 2026 JM-Pilot */
#include <stdint.h>
#include <stdarg.h>
#include <libk/stdio.h>
#include <libk/stdlib.h>
#include "../kernel/console/tty.h"
void k_vsprintf(char *buf, const char *fmt, va_list args){
	int i = 0;
	while (*fmt){
		if (*fmt == '%'){
			fmt++;
			switch (*fmt){
				case 'c': {
					buf[i++] = (char)va_arg(args, int);
					break;
				}
				case 'd': {
					char tmp[10];
					k_itoa(tmp, va_arg(args, int), 
						BASE_DEC);
					for (int j = 0; tmp[j] != '\0'; j++)
						buf[i++] = tmp[j];
					break;
				}
				case 'x': {
					char tmp[10];
					k_itoa(tmp, va_arg(args, uint32_t), 
						BASE_HEX);
					for (int j = 0; tmp[j] != '\0'; j++)
						buf[i++] = tmp[j];
					break;
				}
				case 's': {
					const char *s = va_arg(args, 
						const char*);
					while (*s) 
						buf[i++] = *s++;
					break;
				}
				default: {
					buf[i++] = *fmt;
					break;
				}
			}
		} else {
			buf[i++] = *fmt;
		}
		fmt++;
	}
	buf[i++] = '\0';
}
void k_printf(const char *fmt, ...){
	va_list args;
	va_start(args, fmt);
	char buffer[2048];
	k_vsprintf(buffer, fmt, args);
	va_end(args);
	k_puts(buffer, STDOUT);
}