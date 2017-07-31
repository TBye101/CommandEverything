#
# Copyright 2014-2015 NVIDIA Corporation. All rights reserved
#
INCLUDES=-I../../include

ifndef OS
 OS   := $(shell uname)
 HOST_ARCH := $(shell uname -m)
endif

ifeq ($(OS),Windows_NT)
    export PATH := $(PATH):../../libWin32:../../libx64
    LIBS= -lcuda -L ../../libWin32 -L ../../libx64 -lcupti
    OBJ = obj
else
    ifeq ($(OS), Darwin)
        export DYLD_LIBRARY_PATH := $(DYLD_LIBRARY_PATH):../../lib
        LIBS= -Xlinker -framework -Xlinker cuda -L ../../lib -lcupti
    else
        export LD_LIBRARY_PATH := $(LD_LIBRARY_PATH):../../lib:../../lib64
        LIBS= -lcuda -L ../../lib -L ../../lib64 -lcupti
    endif
    OBJ = o
endif

pc_sampling: pc_sampling.$(OBJ)
	nvcc -o $@ pc_sampling.$(OBJ) $(LIBS)

pc_sampling.$(OBJ): pc_sampling.cu
	nvcc -arch=sm_52 -lineinfo  -c $(INCLUDES) $<

run: pc_sampling
	./$<

clean:
	rm -f pc_sampling pc_sampling.$(OBJ)

