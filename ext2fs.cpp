#include "ext2fs.h"
#include "vdi_file.h"
#include "superblock.h"
#include "bgdt.h"
#include "inode.h"
#include "dirEntry.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <unistd.h>

namespace INTCHK5806 {
	/*
		Contructs an extfs from the vdi file at the provided path
		initializes all the internal objects of an ext2fs
	 */
	ext2fs::ext2fs (std::string path) {
		this->dirCount = 0;
		this->fileCount = 0;
		this->fsDirCount = 0;
		this->fsFileCount = 0;
		this->vdi = new vdi_file(path);
		this->vdi->vdi_open();
		this->mSB = fetch_superblock();
		initBitmaps();
		push_back_block(mSB->get_first_data_block());
		this->mBgdt = fetch_bgdt(1024/mSB->get_block_size()+1);
		initBlockGroups();
		inode * rI = fetch_inode(2);
		//in_use_inodes.push_back(2);
		dirCount++;
		parseDirectory(rI);
		inode * hI;
		for (int i = 1; i < mSB->get_first_ino(); i++) {
			//std::cout << "Expanding hidden inode: " << i << std::endl;
			hI = fetch_inode(i);
			countFileBlocks(hI);
		}

		//std::cout << "file: " << fileCount << " dirCount: " << dirCount << std::endl;
		for (int i = 0; i < mSB->get_block_groups_count(); i++) {
			fsDirCount += mBgdt->get_block_group_descriptor(i)->get_used_dirs_count();
		}
		fsFileCount += mSB->get_inodes_count() - mSB->get_free_inodes_count() - 9 - fsDirCount;
		//std::cout << "fsFileCount: " << fsFileCount << " fsDirCount: " << fsDirCount << std::endl;

	}

	/*
		delete all pointer member vars
	 */
	ext2fs::~ext2fs() {
		for (int i = 0; i < mSB->get_block_groups_count(); i++) {
			if (cSB[i] != NULL) delete cSB[i];
			if (cBgdt[i] != NULL) delete cBgdt[i];
			delete inode_tables[i];
		}
		delete inodes;
		delete dbs;
		delete fsInodes;
		delete fsDbs;
		delete inode_tables;
		delete mBgdt;
		delete mSB;
		vdi->vdi_close();
		delete vdi;
	}

	/*
		returns a pointer to the master suberblock
	 */
	superblock * ext2fs::fetch_superblock() {
		uint8_t * buf = new uint8_t[1024];
		vdi->vdi_read(buf,1024,1024);
		superblock * tmpSB = new superblock(buf);
		return tmpSB;
	}

	/*
		returns a pointer to a superblock
	 */
	superblock * ext2fs::fetch_superblock(int blockID) {
		uint8_t * buf = new uint8_t[mSB->get_block_size()];
		fetch_block(buf,blockID);
		return new superblock(buf);
	}

	/*
		reads a specified block of data into the buffer
	 */
	void ext2fs::fetch_block(void * buf, int blockID) {
		if (blockID != 0) push_back_block(blockID);
		vdi->vdi_read(buf,mSB->get_block_size(),mSB->get_block_size()*blockID);
	}

	/*
		writes the buffer to the specified block of data
	 */
	void ext2fs::write_block(void * buf, int blockID) {
		vdi->vdi_write(buf,mSB->get_block_size(),mSB->get_block_size()*blockID);
	}

	/*
		returns a pointer to a bgdt starting at the specified block id
		will read in the necessary data starting at the specified block, if the 
		bgdt spans multiple blocks, will read in all those blocks as well
	 */
	bgdt * ext2fs::fetch_bgdt(int blockID) {
		int nBG = mSB->get_block_groups_count();
		int bgdtSize = nBG * 32;
		//std::cout << "BGDTSIZE: " << bgdtSize << std::endl;
		bgdtSize += bgdtSize % mSB->get_block_size();
		uint8_t * bgdtBuf = new uint8_t[bgdtSize];
		//std::cout << "BGDTSIZE: " << bgdtSize / mSB->get_block_size() << std::endl;
		for (int i = 0; i < bgdtSize / mSB->get_block_size(); i++) {
			fetch_block(bgdtBuf+i*mSB->get_block_size(),blockID+i);
		}
		return new bgdt(bgdtBuf,nBG);
	}

