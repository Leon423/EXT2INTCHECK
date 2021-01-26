#ifndef _UTILITY_5806_H
#define _UTILITY_5806_H

#include <cstdint>

namespace INTCHK5806 {
	/*
		Pulls a 4 byte piece of data from a buffer of bytes
		@params uint8_t * buffer to get 4 byte value from
		@params int offset into the buffer for the start of the value
	 */
	uint32_t bufferTo4Bytes(uint8_t * buf, int offset);

	/*
		Pulls a 2 byte piece of data from a buffer of bytes
		@params uint8_t * buffer to get 2 byte value from
		@params int offset into the buffer for the start of the value
	 */
	uint16_t bufferTo2Bytes(uint8_t * buf, int offset);
}

#endif