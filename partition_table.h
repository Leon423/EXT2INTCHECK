#ifndef PARTITION_TABLE
#define PARTITION_TABLE

#include <stdint.h>
#include "t_partition.h"

namespace INTCHK5806 {
	/*
		Representation of a partition table
	 */
	class partition_table
	{
	private:
		t_partition * p_entries[4];
	public:
		partition_table(uint8_t *);
		int get_linuxOffsetStart();
	};
}

#endif