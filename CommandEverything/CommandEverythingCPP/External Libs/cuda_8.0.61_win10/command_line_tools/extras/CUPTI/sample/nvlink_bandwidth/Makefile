#
# Copyright 2015-2016 NVIDIA Corporation. All rights reserved.
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

nvlink_bandwidth: nvlink_bandwidth.$(OBJ)
	nvcc -o $@ nvlink_bandwidth.$(OBJ) $(LIBS)

nvlink_bandwidth.$(OBJ): nvlink_bandwidth.cu
	nvcc -c $(INCLUDES) $<

run: nvlink_bandwidth
	./$<

clean:
	rm -f nvlink_bandwidth nvlink_bandwidth.$(OBJ)

