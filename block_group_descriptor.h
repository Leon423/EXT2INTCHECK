#ifndef block_group_descriptor_H
#define block_group_descriptor_H
#include <stdint.h>

namespace INTCHK5806 {
	/*
		Representation of a block group descriptor. 
		Contains all information stored in a BGD and has a comparison operator and print() function to see its information.
	 */
	class block_group_descriptor
	{
	private:
		uint32_t block_bitmap;
		uint32_t inode_bitmap;
		uint32_t inode_table;
		uint16_t free_blocks_count;
		uint16_t free_inodes_count;
		uint16_t used_dirs_count;
		uint16_t pad;
		uint8_t reserved[12];

	public:
		block_group_descriptor(uint8_t *);
		~block_group_descriptor();
		uint32_t get_block_bitmap();
		uint32_t get_inode_bitmap();
		uint32_t get_inode_table();
		uint16_t get_free_blocks_count();
		uint16_t get_free_inodes_count();
		uint16_t get_used_dirs_count();
		uint16_t get_pad();
		uint8_t * get_reserved();
		void print();
		bool operator==(const block_group_descriptor* rhs);
	};
}

#endif