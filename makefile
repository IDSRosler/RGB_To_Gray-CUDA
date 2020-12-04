##########################################################
## NVCC COMPILER OPTIONS ##

NVCC=nvcc

##########################################################
## OPENCV ##

OPENCV = `pkg-config opencv4 --cflags --libs`

##########################################################
## Project file structure ##

# Source file directory:
SRC_DIR = src

##########################################################
## EXE ##

EXE = rgb_to_gray

##########################################################
##	Compiling

default:
	$(NVCC) $(SRC_DIR)/kernel.cu $(SRC_DIR)/main.cpp -o $(EXE) $(OPENCV)

clean:
	rm $(EXE)

##########################################################