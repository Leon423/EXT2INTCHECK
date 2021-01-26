#include "f_header.h"
#include <stdint.h>
#include <iostream>

using namespace std;

namespace INTCHK5806 {
	/*
		Constructor, creates a file header from a buffer
	 */
	f_header::f_header(unsigned int * buf, unsigned int imageT) {

		imageType = imageT;
		offsetBlocks = buf[0];
		offsetData = buf[1];
		numberOfSectors = buf[4];
		sectorSize = buf[5];
		//disk size here, need to figure it out. (buf[7 and 8])
		diskSize = buf[7] | (buf[8] << sizeof(int));
		blockSize = buf[9];
		blocksInHDD = buf[11];
		blocksAllocated = buf[12];
	}

	// getters
	unsigned int f_header::get_ImageType(){
		return imageType;
	}

	unsigned int f_header::get_OffsetBlocks(){
		return offsetBlocks;
	}

	unsigned int f_header::get_OffsetData(){
		return offsetData;
	}

	unsigned int f_header::get_NumberOfSectors() {
		return numberOfSectors;
	}

	unsigned int f_header::get_SectorSize() {
		return sectorSize;
	}

	unsigned long int f_header::get_DiskSize() {
		return diskSize;
	}

	unsigned int f_header::get_BlockSize() {
		return blockSize;
	}

	unsigned int f_header::get_BlocksInHDD() {
		return blocksInHDD;
	}

	unsigned int f_header::get_BlocksAllocated() {
		return blocksAllocated;
	}
	
}