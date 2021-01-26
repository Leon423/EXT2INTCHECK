#ifndef f_header_H
#define f_header_H
#include <stdint.h>

namespace INTCHK5806 {
	/*
		Representation of a file header
	 */
	class f_header
	{
	private:
		// 1 == dynamic, 2 == fixed
		unsigned int imageType; // located at bytes 77-80
		unsigned int offsetBlocks; //located at 117-120
		unsigned int offsetData; // located at bytes 121-124
		unsigned int numberOfSectors; // located at bytes 133-136
		unsigned int sectorSize; // located at bytes 137-140
		unsigned int diskSize; // located at bytes 145-152
		unsigned long int blockSize; // located at bytes 153-156 (pagesize)
		unsigned int blocksInHDD; // located 161-164
		unsigned int blocksAllocated; // located 165-168

	public:
		f_header(unsigned int *, unsigned int);
		unsigned int get_ImageType();
		unsigned int get_OffsetBlocks();
		unsigned int get_OffsetData();
		unsigned int get_NumberOfSectors();
		unsigned int get_SectorSize();
		unsigned long int get_DiskSize();
		unsigned int get_BlockSize();
		unsigned int get_BlocksInHDD();
		unsigned int get_BlocksAllocated();
	};
}

#endif