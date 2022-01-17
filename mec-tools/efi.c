/* gcc -o efi -O2 efi.c */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
 
#define MAX_PE_IMAGES                  63
#define FILE_TYPE_FIXED_LOADER         0
#define FILE_TYPE_RELOCATABLE_PE_IMAGE 1
 
typedef struct {
	uint32_t checksum;
	uint32_t offset;
	uint32_t length;
	char fn[52];
} EFILDR_IMAGE;
 
typedef struct {
	uint32_t magic;
	uint32_t checksum;
	uint32_t filelength;
	uint32_t filecount;
} EFILDR_HEADER;
 
#define EFIL_MAGIC 0x4546494C // 'EFIL'

/* return 0 if found.
 * return 1 if not found
 */
int
find_magic(const uint8_t *src, int len, uint32_t match, int *r)
{
	int i;
	uint8_t key[4];

	if (src == NULL || len < 2 || r == NULL) return 1;

	key[0] = (uint8_t)((match >> 24) & 0xff);
	key[1] = (uint8_t)((match >> 16) & 0xff);
	key[2] = (uint8_t)((match >> 8) & 0xff);
	key[3] = (uint8_t)(match & 0xff);

	for (i = 0; i < len - 3; i ++) {
		if ((src[i] == key[0]) && (src[i + 1] == key[1]) && (src[i + 2] == key[2]) && (src[i + 3] == key[3])) {
			*r = i;
			return 0;
		}
	}

	return 1;
}

int main(int argc, char **argv)
{
	FILE *in, *out;
	EFILDR_HEADER header;
	EFILDR_IMAGE image;
	uint8_t *buf;
	char fn[52], *p;
	int filesize, i, pos, start, j;
	
	if (argc == 1) {
		printf("Usage: %s fn ...\n", argv[0]);
		return 1;
	}

	for (i = 1; i < argc; i ++) {
		in = fopen(argv[i], "rb");
		if (in == NULL) {
			printf("can't open %s\n", argv[i]);
			continue;
		}
		fseek(in, 0, SEEK_END);
		filesize = ftell(in);
		fseek(in, 0, SEEK_SET);
		buf = malloc(filesize + 1);
		if (buf == NULL) {
			fclose(in);
			continue;
		}
		if (filesize != fread(buf, 1, filesize, in)) {
			free(buf);
			fclose(in);
			continue;
		}
		fclose(in);
		/* processing file */
		/* to find 'EFIL' in buffer */

		if (1 == find_magic(buf, filesize, EFIL_MAGIC, &pos)) {
			printf("can't find 'EFIL' magic in %s, continue\n", argv[i]);
			free(buf);
			continue;
		}

		memcpy(&header, buf + pos, sizeof(header));
		printf("found EFIL header at #%d of %s(filesize #%d) -> \n", pos, argv[i], filesize);
		printf("\tchecksum 0x%x, file length %d, total file %d\n", header.checksum, header.filelength, header.filecount);

		start = pos;
		pos += sizeof(header);
		for (j = 1; j <= header.filecount; j ++) {
			/* walk through efi file */
			memcpy(&image, buf + pos, sizeof(image));
			printf("#%d file -> %s, checksum 0x%x, offset %d, length %d\n", j, image.fn, image.checksum, image.offset, image.length);
			if (image.fn[50] == '\\') {
				/* no filename */
				snprintf(fn, 52, "%d.image", j);
			} else {
				p = strrchr(image.fn, '\\');
				if (p == NULL)
					strcpy(fn, image.fn);
				else
					strcpy(fn, p + 1);
			}
			out = fopen(fn, "wb");
			if (out) {
				fwrite(buf + start + image.offset, 1, image.length, out);
				fclose(out);
				printf("write #%d image to %s (filesize %d)\n", j, fn, image.length);
			}

			pos += sizeof(image); //image.length;
		}
		free(buf);
	}
}
