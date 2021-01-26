#include <cstdint>
#include <cstring>
#include "dirEntry.h"
#include "utility.h"

namespace INTCHK5806 {
	/*
		takes a buffer filled with data for a directory entry
		returns a directory entry object
	 */
	dirEntry::dirEntry(uint8_t * dirBlock) {
		this->iNum = bufferTo4Bytes(dirBlock,0);
		this->rec_len = bufferTo2Bytes(dirBlock,4);
		this->name_len = dirBlock[6];
		this->file_type = dirBlock[7];
		this->name = new char[name_len];
		for (int i = 0; i < name_len; i++) {
			name[i] = dirBlock[8+i];
		}
	}

	/*
		deletes the array storing the name attribute
	 */
	dirEntry::~dirEntry() {
		delete name;
	}

	/*
		getters
	 */
	uint32_t dirEntry::get_iNum() {
		return iNum;
	}
	uint16_t dirEntry::get_rec_len() {
		return rec_len;
	}
	uint8_t dirEntry::get_name_len() {
		return name_len;
	}
	uint8_t dirEntry::get_file_type() {
		return file_type;
	}
	char * dirEntry::get_name() {
		return name;
	}

	/*
		returns whether the dir entry is a file
	 */
	bool dirEntry::isFile() {
		return file_type != 2;
	}

	/*
		returns whether dir entry is a dir
	 */
	bool dirEntry::isDir() {
		return file_type == 2;
	}
}