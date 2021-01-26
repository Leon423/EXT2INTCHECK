#include "t_partition.h"
#include <stdint.h>
#include <iostream>

namespace INTCHK5806 {

	/*
		Constructor, creates a partition entry from a 16 byte buffer
	 */
	t_partition::t_partition(uint8_t * buf) {
		bootable = (buf[0] == 0x80) ? true : false;
		systemID = buf[4];
		offsetStart = (uint32_t)(buf[8] | (buf[9] << 8) | (buf[10] << 16) | (buf[11] << 24)) * 512;
		offsetEnd = (uint32_t)(buf[8] | (buf[9] << 8) | (buf[10] << 16) | (buf[11] << 24)) * 512 + offsetStart;
	}

	/*
		returns whether the partition is bootable
	 */
	bool t_partition::isBootable () {
		return bootable;
	}

	/*
		returns whether the partition is a linux partition
	 */
	bool t_partition::isLinux() {
		return systemID == 0x83;
	}

	/*
		returns the offset in bytes for the start of the partition
	 */
	uint32_t t_partition::get_offsetStart () {
		return offsetStart;
	}

	/*
		returns the offset in bytes for the end of the partition
	 */
	uint32_t t_partition::get_offsetEnd() {
		return offsetEnd;
	}
}