	/*
		returns a pointer to an inode table starting at the specified block id
		will read in the necessary data starting at the specified block, if the 
		inode table spans multiple blocks, will read in all those blocks as well
	 */
	inode_table * ext2fs::fetch_inode_table(int blockID) {
		int itSize = mSB->get_inodes_per_group()*128;
		//std::cout << blockID << " " << itSize << std::endl;
		itSize += itSize % mSB->get_block_size();
		uint8_t * itBuf = new uint8_t[itSize];
		for (int i = 0; i < itSize / mSB->get_block_size(); i++) {
			fetch_block(itBuf+i*mSB->get_block_size(),blockID+i);
		}
		return new inode_table(itBuf,mSB->get_inodes_per_group());
	}

	/*
		fetches the block of date pointed to by the id of the specified block in the i_block array in the provided inode
		takes a pointer to an inode object, an index into the i_block array, and a buffer to fill with the data
	 */
	void ext2fs::fetchBlockFromFile(inode * i, int b, uint8_t * buf) {
		//cout << b << endl;
		int ipb = mSB->get_block_size()/4;
		uint32_t * l;
		uint32_t * tmp = new uint32_t[mSB->get_block_size()/4];
		if(b < 12){
			l = i->get_i_block();
			//if (*l == 0) return 0;
			goto direct;
		}

		b -= 12;
		if(b < ipb){
			l = i->get_i_block() + 12;
			//if (*l == 0) return 0;
			goto single;
		}

		b -= ipb;

		if(b < ipb*ipb){
			l = i->get_i_block() + 13;
			//if (*l == 0) return 0;
			goto doub;
		}

		b -= ipb*ipb;

		l = i->get_i_block() + 14;
		//if (*l == 0) return 0;

		triple:
			//std::cout<< "triple " << l[b/ipb*ipb*ipb]  << " " << b << " " << (b/ipb*ipb*ipb) << endl;
			//if (l[b/ipb*ipb*ipb] == 0) return 0;
			fetch_block(tmp, l[b/(ipb*ipb*ipb)]);
			l = tmp;
			b = b % (ipb*ipb*ipb);
		doub:
			//std::cout<< "double " << l[b/ipb*ipb]  << " " << b << " " << b/ipb*ipb*ipb << endl;
			//if (l[b/ipb*ipb] == 0) return 0;
			fetch_block(tmp, l[b/(ipb*ipb)]);
			l = tmp;
			b = b % (ipb*ipb);
		single:
			//std::cout<< "single " << l[b/ipb] << " " << b << " " << b/ipb*ipb*ipb << endl;
			//if (l[b/ipb] == 0) return 0;
			fetch_block(tmp, l[b/ipb]);
			l = tmp;
			b = b % ipb;
		direct:
			//std::cout<< "direct " << l[b]  << " " << b << " " << b/ipb*ipb*ipb << endl;
			fetch_block(buf, l[b]);
			// TODO: USED CHECKS HERE;
	}

	/*
		takes a pointer to the inode of a directory
		recures through that directory and records all data blocks and inodes that are found
		also counts the number of files and directories encountered
	 */
	void ext2fs::parseDirectory(inode * dirI) {
		//fetch the block first block
		uint8_t * blBuf = new uint8_t[dirI->get_i_size()];
		int cursor = 0, iBIndex = 0;

		fetchBlockFromFile(dirI, iBIndex, (blBuf+cursor));

		dirEntry * entry = new dirEntry(blBuf+cursor);
		//std::cout << "Skipping...\n" << entry->get_name() << std::endl;
		cursor += entry->get_rec_len();
		entry = new dirEntry(blBuf+cursor);
		//std::cout << entry->get_name() << std::endl;
		cursor += entry->get_rec_len();
		
		while (cursor < dirI->get_i_size()) {
			//check if need to get next data block
			if ((cursor / mSB->get_block_size()) > iBIndex) {
				//get next data block
				iBIndex++;
				fetchBlockFromFile(dirI, iBIndex, (blBuf+cursor));
			}

			//get the next entry
			entry = new dirEntry(blBuf+cursor);
			cursor += entry->get_rec_len();

			if (entry->get_iNum() == 0) continue;

			//std::cout << entry->get_name() << std::endl;
			//get the inode for the entry
			inode * eI = fetch_inode(entry->get_iNum());

			//if dir, recurse
			if (eI->get_i_mode() & 0x4000) {
				dirCount++;
				parseDirectory(eI);
			}
	//		if file, add all its data blocks to in_use
			if (!entry->isDir()) {
				fileCount++;
				countFileBlocks(eI);
			}
		}
		//return;
	}

