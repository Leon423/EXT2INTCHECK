#ifndef BGDT_H
#define BGDT_H

#include "block_group_descriptor.h"
#include <cstdint>

namespace INTCHK5806 {
	/*
		Represents a block group descriptor table
	 */
	class bgdt
	{
	private:
		block_group_descriptor ** bgdarr;	//pointer to an array of block group descriptor pointers
		int nBG;							//number of block groups in the table
	public:
		bgdt(uint8_t *, int);
		~bgdt();
		block_group_descriptor * get_block_group_descriptor(int);
		void print();

		bool operator==(const bgdt* rhs);
	};
}

#endif