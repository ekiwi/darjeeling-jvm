#include "stdio.h"
#include "string.h"

#include "progflash_fs.h"

#include <avr/interrupt.h>
#include <avr/boot.h>

static uint32_t range_start, range_end;
static uint32_t data_start, index_start;
static uint32_t next_id;

#define PAGEADDR(addr) ((uint32_t)((uint32_t)addr & ~(uint32_t)(SPM_PAGESIZE-1)))
#define PAGEOFFSET(addr) ((uint32_t)((uint32_t)addr & (uint32_t)(SPM_PAGESIZE-1)))

static inline void dj_pfs_writeBytes( uint8_t * bytes, uint32_t startAddress, uint32_t length)
{
	uint32_t endAddress = startAddress + length;
	uint32_t startPage = PAGEADDR(startAddress);
	uint32_t endPage = PAGEADDR(endAddress);
	uint32_t middlePage;

	// If startPage equals endPage, the write spans just a single page and we can have to perform only
	// a single write. Otherwise the write command spans multiple pages
	if (startPage==endPage)
	{
		// boot_program_page(startPage, PAGEOFFSET(startAddress), PAGEOFFSET(endAddress), bytes);
//		printf("%d bytes written\n",
//				PAGEOFFSET(endAddress) - PAGEOFFSET(startAddress)
//				);
	} else
	{
//		printf("Long write\n");
		// write the first page
		uint32_t offset = PAGEADDR(startAddress);
		// boot_program_page(startPage, offset, SPM_PAGESIZE, bytes);
		bytes += SPM_PAGESIZE - offset;

//		// copy entire pages in between startPage and endPage
//		for (middlePage = startPage + SPM_PAGESIZE; middlePage<endPage; middlePage += SPM_PAGESIZE)
//		{
//		}


	}


}

static inline void dj_pfs_readFileIndex(dj_pfs_file_header * header, uint32_t address)
{
	// fos_progflash_read(header, address, sizeof(dj_pfs_file_header));
}

static inline void dj_pfs_writeFileIndex(dj_pfs_file_header * header, uint32_t address)
{
	dj_pfs_writeBytes( (uint8_t*)header, address, sizeof(dj_pfs_file_header) );
}

static void dj_pfs_initFileHeader(dj_pfs_file_header * header)
{
	header->magic[0] = 'D';
	header->magic[1] = 'J';
	header->magic[2] = 'F';
	header->magic[3] = 'S';
}

char dj_pfs_init()
{
	uint32_t range_size = 4096;
//	range_end = fos_progflash_top();
//	range_start = fos_progflash_top() - range_size;
	dj_pfs_file_header header;

	// check if a file system is present
	dj_pfs_readFileIndex(&header, range_start);
	if ( (header.magic[0]!='D') || (header.magic[1]!='J') ||
		(header.magic[2]!='F') || (header.magic[3]!='S') )
	{
//		printf("FILE SYSTEM NOT FOUND, BOOTSTRAPPING\n");
		dj_pfs_initFileHeader(&header);
		header.size = range_size;
		header.used = 0;
		header.id = 0;
		dj_pfs_writeFileIndex(&header, range_start);
	}
	return 0;
}

static uint32_t dj_pfs_findFreeBlock(dj_pfs_file_header * header, uint32_t size)
{

	// read file header
	uint32_t address = range_start;
	while (address<range_end)
	{
		dj_pfs_readFileIndex(&header, address);
		if ((header->used==0)&&(header->size>=size)) return address;
	}

	// not found, return error -1
	return -1;
}

char dj_pfs_createFile(int size, const char *name, char boot, char infusion)
{
	dj_pfs_file_header header, unusedHeader;
	uint32_t address;
	uint32_t blockSize = size + strlen(name) + sizeof(dj_pfs_file_header);
	if (PAGEOFFSET(blockSize)>0) blockSize += SPM_PAGESIZE;

	if ((address=dj_pfs_findFreeBlock(&header, blockSize))==-1)
	{
//		printf("ERROR NOT ENOUGH SPACE");
		// TODO: coalesce
	} else
	{
		// split the block
		unusedHeader.size = header.size - blockSize;
		unusedHeader.used = 0;
		dj_pfs_initFileHeader(&unusedHeader);
		dj_pfs_writeFileIndex(&unusedHeader, address + blockSize);

		// split the block
		header.size = blockSize;
		header.used = 1;
		header.boot = boot;
		header.infusion = infusion;
		header.name_length = strlen(name);
	}

	return 0;
}

void dj_pfs_dump()
{
	dj_pfs_file_header header;

	// read file header
	uint32_t address = range_start;
	while (address<range_end)
	{
		dj_pfs_readFileIndex(&header, address);
//		printf("HEADER[id=%d, size=%d, used=%d, boot=%d, infusion=%d, name=%s]",
//				header.id,
//				header.size,
//				header.used,
//				header.boot,
//				header.infusion,
//				/*((char*)&header) + sizeof(dj_pfs_file_header)*/ "<niks nog>"
//				);
		address += header.size;
	}

}

char dj_pfs_deleteFile(int id)
{

}

void dj_pfs_startList()
{

}

int dj_pfs_nextFile()
{

}

char dj_pfs_getFileName(int id, char * namebuf, int bufSize)
{

}

unsigned int dj_pfs_getFileSize(int id)
{

}

unsigned char dj_pfs_getFileFlags(int id)
{

}

unsigned int dj_pfs_getFileAddress(int id)
{

}

char dj_pfs_setFileFlags(int id, unsigned char flags)
{

}
