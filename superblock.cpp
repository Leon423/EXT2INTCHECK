#include "superblock.h"
#include <stdint.h>
#include <iostream>
#include "utility.h"

using namespace std;

namespace INTCHK5806 {

	/*
		Constructor, creates a superblock from a 1 k buffer
	 */
	superblock::superblock(uint8_t * buf) {
		inodes_count = bufferTo4Bytes(buf, 0);
		blocks_count = bufferTo4Bytes(buf, 4);
		r_blocks_count = bufferTo4Bytes(buf, 8);
		free_blocks_count = bufferTo4Bytes(buf, 12);
		free_inodes_count = bufferTo4Bytes(buf, 16);
		first_data_block = bufferTo4Bytes(buf, 20);
		log_block_size = bufferTo4Bytes(buf, 24);
		block_size = 1024 << log_block_size;
		log_frag_size = bufferTo4Bytes(buf, 28);
		blocks_per_group = bufferTo4Bytes(buf, 32);
		frags_per_group = bufferTo4Bytes(buf, 36);
		inodes_per_group = bufferTo4Bytes(buf, 40);
		mtime = bufferTo4Bytes(buf, 44);
		wtime = bufferTo4Bytes(buf, 48);
		mnt_count = bufferTo2Bytes(buf, 52);
		max_mnt_count = bufferTo2Bytes(buf, 54);
		magic = bufferTo2Bytes(buf, 56);
		//std::cout << "SB MAGIC NUMBER: " << magic << endl;
		state = bufferTo2Bytes(buf, 58);
		errors = bufferTo2Bytes(buf, 60);
		minor_rev_level = bufferTo2Bytes(buf, 62);
		lastCheck = bufferTo4Bytes(buf, 64);
		checkInterval = bufferTo4Bytes(buf, 68);
		creator_os = bufferTo4Bytes(buf, 72);
		rev_level = bufferTo4Bytes(buf, 76);
		def_resuid = bufferTo2Bytes(buf, 80);
		def_resgid = bufferTo2Bytes(buf, 82);

		first_ino = bufferTo4Bytes(buf, 84);
		inode_size = bufferTo2Bytes(buf, 88);
		block_group_nr = bufferTo2Bytes(buf, 90);
		feature_compat = bufferTo4Bytes(buf, 92);
		feature_incompat = bufferTo4Bytes(buf, 96);
		feature_ro_compat = bufferTo4Bytes(buf, 100);

		for(int i = 0; i < 16; i++){
			uuid[i] = buf[104+i];
		}

		for(int i = 0; i < 16; i++){
			volume_name[i] = buf[120+i];
		}

		for(int i = 0; i < 64; i++){
			last_mounted[i] = buf[136+i];
		}

		algo_bitmap = bufferTo4Bytes(buf, 200);

		prealloc_blocks = buf[204];
		prealloc_dir_blocks = buf[205];

		for(int i = 0; i < 16; i++){
			journal_uuid[i] = buf[208+i];
		}

		journal_inum = bufferTo4Bytes(buf, 224);
		journal_dev = bufferTo4Bytes(buf, 228);
		last_orphan = bufferTo4Bytes(buf, 232);

		for(int i = 0; i < 4; i++){
			hash_seed[i] = bufferTo4Bytes(buf, 236 + (i*4));
		}

		def_hash_version = buf[252];
		default_mount_options = bufferTo4Bytes(buf, 256);
		first_meta_bg = bufferTo4Bytes(buf, 260);
	}

	//getters
	
	unsigned int superblock::get_inodes_count(){
		return inodes_count;
	}

	unsigned int superblock::get_blocks_count(){
		return blocks_count;
	}

	unsigned int superblock::get_r_blocks_count(){
		return r_blocks_count;
	}

	unsigned int superblock::get_free_blocks_count(){
		return free_blocks_count;
	}

	unsigned int superblock::get_free_inodes_count(){
		return free_inodes_count;
	}

	unsigned int superblock::get_first_data_block(){
		return first_data_block;
	}

	unsigned int superblock::get_log_block_size(){
		return log_block_size;
	}

	unsigned int superblock::get_block_size() {
		return block_size;
	}

	unsigned int superblock::get_log_frag_size(){
		return log_frag_size;
	}

	unsigned int superblock::get_blocks_per_group(){
		return blocks_per_group;
	}

	unsigned int superblock::get_block_groups_count() {
		return blocks_count / blocks_per_group + (blocks_count % blocks_per_group > 0);
	}

	unsigned int superblock::get_frags_per_group(){
		return frags_per_group;
	}

	unsigned int superblock::get_inodes_per_group(){
		return inodes_per_group;
	}

	unsigned int superblock::get_mtime(){
		return mtime;
	}

	unsigned int superblock::get_wtime(){
		return wtime;
	}

	unsigned short int superblock::get_mnt_count(){
		return mnt_count;
	}

