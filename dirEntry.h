#ifndef DIR_ENTRY_H
#define DIR_ENTRY_H

#include <cstdint>

namespace INTCHK5806 {
	/*
		represents a directory entry
	 */
	class dirEntry
	{
	private:
		uint32_t iNum;		//inode number
		uint16_t rec_len;	// record length
		uint8_t name_len;	//name length
		uint8_t file_type;	//file type
		char * name;		//name
	public:
		dirEntry(uint8_t *);
		~dirEntry();
		uint32_t get_iNum();
		uint16_t get_rec_len();
		uint8_t get_name_len();
		uint8_t get_file_type();
		char * get_name();
		bool isFile();
		bool isDir();
	};
}

#endif