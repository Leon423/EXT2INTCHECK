#include "utility.h"
#include <cstdint>

namespace INTCHK5806 {
	/*
		Pulls a 4 byte piece of data from a buffer of bytes
		@params uint8_t * buffer to get 4 byte value from
		@params int offset into the buffer for the start of the value
	 */
	uint32_t bufferTo4Bytes(uint8_t * buf, int offset) {
		return buf[offset] | buf[offset+1] << 8 | buf[offset+2] << 16 | buf[offset+3] << 24;
	}

	/*
		Pulls a 2 byte piece of data from a buffer of bytes
		@params uint8_t * buffer to get 2 byte value from
		@params int offset into the buffer for the start of the value
	 */
	uint16_t bufferTo2Bytes(uint8_t * buf, int offset) {
		return buf[offset] | buf[offset+1] << 8;
	}
}