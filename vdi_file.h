#ifndef VDI_FILE_H
#define VDI_FILE_H

#include <string>
#include "partition_table.h"
#include "f_header.h"
#include "superblock.h"
#include "bgdt.h"
#include "inode_table.h"
#include "inode.h"


#include <string>
namespace INTCHK5806 {
	/*
		Representation of a vdi file
	 */
	class vdi_file {
	private:
		std::string filePath;			//path to the vdi
		int file_descriptor;			//the file descriptor number
		f_header * fh;					//the file header object
		partition_table * pt;			//the partition table
		int * tlTB;						//the translation table
	public:
		vdi_file(std::string path);
		~vdi_file();
		void vdi_open();
		int vdi_read(void * buf, std::size_t bytes, std::size_t offset);
		void vdi_close();
		void vdi_seek(int offset, int way = 0);
		int vdi_write(void *, std::size_t, std::size_t);
		f_header * createFileHeader();
	};
}

#endif
