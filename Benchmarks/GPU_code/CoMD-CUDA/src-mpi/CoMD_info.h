#ifndef CoMD_info_hpp
#define CoMD_info_hpp

#define CoMD_VARIANT "CoMD-cuda"
#define CoMD_HOSTNAME "n02"
#define CoMD_KERNEL_NAME "'Linux'"
#define CoMD_KERNEL_RELEASE "'4.2.0-27-generic'"
#define CoMD_PROCESSOR "'x86_64'"

#define CoMD_COMPILER "'/usr/bin/mpicc'"
#define CoMD_COMPILER_VERSION "'gcc (Ubuntu 4.8.4-2ubuntu1~14.04.4) 4.8.4'"
#define CoMD_CFLAGS "'-std=c++11 -Wno-unused-result -DMAXATOMS=256 -I/usr/local/cuda-7.0//targets/x86_64-linux/include/thrust/system/cuda/detail/cub/ -DCOMD_DOUBLE -g -O5  -I/usr/local/cuda-7.0//targets/x86_64-linux/include/'"
#define CoMD_LDFLAGS "'-lm -L/usr/local/cuda-7.0//lib64 -lcudart'"

#endif
