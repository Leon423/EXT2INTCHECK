#include "vdi_file.h"
#include "superblock.h"
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h> 
#include <string>
#include <iostream>
#include <fcntl.h>
#include "f_header.h"
#include "bgdt.h"
#include "inode_table.h"
#include "inode.h"
#include "err_def.h"

using namespace std;

namespace INTCHK5806 {
	/*
		Constructor
		@params string vdi file path
	 */
	vdi_file::vdi_file(std::string path) {
		this->filePath = path;
	}

	vdi_file::~vdi_file() {
		delete tlTB;
		delete pt;
		delete fh;
	}

	/*
		opens the vdi file and inits the file header, partiton table and translation table.
	 */
	void vdi_file::vdi_open() {
		//std::cout << "Opening file: " << this->filePath << std::endl;
		this->file_descriptor = open(filePath.c_str(), O_RDWR);
		if (file_descriptor < 0) {
			std::cerr << "Failed to open VDI file: " << filePath << std::endl;
			exit(ERR_VDI_OPEN);
		}
		//std::cout << sz << std::endl;
		fh = createFileHeader();

		tlTB = new int[fh->get_BlockSize()];
		lseek(this->file_descriptor,fh->get_BlockSize(),SEEK_SET);
		read(this->file_descriptor,tlTB,fh->get_BlockSize());
		unsigned char * ptBuf = new unsigned char[64];
		lseek(this->file_descriptor,fh->get_OffsetData()+446,SEEK_SET);
		read(this->file_descriptor,ptBuf,64);
		pt = new partition_table(ptBuf);
	}

	/*
		Used to read data from the vdi_file. Reading starts at the specified offset plus the start of the linux partition
		@params void * buffer to read data into.
		@params size_t number of bytes to read in
		@params size_t offset into the data to begin reading
		@returns int indicates whether the read succeeded or failed.
	 */
	int vdi_file::vdi_read(void * buf, std::size_t bytes, std::size_t offset) {
		//std::cout << "Reading file: " << this->filePath << std::endl;
		offset += pt->get_linuxOffsetStart();
		int vPage = offset / fh->get_BlockSize();
		int offsetPage = offset % fh->get_BlockSize();
		int pPage = tlTB[vPage];
		lseek(this->file_descriptor, pPage*fh->get_BlockSize() + fh->get_OffsetData() + offsetPage, SEEK_SET);
		return read(this->file_descriptor, buf, bytes);
	}

	/*
		seeks inside the vdi file
	 */
	void vdi_file::vdi_seek(int offset, int way) {
		switch (way) {
			case 0:
				lseek(this->file_descriptor,offset,SEEK_SET);
				break;
			case 1:
				lseek(this->file_descriptor,offset,SEEK_CUR);
				break;
			case 2:
				lseek(this->file_descriptor,offset,SEEK_END);
				break;
		}
	}

	/*
		used to write data to the vdi file
		@param void * buffer of data to write into the file
		@param size_t length of the buffer
	 */
	int vdi_file::vdi_write(void *buf, std::size_t bytes, std::size_t offset) {
		//std::cout << "Reading file: " << this->filePath << std::endl;
		offset += pt->get_linuxOffsetStart();
		int vPage = offset / fh->get_BlockSize();
		int offsetPage = offset % fh->get_BlockSize();
		int pPage = tlTB[vPage];
		lseek(this->file_descriptor, pPage*fh->get_BlockSize() + fh->get_OffsetData() + offsetPage, SEEK_SET);
		return write(this->file_descriptor, buf, bytes);
	}

	/*
		used to close the vdi file
	 */
	void vdi_file::vdi_close() {
		if (close(this->file_descriptor)==-1) {
			std::cerr << "Error closing file " << this->filePath << std::endl;
			exit(ERR_VDI_CLOSE);
		}
	}

	/*
		used to init the file header variable
		@returns f_header * pointer to a file header
	 */
	f_header * vdi_file::createFileHeader(){
		unsigned int * buf = new unsigned int[1];
		lseek(this->file_descriptor,76,0);
		read(this->file_descriptor,buf, 1);
		unsigned int imgType = buf[0];
		delete buf;

		buf = new unsigned int[13];
		lseek(this->file_descriptor,340,SEEK_SET);
		read(this->file_descriptor,buf, 52);
	// DEBUG STUFF

		//create file descriptor
		f_header * fd = new f_header(buf,imgType);
		//f_header fd(buf, imgType);
		//free buffer
		delete buf;
		// return file descriptor
		return fd;
	}
}