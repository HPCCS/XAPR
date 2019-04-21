#ifndef _MEMORY_STRIDE_
#define _MEMORY_STRIDE_

#include <vector>


class mem_stride
{
public: 
  int line_num;
  float addr;
  vector<float> stride;
};

class trans_rate
{
public:
  int line_num;
  int opcode;
  float ini_addr;
  vector<float> addr_set; 
};


class br_div
{
public:
  int op_sz;
  float dyn_value;
};

#endif
