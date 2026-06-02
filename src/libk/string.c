
/* MIT LICENSE (C) 2026 JM-Pilot */
#include <stdint.h>
#include <stddef.h>
#include <libk/string.h>

void *k_memset(void *dest, int val, size_t len){
	uint8_t *ptr = (uint8_t*)dest;
	while (len--)
		*ptr++ = val;
	return dest;
}

void *k_memcpy(void *dest, const void *src, size_t len){
	uint8_t *d = (uint8_t*)dest;
	uint8_t *s = (uint8_t*)src;
	while (len--){
		*d++ = *s++;
	}
	return dest;
}

void *k_memmove(void *dest, void *src, size_t len){
	char *d = dest;
	char *s = src;
	if (d < s){
		while (len--)
			*d++ = *s++;
	} else {
		char *lasts = s + (len - 1);
		char *lastd = d + (len - 1);
		while (len--)
			*lastd++ = *lasts++;
	}
	return dest;
}

int k_memcmp(const void *a, const void *b, size_t count){
	const uint8_t *a1 = a;
	const uint8_t *b1 = b;
	while (count--){
		if (*a1++ != *b1++)
			return a1[-1] < b1[-1] ? -1 : 1;
	}
	return 0;
}

/* Old GLIBC harder to make it work here */
int k_strcmp(const char *p1, const char *p2) {
	const uint8_t *s1 = (const uint8_t *) p1;
	const uint8_t *s2 = (const uint8_t *) p2;
	uint8_t c1, c2;

	do {
		c1 = (uint8_t) *s1++;
		c2 = (uint8_t) *s2++;
		if (c1 == '\0')
			return c1 - c2;
	}
	while (c1 == c2);

	return c1 - c2;
}
int k_strncmp(const char *s1, const char *s2, size_t n) {
	uint8_t u1, u2;

	while (n--) {
		u1 = (uint8_t) *s1++;
		u2 = (uint8_t) *s2++;
		if (u1 != u2)
			return u1 - u2;
		if (u1 == '\0')
			return 0;
	}
	return 0;
}

char *k_strcpy(char *dest, const char *src){
	if(dest == NULL)
		return NULL;
	char *ptr = dest;
	while(*src != '\0'){
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return ptr;
}