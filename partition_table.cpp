#include "partition_table.h"
#include "t_partition.h"

namespace INTCHK5806 {
	/*
		Constructor, creates a partition table from a 64 byte buffer
	 */
	partition_table::partition_table(uint8_t * buf) {
		p_entries[0] = new t_partition(buf);
		p_entries[1] = new t_partition(buf+16);
		p_entries[2] = new t_partition(buf+32);
		p_entries[3] = new t_partition(buf+48);
	}

	/*
		returns the offset in bytes to the start of the linux partition
	 */
	int partition_table::get_linuxOffsetStart() {
		for (int i = 0; i < 4; i++) 
			if (p_entries[i]->isLinux()) 
				return p_entries[i]->get_offsetStart();
		return -1;
	}
}