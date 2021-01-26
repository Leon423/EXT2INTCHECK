#ifndef EXT2_FS_H
#define EXT2_FS_H

#include "vdi_file.h"
#include "superblock.h"
#include "bgdt.h"
#include "inode.h"
#include "dirEntry.h"
#include <string>
#include <map>
#include <vector>

namespace INTCHK5806 {
class ext2fs
{
	private:
		vdi_file * vdi;										//pointer to the vdi file object holding this ext2fs
		superblock * mSB, ** cSB;							//pointer to the master superblock, pointer to an array of superblock pointers which represent the superblock copies
		bgdt * mBgdt, ** cBgdt;								//pointer to the master BGDT, pointer to an array of bgdt pointers which represent the bgdt copies
		int dirCount, fileCount;							//counts of the files and directories found
		inode_table ** inode_tables;						//pointer to an array of inode tables
		uint8_t ** inodes, **dbs, **fsInodes, **fsDbs;		//pointers to arrays of uint8_t buffers which store the bitmaps, fs stands for filesystem, meaning they store the filesystem copy of the bitmap
		int fsDirCount, fsFileCount;						//count of the reported files and dir in the filesystem
		void parseDirectory(inode *);
		inode * fetch_inode(int iID);
		void countFileBlocks(inode *);
		void initBitmaps();
		void initBlockGroups();
		void push_back_block(int);
	public:
		ext2fs(std::string path);
		~ext2fs();
		superblock * fetch_superblock();
		superblock * fetch_superblock(int);
		void fetch_block(void *, int);
		void write_block(void *, int);
		bgdt * fetch_bgdt(int);
		inode_table * fetch_inode_table(int);
		void testCount();
		void fetchBlockFromFile(inode *, int, uint8_t *);
		bool checkMagic();
		bool checkSBCopies();
		bool checkBGDTCopies();
		bool checkInodeBitmaps();
		bool checkDbBitmaps();
		bool checkDirCount();
		bool checkFileCount();
		void printInformation();
	};
}
#endif