#ifndef __progflash_fs__
#define __progflash_fs__

#include <stdint.h>
#include <avr/pgmspace.h>
#include <avr/boot.h>

typedef struct _dj_pfs_file_header dj_pfs_file_header;

struct _dj_pfs_file_header
{
	uint8_t magic[4];
	uint16_t id;
	uint16_t offset;
	uint16_t size;
	uint16_t name_length;
	uint16_t used:1;
	uint16_t boot:1;
	uint16_t infusion:1;
	uint16_t reserved:13;
} __attribute__ ((__packed__));

char dj_pfs_init();

char dj_pfs_createFile(int size, const char *name, char boot, char infusion);
char dj_pfs_deleteFile(int id);

void dj_pfs_startList();
int dj_pfs_nextFile();

char dj_pfs_getFileName(int id, char * namebuf, int bufSize);
unsigned char dj_pfs_getFileFlags(int id);
unsigned int dj_pfs_getFileSize(int id);
unsigned int dj_pfs_getFileAddress(int id);

char dj_pfs_setFileFlags(int id, unsigned char flags);

void dj_pfs_dump();

#endif
