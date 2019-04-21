#ifndef _GPU_REL_FEATURE_
#define _GPU_REL_FEATURE_
#include <vector>
#include <fstream>
#include <assert.h>
#include "instr_info.h"
#include "omp.h"

#define BATCH_SIZE 5000

class gpu_rel_feature
{
public:
 
  gpu_rel_feature();
  void calculator(vector<instr_info> &instr_info_set, ofstream &ou,bool signal0);

  ~gpu_rel_feature(void);
};

#endif 
