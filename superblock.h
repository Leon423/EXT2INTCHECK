#ifndef superblock_H
#define superblock_H
#include <stdint.h>

namespace INTCHK5806 {
	/*
		Representation of a superblock
	 */
	class superblock
	{
	private:
		unsigned int inodes_count;
		unsigned int blocks_count;
		unsigned int r_blocks_count;
		unsigned int free_blocks_count;
		unsigned int free_inodes_count;
		unsigned int first_data_block;
		unsigned int log_block_size;
		unsigned int block_size;
		unsigned int log_frag_size;
		unsigned int blocks_per_group;
		unsigned int frags_per_group;
		unsigned int inodes_per_group;
		unsigned int mtime;
		unsigned int wtime;

		unsigned short int mnt_count;
		unsigned short int max_mnt_count;
		unsigned short int magic;
		unsigned short int state;
		unsigned short int errors;
		unsigned short int minor_rev_level;

		unsigned int lastCheck;
		unsigned int checkInterval;
		unsigned int creator_os;
		unsigned int rev_level;
		unsigned short int def_resuid;
		unsigned short int def_resgid;

		// EXT2 DYNAMIC REV Specific
		unsigned int first_ino;
		unsigned short int inode_size;
		unsigned short int block_group_nr;
		unsigned int feature_compat;
		unsigned int feature_incompat;
		unsigned int feature_ro_compat;

		uint8_t uuid[16];
		uint8_t volume_name[16];
		uint8_t last_mounted[64];
		unsigned int algo_bitmap;

		// Performance Hints
		uint8_t prealloc_blocks;
		uint8_t prealloc_dir_blocks;
		// 2 byte gap after ^
		// Journaling Support
		uint8_t journal_uuid[16];
		unsigned int journal_inum;
		unsigned int journal_dev;
		unsigned int last_orphan;

		//Directory Indexing Support
		unsigned int hash_seed[4];
		uint8_t def_hash_version;
		//3 bytes for padding.

		//Other Options
		unsigned int default_mount_options;
		unsigned int first_meta_bg;

		// 760 unused bytes.



	public:
		superblock(uint8_t *);

		unsigned int get_inodes_count();
		unsigned int get_blocks_count();
		unsigned int get_r_blocks_count();
		unsigned int get_free_blocks_count();
		unsigned int get_free_inodes_count();
		unsigned int get_first_data_block();
		unsigned int get_log_block_size();
		unsigned int get_block_size();
		unsigned int get_log_frag_size();
		unsigned int get_blocks_per_group();
		unsigned int get_block_groups_count();
		unsigned int get_frags_per_group();
		unsigned int get_inodes_per_group();
		unsigned int get_mtime();
		unsigned int get_wtime();

		unsigned short int get_mnt_count();
		unsigned short int get_max_mnt_count();
		unsigned short int get_magic();
		unsigned short int get_state();
		unsigned short int get_errors();
		unsigned short int get_minor_rev_level();

		unsigned int get_lastCheck();
		unsigned int get_checkInterval();
		unsigned int get_creator_os();
		unsigned int get_rev_level();
		unsigned short int get_def_resuid();
		unsigned short int get_def_resgid();

		// EXT2 DYNAMIC REV Specific
		unsigned int get_first_ino();
		unsigned short int get_inode_size();
		unsigned short int get_block_group_nr();
		unsigned int get_feature_compat();
		unsigned int get_feature_incompat();
		unsigned int get_feature_ro_compat();
		// parameters in these used to give the array length back as well.
		uint8_t * get_uuid(int *);
		uint8_t * get_volume_name(int *);
		uint8_t * get_last_mounted(int *);
		unsigned int get_algo_bitmap();

		// Performance Hints
		uint8_t get_prealloc_blocks();
		uint8_t get_prealloc_dir_blocks();
		// 2 byte gap after ^
		// Journaling Support
		uint8_t * get_journal_uuid(int *);
		unsigned int get_journal_inum();
		unsigned int get_journal_dev();
		unsigned int get_last_orphan();

		//Directory Indexing Support
		unsigned int * get_hash_seed(int *);
		uint8_t get_def_hash_version();
		//3 bytes for padding.

		//Other Options
		unsigned int get_default_mount_options();
		unsigned int get_first_meta_bg();

		bool operator==(const superblock* rhs);
	};
}

#endif