	/*
		returns a pointer to an inode object that represents the inode at the specified inode id
	 */
	inode * ext2fs::fetch_inode(int iID) {
		int 
			bgNum = (iID - 1) / mSB->get_inodes_per_group(),
			lINum = (iID - 1) % mSB->get_inodes_per_group();
		inodes[bgNum][lINum/8] |= (1 << (lINum % 8));

		return inode_tables[bgNum]->get_inode(lINum);
	}

	/*
		records all the data blocks on a file inode
	 */
	void ext2fs::countFileBlocks(inode * fI) {
		//get the number of blocks used by the file
		int blockCount = fI->get_i_size() / mSB->get_block_size() + ((fI->get_i_size() % mSB->get_block_size()) > 0);
		//loop for the number of blocks
		for (int i = 0; i < blockCount; i++) {
			//get the dbID
			fetchBlockFromFile(fI, i, NULL);
		}
	}

	/*
		initializes the data bitmaps used to record the data blocks and inodes, fills them with zero's initially
	 */
	void ext2fs::initBitmaps() {
		this->inodes = new uint8_t*[mSB->get_block_groups_count()];
		this->dbs = new uint8_t*[mSB->get_block_groups_count()];
		fsInodes = new uint8_t*[mSB->get_block_groups_count()];
		fsDbs = new uint8_t*[mSB->get_block_groups_count()];
		for (int i = 0; i < mSB->get_block_groups_count(); i++){
			inodes[i] = new uint8_t[mSB->get_inodes_per_group()/8];
			dbs[i] = new uint8_t[mSB->get_block_size()];
			fsInodes[i] = NULL;
			fsDbs[i] = NULL;
		}
		for (int a = 0; a < mSB->get_block_groups_count(); a++){
			for (int b = 0; b < mSB->get_block_size(); b++) {
				if (b < mSB->get_inodes_per_group()) inodes[a][b] = 0;
				dbs[a][b] = 0;
			}
		}
	}

	/*
		pushes a data block onto the data blocks bitmap
	 */
	void ext2fs::push_back_block(int blockID) {
		blockID -= mSB->get_first_data_block();
		int bg = blockID / mSB->get_blocks_per_group(), lBID = blockID % mSB->get_blocks_per_group();
		dbs[bg][lBID/8] |= (1 << (lBID % 8));
	}

	/*
		inits the block groups
		this entails, getting all the super block and block group descriptor copies (not the super copies),
		the inode bitmaps, the data block bitmaps and inode tables all stored in the filesystem
		all the blocks holding the structures are recorded accordingly.
	 */
	void ext2fs::initBlockGroups() {
		fsInodes = new uint8_t*[mSB->get_block_groups_count()];
		fsDbs = new uint8_t*[mSB->get_block_groups_count()];
		inode_tables = new inode_table*[mSB->get_block_groups_count()];
		cSB = new superblock*[mSB->get_block_groups_count()];
		cBgdt = new bgdt*[mSB->get_block_groups_count()];
		int nCopies =0;
		for (int i = 0; i < mSB->get_block_groups_count(); i++) {
			block_group_descriptor * bgd = mBgdt->get_block_group_descriptor(i);
			if (i > 0 && bgd->get_block_bitmap() != (((mSB->get_blocks_per_group()) * i) + 1)) {
				nCopies +=2;
				//std::cout << "Has copies! " << bgd->get_block_bitmap() << " " << (mSB->get_blocks_per_group() * i + 1) << std::endl;
				cSB[i] = fetch_superblock(mSB->get_blocks_per_group()* i + 1);
				cBgdt[i] = fetch_bgdt(i*mSB->get_blocks_per_group() + 2);
			} else {
				//std::cout << "No copies " << i << '\n';
				cSB[i] = NULL;
				cBgdt[i] = NULL;
			}
			//std::cout << "bg0: " << bgd->get_block_bitmap() << " " << bgd->get_inode_bitmap() << std::endl;
			fsInodes[i] = new uint8_t[mSB->get_inodes_per_group()/8];
			fsDbs[i] = new uint8_t[mSB->get_block_size()];
			inode_tables[i] = fetch_inode_table(bgd->get_inode_table());
			fetch_block(fsInodes[i],bgd->get_inode_bitmap());
			fetch_block(fsDbs[i],bgd->get_block_bitmap());
		}
	}

	// check magic number of the master superblock
	bool ext2fs::checkMagic(){
		if(mSB->get_magic() == 0xEF53)
			return true;
		return false;
	}

