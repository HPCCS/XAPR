#export OMP_NUM_THREADS=16

EXE=gpu_rel_feature
CC = g++ -std=c++11 -I/usr/apps/gnu/4.9.3
C_INC = 
#CFLAGS =   -std=c++11 -Wall -O0 -g
CFLAGS = -std=c++11 -O3
#LDFLAGS+=-L/g/g90/guo7/graphviz/lib -lgvc -lcgraph  -lcdt
CLINK = ${CC} 
#CLINKFLAGS = -g 
#MY_LIB=/opt/pasa_lib/graphviz-2.40/lib/pkgconfig

SRCS = main.cpp  gpu_rel_feature.cpp

OBJS=$(patsubst %.cpp, %.o, $(SRCS))

$(EXE): main.o gpu_rel_feature.o
	$(CC) main.o gpu_rel_feature.o -fopenmp -std=c++11 -o $@ #$(LDFLAGS)

main.o: main.cpp instr_info.h gpu_rel_feature.h
	$(CC) -c  main.cpp -o $@ $(CFLAGS)

gpu_rel_feature.o: gpu_rel_feature.cpp
	$(CC) -c gpu_rel_feature.cpp -fopenmp -std=c++11 -o $@ $(CFLAGS)

clean:
	rm -f *.o ${EXE}
