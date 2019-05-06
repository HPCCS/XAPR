/* _4instypeing pattern detection
 * the main function
 */

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <time.h>
#include <string>
#include <stdlib.h>
#include <fstream>

#define BATCH_SIZE 1000000
#include "gpu_rel_feature.h"
#include "instr_info.h"
using namespace std;

int main(){

  vector<instr_info> instr_info_set;
  gpu_rel_feature a_gpu_rel_feature;

  // the file stream for dynamic trace
  ifstream in;
  string file(getenv("TRACE_FILE_NAME"));
  string infile="../DELIMITER_TRACE_FILES1/"+file;
  in.open(infile.c_str(), ifstream::in);
  ofstream ou;
  string outfile="../DELIMITER_RESULTS_DEADLOC1/"+file+"_gpu_rel_feature.txt";
  ou.open(outfile.c_str(), ofstream::out);

  // 
  if(!in){
    cout<<"ERROR IN OPENNING THE TRACEFILE!"<<endl;
    return 1;
  }

  // reading the dynamic trace into memory
  //
  bool signal=false;
  string line;
  int i=0, n=0;//, len;
  printf("Begin reading the instruction trace...\n");
  while(true){
    //read info from the trace file
    getline(in, line);
    size_t yes_no, NumOfComma1, NumOfComma2;
    yes_no = line.find(",",0);
    if (yes_no == string::npos){
      if(i>0){
	++n;
      }

      // dynamic trace processing batch by batch
      //if ((n>=an_arguConfig->NUMBER_OF_BLOCKS_PROCESS_PER_BATCH)||in.eof()){
      if (n>=BATCH_SIZE||in.eof()){
	//int k;
	// Stopping read of the trace, and starting trace analysis
	int len = instr_info_set.size();
	if(n<BATCH_SIZE){
	  signal=true;
	}
	a_gpu_rel_feature.calculator(instr_info_set, ou,signal);
	instr_info_set.clear();
	n = 0;
	instr_info an_instr_info;
	instr_info_set.push_back(an_instr_info);
	    printf("The Tool has completed the %d instructions...\n", instr_info_set[len-1].dynInstId);
/*
	for(k=0;k<len;k++){
	  int opcodeId = instr_info_set[k].opcodeId;
	  if (opcodeId < 0){
	    printf("Exception: disaster found %d!", instr_info_set[k].dynInstId);
	  }

	  /-* resilience pattern analysis
 	   * TODO: _4instypeing
 	   *-/
	  // the size of trace block: sz
	  //int sz = instr_info_set.size();
	  //printf("the size of trace block is: %d \n", sz);
	  
	  // _4instypeing pattern detection
	  a__4instype_detect.detector(instr_info_set, ou);

	  /-* resilience pattern detection DONE!
 	   *-/
           
	  // print out the execution states
	  if(instr_info_set[k].dynInstId%50000 == 0){
	    printf("The Tool has completed the %d instructions...\n", instr_info_set[k].dynInstId);
	  }

	  // start to free the memory space when 80% of the batch is hit
	  //if (k>=0.8*an_arguConfig->NUMBER_OF_BLOCKS_PROCESS_PER_BATCH){
	  if (k>=0.8*BATCH_SIZE){
	    instr_info_set.erase(instr_info_set.begin(), (instr_info_set.begin()+k));
	    n = instr_info_set.size();
	    instr_info an_instr_info;
	    instr_info_set.push_back(an_instr_info);
	    break;
	  }
	}
*/
	// Jump out of the loop while hitting the bottom of the file
	if (in.eof()){
	  //int n0 = instr_info_set.size();
	  //if (k >= n0-1){
	    break;
	  //}
	}
	continue;
      }else{
	// While 80% trace finished its analysis, stopping the analysis, and starting free the completed trace
	i = 0;
	instr_info an_instr_info;
	instr_info_set.push_back(an_instr_info);
	continue;
      }
      
    }else{
      NumOfComma1 = 0;
      NumOfComma2 = line.find(",", NumOfComma1);
      string blockId_abnom = line.substr(NumOfComma1, NumOfComma2).c_str();
      // assign to line-0/1/2/r
      if(blockId_abnom == "0") // assign to line 0
      {
	instr_info_set[n].blockId = line.substr(NumOfComma1, NumOfComma2).c_str();
	// the first field
	NumOfComma1 = NumOfComma2;
	NumOfComma2 = line.find(",", NumOfComma1 + 1);
	instr_info_set[n].lineId = atoi(line.substr(NumOfComma1+1, NumOfComma2-NumOfComma1-1).c_str());
	// the second field
	NumOfComma1 = NumOfComma2;
	NumOfComma2 = line.find(",", NumOfComma1 + 1);
	instr_info_set[n].funcName = line.substr(NumOfComma1+1, NumOfComma2-NumOfComma1-1).c_str();
	// the third field
	NumOfComma1 = NumOfComma2;
	NumOfComma2 = line.find(",", NumOfComma1 + 1);
	instr_info_set[n].basicBlockId = line.substr(NumOfComma1+1, NumOfComma2-NumOfComma1-1).c_str();
	// the fourth field
	NumOfComma1 = NumOfComma2;
	NumOfComma2 = line.find(",", NumOfComma1 + 1);
	instr_info_set[n].staticInstId = line.substr(NumOfComma1+1, NumOfComma2-NumOfComma1-1).c_str();
	// the fifth field
	NumOfComma1 = NumOfComma2;
	NumOfComma2 = line.find(",", NumOfComma1 + 1);
	instr_info_set[n].opcodeId = atoi(line.substr(NumOfComma1+1, NumOfComma2-NumOfComma1-1).c_str());
	// the sixth field
	NumOfComma1 = NumOfComma2;
	NumOfComma2 = line.find(",", NumOfComma1 + 1);
	instr_info_set[n].dynInstId = atoi(line.substr(NumOfComma1+1, NumOfComma2-NumOfComma1-1).c_str());
	//
	i++;
	continue;
      }else{
	oprd_line an_oprd_line;
	// the argueId
	an_oprd_line.arguId = line.substr(NumOfComma1, NumOfComma2).c_str();
	// the size of argue
	NumOfComma1 = NumOfComma2;
	NumOfComma2 = line.find(",", NumOfComma1 + 1);
	an_oprd_line.sizeOfArgu = atoi(line.substr(NumOfComma1+1, NumOfComma2-NumOfComma1-1).c_str());
	// the dynamic value
	NumOfComma1 = NumOfComma2;
	NumOfComma2 = line.find(",", NumOfComma1 + 1);
	an_oprd_line.dynValue = atof(line.substr(NumOfComma1+1, NumOfComma2-NumOfComma1-1).c_str());
	// is reg or not
	NumOfComma1 = NumOfComma2;
	NumOfComma2 = line.find(",", NumOfComma1 + 1);
	an_oprd_line.regORnot = (atoi(line.substr(NumOfComma1+1, NumOfComma2-NumOfComma1-1).c_str()) ==1);
	// the reg name
	NumOfComma1 = NumOfComma2;
	NumOfComma2 = line.find(",", NumOfComma1 + 1);
	an_oprd_line.regName = line.substr(NumOfComma1+1, NumOfComma2-NumOfComma1-1).c_str();
	// the pre-block id
	NumOfComma1 = NumOfComma2;
	NumOfComma2 = line.find(",", NumOfComma1 + 1);
	an_oprd_line.pre_block_id = line.substr(NumOfComma1+1, NumOfComma2-NumOfComma1-1).c_str();
 	// push back
 	instr_info_set[n].oprd_line_set.push_back(an_oprd_line);
	//
	i++;
	continue;
      }
    }
  }

// finishing reading, close the file stream
//printf("total number of _4instype operation is: %d \n",a__4instype_detect.num1);
//printf("total number of _4instype bits is: %d \n",a__4instype_detect.num);
//ou<<endl;
//ou<<"total number of _4instype operations is:" << a__4instype_detect.num1 <<endl;
//ou<<"total number of _4instype bits is:" << a__4instype_detect.num <<endl;
//ou<<"resilience of per _4instype pattern is:" << a__4instype_detect.num/a__4instype_detect.num1 <<endl;
ou<<endl;
in.close();
ou.close();

return 0;

}

