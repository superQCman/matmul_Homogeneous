# Project environment
# SIMULATOR_ROOT, defined by setup_env.sh
BENCHMARK_ROOT=$(SIMULATOR_ROOT)/benchmark/matmul_Homogeneous

# Compiler environment of C/C++
CC=g++
CFLAGS=-Wall -Werror -g -I$(SIMULATOR_ROOT)/interchiplet/includes
INTERCHIPLET_C_LIB=$(SIMULATOR_ROOT)/interchiplet/lib/libinterchiplet_c.a

# C/C++ Source file
C_SRCS=matmul.cpp 
C_OBJS=obj/matmul.o
C_TARGET=bin/matmul_c

C_kernel_SRCS=kernel.cpp 
C_kernel_OBJS=obj/kernel.o
C_kernel_TARGET=bin/kernel_c


all: bin_dir obj_dir C_target C_kernel_target

# C language target
C_target: $(C_OBJS)
	$(CC) $(C_OBJS) $(INTERCHIPLET_C_LIB) -o $(C_TARGET)

C_kernel_target: $(C_kernel_OBJS)
	$(CC) $(C_kernel_OBJS) $(INTERCHIPLET_C_LIB) -o $(C_kernel_TARGET)


# Rule for C object
obj/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@


# Directory for binary files.
bin_dir:
	mkdir -p bin

# Directory for object files for C.
obj_dir:
	mkdir -p obj

run:
	../../interchiplet/bin/interchiplet ./matmul.yml 
# Clean generated files.
clean:
	rm -rf bench.txt delayInfo.txt buffer* message_record.txt
	rm -rf proc_r*_t* *.log
	rm -rf obj cuobj bin
