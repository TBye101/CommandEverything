#
# Copyright 2013-2015 NVIDIA Corporation. All rights reserved
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

ifeq ($(OS), Darwin)
unified_memory:
	@echo "Unified memory is not supported on MacOSX. Build/clean/run aborted."
else
unified_memory: unified_memory.$(OBJ)
	nvcc -o $@ unified_memory.$(OBJ) $(LIBS)

unified_memory.$(OBJ): unified_memory.cu
	nvcc -c $(INCLUDES) $<

run: unified_memory
	./$<

clean:
	rm -f unified_memory unified_memory.$(OBJ)
endif

