#include "inode_table.h"
#include "inode.h"
#include <cstdint>
#include <iostream>

namespace INTCHK5806 {
	/*
		constructs an inode table object
		takes a buffer filled with the data for the inode table and an int representing the number of inodes in the table
	 */
	inode_table::inode_table(uint8_t * buf, int nI) {
		this->nI = nI;
		iarr = new inode*[nI];
		for (int i = 0; i < nI; i++) {
			iarr[i] = new inode(buf+i*128);
		}
		//delete buf;
	}

	//deletes all the pointers to the inodes
	inode_table::~inode_table() {
		for (int i = 0; i < nI; i++) {
			delete iarr[i];
		}
		delete iarr;
	}

	//returns the inode at the inode id (inode id is relative in this context to the inode table, used as an index into the inode table)
	inode * inode_table::get_inode(int inodeID) {
		return iarr[inodeID];
	}
}