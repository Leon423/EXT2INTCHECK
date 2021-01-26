#ifndef IN_TB_5806_H
#define IN_TB_5806_H

#include <cstdint>
#include "inode.h"

namespace INTCHK5806 {
	/*
		represents an inode table
	 */
	class inode_table
	{
	private:
		inode ** iarr;		//pointer to array of inode pointers
		int nI;				//number of inodes
	public:
		inode_table(uint8_t *, int);
		~inode_table();
		inode * get_inode(int);
	};
}

#endif