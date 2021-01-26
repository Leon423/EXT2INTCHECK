#ifndef T_PARTITION_H
#define T_PARTITION_H
#include <stdint.h>

namespace INTCHK5806 {
	/*
		representation of a partition entry
	 */
	class t_partition
	{
	private:
		bool bootable;
		uint8_t systemID;
		uint32_t offsetStart;
		uint32_t offsetEnd;
	public:
		t_partition(uint8_t *);
		uint32_t get_offsetStart();
		uint32_t get_offsetEnd();
		bool isBootable();
		bool isLinux();
	};
}

#endif