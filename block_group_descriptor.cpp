#include "block_group_descriptor.h"
#include <stdint.h>
#include <iostream>
#include "utility.h"

using namespace std;

namespace INTCHK5806 {

	/*
		Constructor, creates a block_group_descriptor from a filled 1 k buffer
	 */
	block_group_descriptor::block_group_descriptor(uint8_t * buf) {
		block_bitmap = bufferTo4Bytes(buf, 0);
		inode_bitmap = bufferTo4Bytes(buf, 4);
		inode_table = bufferTo4Bytes(buf, 8);
		free_blocks_count = bufferTo2Bytes(buf, 12);
		free_inodes_count = bufferTo2Bytes(buf, 14);
		used_dirs_count = bufferTo2Bytes(buf, 16);
		pad = bufferTo2Bytes(buf, 18);
		for(int i = 0; i < 12; i++){
			reserved[i] = buf[20+i];
		}
		//std::cout << "bl_bm: " << block_bitmap << " in_bm: " << inode_bitmap << endl;
	}

	block_group_descriptor::~block_group_descriptor() {
		
	}
	
	// getters for all variables.
	uint32_t block_group_descriptor::get_block_bitmap(){
		return block_bitmap;
	}
	uint32_t block_group_descriptor::get_inode_bitmap(){
		return inode_bitmap;
	}
	uint32_t block_group_descriptor::get_inode_table(){
		return inode_table;
	}
	uint16_t block_group_descriptor::get_free_blocks_count(){
		return free_blocks_count;
	}
	uint16_t block_group_descriptor::get_free_inodes_count(){
		return free_inodes_count;
	}
	uint16_t block_group_descriptor::get_used_dirs_count(){
		return used_dirs_count;
	}
	uint16_t block_group_descriptor::get_pad(){
		return pad;
	}
	uint8_t * block_group_descriptor::get_reserved(){
		return reserved;
	}

	// prints out all relevant information for the block group descriptor
	void block_group_descriptor::print(){
		std::cout << "Block Bitmap Starting Block: " << block_bitmap << std::endl;
		std::cout << "Inode Bitmap Starting Block: " << inode_bitmap << std::endl;
		std::cout << "Inode Table Starting Block: " << inode_table << std::endl;
		std::cout << "Free Blocks Count: " << free_blocks_count << std::endl;
		std::cout << "Free Inodes Count: " << free_inodes_count << std::endl;
		std::cout << "Inodes used for Directories: " << used_dirs_count << std::endl;
	}


	// == operator for block group descriptor. Takes a pointer to another block group descriptor and compares its values. Also prints out what was wrong if something was.
	bool block_group_descriptor::operator==(const block_group_descriptor* rhs){
		bool returnValue = true;

		if(this->block_bitmap != rhs->block_bitmap){
			returnValue = false;
			std::cout << "block bitmap wrong\n";
		}

		if(this->inode_bitmap != rhs->inode_bitmap){
			returnValue = false;
			std::cout << "inode bitmap wrong\n";
		}

		if(this->inode_table != rhs->inode_table){
			returnValue = false;
			std::cout << "inode table wrong\n";
		}

		if(this->free_blocks_count != rhs->free_blocks_count){
			returnValue = false;
			std::cout << "free blocks count wrong\n";
		}

		if(this->free_inodes_count != rhs->free_inodes_count){
			returnValue = false;
			std::cout << "free inodes count wrong\n";
		}

		if(this->used_dirs_count != rhs->used_dirs_count){
			returnValue = false;
			std::cout << "used directories count is wrong\n";
		}

		return returnValue;
	}

}