#include "bgdt.h"
#include "block_group_descriptor.h"
#include <cstdint>
#include <iostream>

namespace INTCHK5806 {
	/*
		Constructor, takes a buffer filled with the data for a block group descriptor as well as an integer representing the number of block groups
		Constructs a block group descriptor object.
	 */
	bgdt::bgdt(uint8_t * buf, int nBG) {
		this->nBG = nBG;
		bgdarr = new block_group_descriptor*[nBG];
		for (int i = 0; i < nBG; i++) {
			//std::cout << "bg: " << i << std::endl;
			bgdarr[i] = new block_group_descriptor(buf+i*32);
		}
	}

	/*
		Deletes the pointers to the block group descriptors
	 */
	bgdt::~bgdt() {
		for (int i = 0; i < nBG; i++) {
			delete bgdarr[i];
		}
		delete bgdarr;
	}

	/*
		takes the id of a block group
		returns the block group descriptor for that group
	 */
	block_group_descriptor * bgdt::get_block_group_descriptor(int blockGroup) {
		//std::cout << "Fetching BGD: " << blockGroup << std::endl;
		return bgdarr[blockGroup];
	}

	/*
		outputs the details for the entire block group desriptor table
	 */
	void bgdt::print(){
		for(int i = 0; i < nBG; i++){
			std::cout << "Block Group " << i+1 << " Info:\n";
			bgdarr[i]->print();
		}
	}

	/*
		compares bgdt
	 */
	bool bgdt::operator==(const bgdt* rhs){
		bool returnValue = true;
		if(this->nBG != rhs->nBG){
			std::cout << "Number of Block Groups Wrong!\n";
			returnValue = false;
		}

		for(int i = 0; i < nBG; i++){
			if(!(*(this->bgdarr[i]) == rhs->bgdarr[i])){
				std::cout << "Block Group Descriptor " << i << " is wrong\n";
				returnValue = false;
			}
		}

		return returnValue;
	}
}