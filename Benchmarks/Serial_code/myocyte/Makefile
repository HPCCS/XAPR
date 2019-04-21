# Example
# target: dependencies
	# command 1
	# command 2
          # .
          # .
          # .
	# command n

# link objects(binaries) together
myocyte.out: main.o
	g++ main.o -lm -fopenmp -o myocyte.out

# compile main function file into object (binary)
main.o: main.cpp define.c ecc.c cam.c fin.c master.c embedded_fehlberg_7_8.c solver.c file.c timer.c
	g++ main.cpp -c -O3 -fopenmp -std=c++11

# delete all object files
clean:
	rm *.o myocyte.out output.txt