	unsigned short int superblock::get_max_mnt_count(){
		return max_mnt_count;
	}

	unsigned short int superblock::get_magic(){
		return magic;
	}

	unsigned short int superblock::get_state(){
		return state;
	}

	unsigned short int superblock::get_errors(){
		return errors;
	}

	unsigned short int superblock::get_minor_rev_level(){
		return minor_rev_level;
	}

	unsigned int superblock::get_lastCheck(){
		return lastCheck;
	}

	unsigned int superblock::get_checkInterval(){
		return checkInterval;
	}

	unsigned int superblock::get_creator_os(){
		return creator_os;
	}

	unsigned int superblock::get_rev_level(){
		return rev_level;
	}

	unsigned short int superblock::get_def_resuid(){
		return def_resuid;
	}

	unsigned short int superblock::get_def_resgid(){
		return def_resgid;
	}

	unsigned int superblock::get_first_ino(){
		return first_ino;
	}

	unsigned short int superblock::get_inode_size(){
		return inode_size;
	}

	unsigned short int superblock::get_block_group_nr(){
		return block_group_nr;
	}

	unsigned int superblock::get_feature_compat(){
		return feature_compat;
	}

	unsigned int superblock::get_feature_incompat(){
		return feature_incompat;
	}

	unsigned int superblock::get_feature_ro_compat(){
		return feature_ro_compat;
	}

	uint8_t * superblock::get_uuid(int * length){
		// cant remember if arrays have a .length or not, so just hardcoded it.
		*length = 16;
		return &uuid[0];
	}

	uint8_t * superblock::get_volume_name(int * length){
		*length = 16;
		return &volume_name[0];
	}

	uint8_t * superblock::get_last_mounted(int * length){
		*length = 64;
		return &last_mounted[0];
	}

	unsigned int superblock::get_algo_bitmap(){
		return algo_bitmap;
	}

	uint8_t superblock::get_prealloc_blocks(){
		return prealloc_blocks;
	}

	uint8_t superblock::get_prealloc_dir_blocks(){
		return prealloc_dir_blocks;
	}

	uint8_t * superblock::get_journal_uuid(int * length){
		*length = 16;
		return &journal_uuid[0];
	}

	unsigned int superblock::get_journal_inum(){
		return journal_inum;
	}

	unsigned int superblock::get_journal_dev(){
		return journal_dev;
	}

	unsigned int superblock::get_last_orphan(){
		return last_orphan;
	}

	unsigned int * superblock::get_hash_seed(int * length){
		*length = 4;
		return &hash_seed[0];
	}

	uint8_t superblock::get_def_hash_version(){
		return def_hash_version;
	}

	unsigned int superblock::get_default_mount_options(){
		return default_mount_options;
	}

