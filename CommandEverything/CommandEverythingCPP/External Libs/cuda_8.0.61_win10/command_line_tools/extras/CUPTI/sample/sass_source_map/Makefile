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

sass_source_map: sass_source_map.$(OBJ)
	nvcc -o $@ sass_source_map.$(OBJ) $(LIBS)

sass_source_map.$(OBJ): sass_source_map.cu
	nvcc -arch=sm_20 -lineinfo  -c $(INCLUDES) $<

run: sass_source_map
	./$<

clean:
	rm -f sass_source_map sass_source_map.$(OBJ)
