#
# Copyright 2014-2015 NVIDIA Corporation. All rights reserved.
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

event_multi_gpu: event_multi_gpu.$(OBJ)
	nvcc -o $@ event_multi_gpu.$(OBJ) $(LIBS)

event_multi_gpu.$(OBJ): event_multi_gpu.cu
	nvcc -c $(INCLUDES) $<

run: event_multi_gpu
	./$<

clean:
	rm -f event_multi_gpu event_multi_gpu.$(OBJ)

