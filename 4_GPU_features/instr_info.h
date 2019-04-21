#ifndef _INSTR_INFO_H_
#define _INSTR_INFO_H_

#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>

using namespace std;

struct oprd_line{
	string arguId;
	int sizeOfArgu;
	double dynValue=0;
	bool regORnot=false;
	string regName;
	string pre_block_id;
};

class instr_info{
public:
	// the operand lines
	vector<oprd_line> oprd_line_set;
	
	// the first line
	string blockId;
	int lineId;
	string funcName;
	string basicBlockId;
	string staticInstId;
	int opcodeId;
	int dynInstId;
};

#endif