	// check all superblock copies against the master superblock and print out what is wrong with them.
	bool ext2fs::checkSBCopies(){
		bool returnValue = true;
		for(int i = 0; i < mSB->get_block_groups_count(); i++){
			if(cSB[i] != NULL){
				if(!(*mSB == cSB[i])){
					std::cout << "SUPERBLOCK in BG " << i << "is bad" << std::endl;
					returnValue = false;
				}
			}
		}
		/*
		for(int i = 0; i < 1; i++){
			if(!(*mSB == cSB[0][i])){
				return false;
			}
		}
	*/
		return returnValue;
	}

	// check all block group descriptor tables against master copy and print out what is wrong with them.
	bool ext2fs::checkBGDTCopies(){
		bool returnValue = true;
		for(int i = 0; i < mSB->get_block_groups_count(); i++){
			if(cBgdt[i] != NULL){
				if(!(*mBgdt == cBgdt[i])){
					std::cout << "BGDT " << i << " is wrong.\n";
					returnValue = false;
				}
			}
		}
		return returnValue;
	}

	// check inode bitmaps against the one in master superblock and print out whats wrong with them.
	bool ext2fs::checkInodeBitmaps(){
		bool returnVal = true;
		for(int i = 0; i < mSB->get_block_groups_count(); i++){
			for(int j = 0; j < mSB->get_inodes_per_group()/8; j++){
				if(fsInodes[i][j] != inodes[i][j]){
					for (int k = 0; k < 8; k++) {
						if ((fsInodes[i][j] & (1 << k)) && !(inodes[i][j] & (1 << k))) {
							std::cout << "Inode " << (i*mSB->get_inodes_per_group() + j*8 + k) << " marked as used but not reachable!\n";
						} else if ((inodes[i][j] & (1 << k)) && !(fsInodes[i][j] & (1 << k))) {
							std::cout << "Inode " << (i*mSB->get_inodes_per_group() + j*8 + k) << " reachable but not marked as used!\n";
						}
					}
					returnVal = false;
				}
			}
		}
		return returnVal;
	}
	// check all datablock bitmaps against copy in master superblock and print out what is wrong with them.
	bool ext2fs::checkDbBitmaps(){
		bool returnVal = true;
		for (int id = mSB->get_first_data_block(); id < mSB->get_blocks_count(); id++) {
			int bgid = (id-mSB->get_first_data_block()) / mSB->get_blocks_per_group();
			int lid = (id-mSB->get_first_data_block()) % mSB->get_blocks_per_group();
			if ((dbs[bgid][lid/8] & (1 << (lid % 8))) != (fsDbs[bgid][lid/8] & (1 << (lid % 8)))) {
				if ((fsDbs[bgid][lid/8] & (1 << (lid % 8))) && !(dbs[bgid][lid/8] & (1 << (lid % 8)))) {
					std::cout << "Data Block " << id << " marked as used but not reachable!\n";
				} else if ((dbs[bgid][lid/8] & (1 << (lid % 8))) && !(fsDbs[bgid][lid/8] & (1 << (lid % 8)))) {
					std::cout << "Data Block " << id << " reachable but not marked as used!\n";
				}
				returnVal = false;
			}	
		}
		return returnVal;
	}

	// check that filecount is correct
	bool ext2fs::checkFileCount(){
		return fsFileCount == fileCount;
	}

	//check that directory count is correct.
	bool ext2fs::checkDirCount(){
		return fsDirCount == dirCount;
	}

	// print needed information about the file system.
	void ext2fs::printInformation(){
		std::cout << "FileSystem Size in Bytes: " << (mSB->get_blocks_count())*mSB->get_block_size() << std::endl;

		std::cout << "Amount of space currently used: " << (mSB->get_blocks_count() - mSB->get_free_blocks_count()) * mSB->get_block_size() << std::endl;

		std::cout << "Number of possible files and directories: " << mSB->get_inodes_count() << std::endl;

		std::cout << "Number of existing files: " << fsFileCount << std::endl;

		std::cout << "Number of existing directories: " << fsDirCount << std::endl;

		std::cout << "Number of block groups: " << mSB->get_block_groups_count() << std::endl << std::endl;

		mBgdt->print();

		std::cout << "\nBlock Size in Bytes: " << mSB->get_block_size() << std::endl;

		std::cout << "State of File System: " << ((mSB->get_state() == 1) ? "Valid\n" : "Errors Detected\n");
	}
}
