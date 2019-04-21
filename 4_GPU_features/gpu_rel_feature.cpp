/** _4instype_detection
 * there are four types of instructions
 * 1) Control fpow changing instructions
 * 2) Floating point instructions
 * 3) Integer instructions
 * 4) Memory-related instructions
 */

#include "gpu_rel_feature.h"
#include "memory_stride.h"
#include <map>

gpu_rel_feature::gpu_rel_feature(){

}

gpu_rel_feature::~gpu_rel_feature(void){

}

void gpu_rel_feature::calculator(vector<instr_info> &instr_info_set, ofstream &ou, bool signal0){
  vector<mem_stride> mem_stride_set;
  vector<trans_rate> trans_rate_set;
  map<int, int> trans_rate_window;
  vector<br_div> br_div_set;
  int sz = instr_info_set.size();
  int opid;
  int tmp_line_num = 0;
  float tmp_addr = 0;
  float tmp_ini_addr = 0;
  float tmp_stride = 0;
  int tmp_opcode = 0;
  int tmp_sz = 0;
  float tmp_val = 0;
  int oprd_sz = 0;
  float str =0;
  bool signal = false;
  bool signal2 = false;
  int stride_sz = 0;
  int even = 0;
  int odd = 0;
  int addr_set_sz = 0;
  int bank_conf_rate = 0;
  int mem_stride_sz = 0;
  int trans_rate_sz = 0;
  int num_window = 0;
  int br_div_sz = 0;
  int num_br_window = 0, num_br_div = 0;
  int first = 0, second = 0;
  double ave_trans_rate = 0;
  //printf("batch size is %d\n", sz);
/*
  #pragma omp parallel
  {   
    int tid = omp_get_thread_num();
    if(ttmp_line_num = instr_info_set[i].lineId;id==0){ 
        nthread = omp_get_num_threads();
        printf("the number of threads %d ...\n", nthread);
    }
  }
*/
//  #pragma omp parallel shared(instr_info_set)
  //{
  //  #pragma omp for
    for(int i=0;i<sz;i++){
      opid =  instr_info_set[i].opcodeId;
      if(opid==29){

	// acquire specific information 
	  tmp_line_num = instr_info_set[i].lineId;
	  oprd_sz = instr_info_set[i].oprd_line_set.size();
	  for(int s=0;s<oprd_sz;s++){
	    if(instr_info_set[i].oprd_line_set[s].arguId == "1"){
	      tmp_addr = instr_info_set[i].oprd_line_set[s].dynValue;
	    }
	    
	    if(instr_info_set[i].oprd_line_set[s].arguId == "3"){
	      tmp_stride = instr_info_set[i].oprd_line_set[s].dynValue;
	    }
	  }

	// push information back to mem_stride
	if(mem_stride_set.size()==0){
	  mem_stride a_mem_stride;
	  a_mem_stride.line_num = tmp_line_num;
	  a_mem_stride.addr = tmp_addr;
	  a_mem_stride.stride.push_back(tmp_stride); 
	  mem_stride_set.push_back(a_mem_stride);

        }
        else{
	  for(int j=0;j<mem_stride_set.size();j++){
	    if(mem_stride_set[j].line_num==tmp_line_num && mem_stride_set[j].addr==tmp_addr){
	      mem_stride_set[j].stride.push_back(tmp_stride);
	      signal = true;
	    }
	  }

	  if(!signal){
	    mem_stride a_mem_stride;
	    a_mem_stride.line_num = tmp_line_num;
	    a_mem_stride.addr = tmp_addr;
	    a_mem_stride.stride.push_back(tmp_stride); 
	    mem_stride_set.push_back(a_mem_stride);
	  }

	  signal = false;
        }
      } // end of if(opid==29)

      if(opid == 27 || opid == 28){
	// acquire specific information
	tmp_line_num = instr_info_set[i].lineId;
        tmp_opcode = instr_info_set[i].opcodeId;
	tmp_ini_addr = tmp_addr;
	tmp_addr = instr_info_set[i].oprd_line_set[0].dynValue;
	// push the specific information back to trans_rate_set
	if(trans_rate_set.size()==0){
	  trans_rate a_trans_rate;
	  a_trans_rate.line_num = tmp_line_num;
	  a_trans_rate.opcode = tmp_opcode;
	  a_trans_rate.ini_addr = tmp_ini_addr;
	  a_trans_rate.addr_set.push_back(tmp_addr);
	  trans_rate_set.push_back(a_trans_rate);
	}
	else{
	  for(int j=0;j<trans_rate_set.size();j++){
	    if(trans_rate_set[j].line_num == tmp_line_num && trans_rate_set[j].ini_addr == tmp_ini_addr && trans_rate_set[j].opcode == tmp_opcode){
	      trans_rate_set[j].addr_set.push_back(tmp_addr);
	      signal = true;
	    }
	  }

	  if(!signal){
	    trans_rate a_trans_rate;
	    a_trans_rate.line_num = tmp_line_num;
	    a_trans_rate.opcode = tmp_opcode;
	    a_trans_rate.ini_addr = tmp_ini_addr;
	    a_trans_rate.addr_set.push_back(tmp_addr);
	    trans_rate_set.push_back(a_trans_rate);
	  }

	  signal = false;
	}
      } // end of if opid==27,28


      if(opid == 2){
	oprd_sz = instr_info_set[i].oprd_line_set.size();
	if(oprd_sz > 2){
	  tmp_sz = instr_info_set[i].oprd_line_set[0].sizeOfArgu;
	  tmp_val = instr_info_set[i].oprd_line_set[0].dynValue;
	}
      } // end of opid ==2

 
      // push back info into br_div
      br_div a_br_div;
      a_br_div.op_sz = tmp_sz;
      a_br_div.dyn_value = tmp_val;
      br_div_set.push_back(a_br_div);


    } // end of new iter on instr_info 


      // Calculating average stride rate for instr windows
      mem_stride_sz = mem_stride_set.size();
      for(int i=0;i<mem_stride_sz;i++){
	stride_sz = mem_stride_set[i].stride.size();
	if (stride_sz<=3){
	  // free vector space
	  mem_stride_set[i].stride.clear();
	  continue;
	}
	for(int j=0;j<stride_sz-1;j++){
	  tmp_stride = mem_stride_set[i].stride[j+1]-mem_stride_set[i].stride[j]; 
	  if((int)tmp_stride%2==0){
	    even++;
	  }else
	  {
	    odd++;
	  }
	}
	// free vector space
	mem_stride_set[i].stride.clear();
      }

      // stride ratio of even (bank-conflict) over the entire space
      bank_conf_rate = even/(even+odd);
      
      // free space of mem_stride
      mem_stride_set.clear();


      // Calculating transaction rate per warp for instr windows
      int tmp;
      trans_rate_sz = trans_rate_set.size();
      for(int i=0;i<trans_rate_sz;i++){
	addr_set_sz = trans_rate_set[i].addr_set.size();
	for(int j=32;j<addr_set_sz;j+=32){
	  tmp = trans_rate_set[i].addr_set[j]-trans_rate_set[i].addr_set[j-32];
	  tmp = 1+tmp/128/8;
	  trans_rate_window[tmp] ++;
	  num_window ++;
	}
      } 
      
      // The average transacton rate across instr windows
      for(auto it=trans_rate_window.begin();it!=trans_rate_window.end();++it){
	first = it->first;
	second = it->second;
	ave_trans_rate += 1/first*second/num_window;
      }

      // free memory space
      for(int i=0;i<trans_rate_sz;i++){
	trans_rate_set[i].addr_set.clear();
      }
      trans_rate_set.clear();

      // count windows that have branch div
      br_div_sz = br_div_set.size();
      for(int i=0;i<br_div_sz;i++){
	// branch div: op_sz != dyn_value
	if(br_div_set[i].op_sz!=br_div_set[i].dyn_value){
	  // branch div found!
	  signal2 = true;
	}

	// every window of 32 decisions, we check if there is branch div
	if(i>0 && (i+1)%32==0){
	  // count the number of windows that have 32 decisions
	  num_br_window ++;
	  if(signal2 == true){
	    num_br_div ++;
	  }
	  signal2 = false;
	}
      }

      // free memory space
      br_div_set.clear();

/*
     if(((opid==28)&&(instr_info_set[i].oprd_line_set[0].regName=="az"||instr_info_set[i].oprd_line_set[1].regName=="az"))){
    	//assert(instr_info_set[i].oprd_line_set[0].arguId == "2" && "ERROR: choosing the wrong operand!");
        ou<<sh_num<<" "<<con_num<<" "<<tr_num<<" "<<ow_num<<" "<<cf_num<<" "<<fp_num<<" "<<in_num<<" "<<mem_num<<endl;
	cf_num=0;
	sh_num=0;
	tr_num=0;
	fp_num=0;
	con_num=0;
	in_num=0;
	mem_num=0;
      }
      if(signal){
        if(i==(sz-1)){
    	  //assert(instr_info_set[i].oprd_line_set[0].arguId == "2" && "ERROR: choosing the wrong operand!");
          ou<<sh_num<<" "<<con_num<<" "<<tr_num<<" "<<ow_num<<" "<<cf_num<<" "<<fp_num<<" "<<in_num<<" "<<mem_num<<endl;
	  cf_num=0;
	  sh_num=0;
	  tr_num=0;
	  fp_num=0;
	  con_num=0;
	  in_num=0;
	  mem_num=0;
	}
      }
    }// Old end of iter on instr_info
*/

  //} // end of pragma
}


