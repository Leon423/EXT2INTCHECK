#ifndef INODE_5806_H
#define INODE_5806_H

#include <cstdint>

namespace INTCHK5806 {
	/*
		reperesents an inode
	 */
	class inode
	{
	private:
		uint16_t i_mode;
		uint16_t i_uid;
		uint32_t i_size;
		uint32_t i_atime;
		uint32_t i_ctime;
		uint32_t i_mtime;
		uint32_t i_dtime;
		uint16_t i_gid;
		uint16_t i_links_count;
		uint32_t i_blocks;
		uint32_t i_flags;
		uint32_t i_osd1;
		uint32_t i_block[15];
		uint32_t i_generation;
		uint32_t i_file_acl;
		uint32_t i_dir_acl;
		uint32_t i_faddr;
		uint32_t i_osd2[3];
	public:
		inode(uint8_t *);
		~inode();
		uint16_t get_i_mode();
		uint16_t get_i_uid();
		uint32_t get_i_size();
		uint32_t get_i_atime();
		uint32_t get_i_ctime();
		uint32_t get_i_mtime();
		uint32_t get_i_dtime();
		uint16_t get_i_gid();
		uint16_t get_i_links_count();
		uint32_t get_i_blocks();
		uint32_t get_i_flags();
		uint32_t get_i_osd1();
		uint32_t * get_i_block();
		uint32_t get_i_generation();
		uint32_t get_i_file_acl();
		uint32_t get_i_dir_acl();
		uint32_t get_i_faddr();
		uint32_t * get_i_osd2();
	};
}

#endif