#include "utility.h"
#include "inode.h"
#include <iostream>
#include <cstdint>

namespace INTCHK5806 {
	inode::inode(uint8_t * buf) {
		i_mode = bufferTo2Bytes(buf,0);
		i_uid = bufferTo2Bytes(buf,2);
		i_size = bufferTo4Bytes(buf,4);
		i_atime = bufferTo4Bytes(buf,8);
		i_ctime = bufferTo4Bytes(buf,12);
		i_mtime = bufferTo4Bytes(buf,16);
		i_dtime = bufferTo4Bytes(buf,20);
		i_gid = bufferTo2Bytes(buf,24);
		i_links_count = bufferTo2Bytes(buf,26);
		i_blocks = bufferTo4Bytes(buf,28);
		i_flags = bufferTo4Bytes(buf,32);
		i_osd1 = bufferTo4Bytes(buf,36);
		i_block[0] = bufferTo4Bytes(buf,40);
		i_block[1] = bufferTo4Bytes(buf,44);
		i_block[2] = bufferTo4Bytes(buf,48);
		i_block[3] = bufferTo4Bytes(buf,52);
		i_block[4] = bufferTo4Bytes(buf,56);
		i_block[5] = bufferTo4Bytes(buf,60);
		i_block[6] = bufferTo4Bytes(buf,64);
		i_block[7] = bufferTo4Bytes(buf,68);
		i_block[8] = bufferTo4Bytes(buf,72);
		i_block[9] = bufferTo4Bytes(buf,76);
		i_block[10] = bufferTo4Bytes(buf,80);
		i_block[11] = bufferTo4Bytes(buf,84);
		i_block[12] = bufferTo4Bytes(buf,88);
		i_block[13] = bufferTo4Bytes(buf,92);
		i_block[14] = bufferTo4Bytes(buf,96);
		i_generation = bufferTo4Bytes(buf,100);
		i_file_acl = bufferTo4Bytes(buf,104);
		i_dir_acl = bufferTo4Bytes(buf,108);
		i_faddr = bufferTo4Bytes(buf,112);
		i_osd2[0] = bufferTo4Bytes(buf,116);
		i_osd2[1] = bufferTo4Bytes(buf,120);
		i_osd2[2] = bufferTo4Bytes(buf,124);
		//std::cout << i_mode << std::endl;
	}

	inode::~inode() {
		
	}

	uint16_t inode::get_i_mode() {
		return i_mode;
	}
	uint16_t inode::get_i_uid() {
		return i_uid;
	}
	uint32_t inode::get_i_size() {
		return i_size;
	}
	uint32_t inode::get_i_atime() {
		return i_atime;
	}
	uint32_t inode::get_i_ctime() {
		return i_ctime;
	}
	uint32_t inode::get_i_mtime() {
		return i_mtime;
	}
	uint32_t inode::get_i_dtime() {
		return i_dtime;
	}
	uint16_t inode::get_i_gid() {
		return i_gid;
	}
	uint16_t inode::get_i_links_count() {
		return i_links_count;
	}
	uint32_t inode::get_i_blocks() {
		return i_blocks;
	}
	uint32_t inode::get_i_flags() {
		return i_flags;
	}
	uint32_t inode::get_i_osd1() {
		return i_osd1;
	}
	uint32_t * inode::get_i_block(){
		return i_block;
	}
	uint32_t inode::get_i_generation() {
		return i_generation;
	}
	uint32_t inode::get_i_file_acl() {
		return i_file_acl;
	}
	uint32_t inode::get_i_dir_acl() {
		return i_dir_acl;
	}
	uint32_t inode::get_i_faddr() {
		return i_faddr;
	}
	uint32_t * inode::get_i_osd2() {
		return i_osd2;
	}
}