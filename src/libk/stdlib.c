/* MIT LICENSE (C) 2026 JM-Pilot */
#include <libk/stdlib.h>
#include <stdint.h>

char *k_itoa(char *buf, int val, int base){
	if (base == BASE_DEC){
		int i, sign;
		char tmp_buffer[10];
		if ((sign = val) < 0){
			val = -val;
		}
		i = 0;
		do {
			tmp_buffer[i++] = val % 10 + '0';
		} while ((val /= 10) > 0);

		if (sign < 0){
			tmp_buffer[i++] = '-';
		}
		int j = 0;
		while (i--) 
			buf[j++] = tmp_buffer[i];
		buf[j++] = '\0';
	} else if (base == BASE_HEX){
		char hex_val[] = "0123456789ABCDEF";
		char tmp_buffer[10]; 
		int i, j;
		for (i = 0; i < 7; i++) {
			tmp_buffer[i] = hex_val[val & 0x0F];
			val >>= 4;
		}
		tmp_buffer[i++] = 'x';
		tmp_buffer[i++] = '0';
		j = 0;
		while (i--)
			buf[j++] = tmp_buffer[i];
		buf[j++] = '\0';
	} else if (base == BASE_BIN){
		char tmp_buffer[34]; 
		int i, j;
		for (i = 0; i <= 32; i++) {
			tmp_buffer[i] = '0' + (val & 0x01);
			val >>= 1;
		}
		tmp_buffer[i++] = 'b';
		tmp_buffer[i++] = '0';
		j = 0;
		while (i--)
			buf[j++] = tmp_buffer[i];
		buf[j++] = '\0';
	}
	return buf;
}