	unsigned int superblock::get_first_meta_bg(){
		return first_meta_bg;
	}
	// == operator for superblock. Takes a pointer to another superblock as the rhs value. Compares all data a superblock has and prints out what was wrong.
	bool superblock::operator==(const superblock* rhs){
		bool returnValue = true;
		if(this->inodes_count != rhs->inodes_count){
			std::cout << "inodes_count block wrong\n";
			returnValue = false;
		}
		if(this->blocks_count != rhs->blocks_count){
			std::cout << "blocks_count wrong.\n";
			returnValue = false;
		}
		if(this->r_blocks_count != rhs->r_blocks_count){
			std::cout << "r_blocks_count block wrong\n";
			returnValue = false;
		}
		if(this->free_blocks_count != rhs->free_blocks_count){
			std::cout << "free_blocks_count wrong\n";
			returnValue = false;
		}
		if(this->free_inodes_count != rhs->free_inodes_count){
			std::cout << "free_inodes_count wrong\n";
			returnValue = false;
		}
		if(this->first_data_block != rhs->first_data_block){
			std::cout << "First data block wrong\n";
			returnValue = false;
		}
		if(this->log_block_size != rhs->log_block_size){
			std::cout << "log block size wrong\n";
			returnValue = false;
		}
		if(this->block_size != rhs->block_size){
			std::cout << "block size wrong\n";
			returnValue = false;
		}
		if(this->log_frag_size != rhs->log_frag_size){
			std::cout << "log frag size wrong\n";
			returnValue = false;
		}

		if(this->blocks_per_group != rhs->blocks_per_group){
			std::cout << "blocks per group wrong\n";
			returnValue = false;
		}
		if(this->frags_per_group != rhs->frags_per_group){
			std::cout << "Frags per group wrong\n";
			returnValue = false;
		}
		if(this->inodes_per_group != rhs->inodes_per_group){
			std::cout << "inodes per group wrong\n";
			returnValue = false;
		}

		if(this->mtime != rhs->mtime){
			std::cout << "mtime wrong\n";
			returnValue = false;
		}
		if(this->wtime != rhs->wtime){
			std::cout << "wtime wrong\n";
			returnValue = false;
		}
		if(this->mnt_count != rhs->mnt_count){
			std::cout << "mnt_count wrong\n";
			returnValue = false;
		}
		if(this->max_mnt_count != rhs->max_mnt_count){
			std::cout << "max_mnt_count wrong\n";
			returnValue = false;
		}
		if(this->magic != rhs->magic){
			std::cout << "magic wrong\n";
			returnValue = false;
		}

		if(this->state != rhs->state){
			std::cout << "state wrong\n";
			returnValue = false;
		}

		if(this->errors != rhs->errors){
			std::cout << "errors wrong\n";
			returnValue = false;
		}
		if(this->minor_rev_level != rhs->minor_rev_level){
			std::cout << "minor_rev_level wrong\n";
			returnValue = false;
		}
		if(this->lastCheck != rhs->lastCheck){
			std::cout << "lastCheck wrong\n";
			returnValue = false;
		}
		if(this->checkInterval != rhs->checkInterval){
			std::cout << "checkInterval wrong\n";
			returnValue = false;
		}
		if(this->creator_os != rhs->creator_os){
			std::cout << "creator_os wrong\n";
			returnValue = false;
		}
		if(this->rev_level != rhs->rev_level){
			std::cout << "rev_level wrong\n";
			returnValue = false;
		}
		if(this->def_resuid != rhs->def_resuid){
			std::cout << "def_resuid wrong\n";
			returnValue = false;
		}
		if(this->def_resgid != rhs->def_resgid){
			std::cout << "def_resgid wrong\n";
			returnValue = false;
		}

		if(this->first_ino != rhs->first_ino){
			std::cout << "first_ino wrong\n";
			returnValue = false;
		}

		if(this->inode_size != rhs->inode_size){
			std::cout << "inode_size wrong\n";
			returnValue = false;
		}
		if(this->block_group_nr != rhs->block_group_nr){
			std::cout << "block_group_nr wrong\n";
			returnValue = false;
		}
		if(this->feature_compat != rhs->feature_compat){
			std::cout << "feature_compat wrong\n";
			returnValue = false;
		}
		if(this->feature_incompat != rhs->feature_incompat){
			std::cout << "feature_incompat wrong\n";
			returnValue = false;
		}
		if(this->feature_ro_compat != rhs->feature_ro_compat){
			std::cout << "feature_incompat wrong\n";
			returnValue = false;
		}
		if(this->feature_ro_compat != rhs->feature_ro_compat){
			std::cout << "feature_ro_compat wrong\n";
			returnValue = false;
		}

		for(int i = 0; i < 16; i++){
			if(this->uuid[i] != rhs->uuid[i]){
				std::cout << "uuid wrong\n";
				returnValue = false;

			}
		}
		for(int i = 0; i < 16; i++){
			if(this->volume_name[i] != rhs->volume_name[i]){
				std::cout << "volume_name wrong\n";
				returnValue = false;
			}
		}
		for(int i = 0; i < 64; i++){
			if(this->last_mounted[i] != rhs->last_mounted[i]){
				std::cout << "last_mounted wrong\n";
				returnValue = false;
			}
		}

		if(this->algo_bitmap != rhs->algo_bitmap){
			std::cout << "algo_bitmap wrong\n";
			returnValue = false;
		}

		if(this->prealloc_blocks != rhs->prealloc_blocks){
			std::cout << "prealloc_blocks wrong\n";
			returnValue = false;
		}

		if(this->prealloc_dir_blocks != rhs->prealloc_dir_blocks){
			std::cout << "prealloc_dir_blocks wrong\n";
			returnValue = false;
		}
		for(int i = 0; i < 16; i++){
			if(this->journal_uuid[i] != rhs->journal_uuid[i]){
				std::cout << "journal_uuid wrong\n";
				returnValue = false;
			}
		}

		if(this->journal_inum != rhs->journal_inum){
			std::cout << "journal_inum wrong\n";
			returnValue = false;
		}
		if(this->journal_dev != rhs->journal_dev){
			std::cout << "journal_dev wrong\n";
			returnValue = false;
		}
		if(this->last_orphan != rhs->last_orphan){
			std::cout << "last_orphan wrong\n";
			returnValue = false;
		}
		for(int i = 0; i < 4; i++){
			if(this->hash_seed[i] != rhs->hash_seed[i]){
				std::cout << "hash_seed wrong\n";
				returnValue = false;
			}
		}

		if(this->def_hash_version != rhs->def_hash_version){
			std::cout << "def_hash_version wrong\n";
			returnValue = false;
		}
		if(this->default_mount_options != rhs->default_mount_options){
			std::cout << "default_mount_options wrong\n";
			returnValue = false;
		}
		if(this->first_meta_bg != rhs->first_meta_bg){
			std::cout << "first_meta_bg wrong\n";
			returnValue = false;
		}
		return returnValue;
	}

}