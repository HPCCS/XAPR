# Example
# target: dependencies
	# command 1
	# command 2
          # .
          # .
          # .
	# command n

# link objects(binaries) together
a.out:	main.o
	g++	main.o \
			-lm -fopenmp -o srad

# compile main function file into object (binary)
main.o: 	main.cpp \
				define.c \
				graphics.c
	g++	main.cpp \
			-std=c++11 -c -O3 -fopenmp

# delete all object files
clean:
	rm *.o srad
