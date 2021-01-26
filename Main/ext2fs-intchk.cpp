#include <iostream>
#include "../ext2fs.h"
#include "../err_def.h"

using namespace INTCHK5806;
using namespace std;


/*
	takes the path to the vdi file as a command line argument
 */
int main (int argc, char *argv[]) {
	if (argc != 2) {
		cerr << "usage is: ext2fs-intchk <path_to_vdi>\n";
		exit(ERR_ARGS_COUNT);
	}
	
	ext2fs fs(argv[1]);
	/*
	bool temp = false;
	temp = fs.checkInodeBitmaps();
	cout << temp << endl;
	temp = false;
	temp = fs.checkDbBitmaps();
	cout << temp << endl;
	*/
	if(fs.checkInodeBitmaps()){
		cout << "InodeBitmaps are Correct\n";
	}else{
		cout << "InodeBitmaps are wrong\n";
	}

	if(fs.checkDbBitmaps()){
		cout << "DataBlockBitmaps are correct\n";
	}else{
		cout << "DataBlockBitmaps are wrong\n";
	}

	if(fs.checkSBCopies()){
		cout << "All copies of Superblock are correct.\n";
	} else{
		cout << "One or more copies of Superblock are incorrect\n";
	}

	if(fs.checkBGDTCopies()){
		cout << "All copies of the Block Group Descriptor Tables are correct.\n";
	}else{
		cout << "One or more copies of the Block Group Descriptor Tables are incorrect\n";
	}
	
	if(fs.checkMagic()){
		cout << "Magic number is correct.\n";
	}else{
		cout << "Master Superblock has the wrong magic number\n";
	}

	if(fs.checkDirCount()){
		cout << "Directory Count Right\n";
	}else{
		cout << "Directory Count Wrong\n";
	}

	if(fs.checkFileCount()){
		cout << "File Count right\n";
	}else{
		cout << "File Count Wrong\n";
	}

	fs.printInformation();

}
