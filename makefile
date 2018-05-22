CC=g++

SRC_FILES=$(wildcard *.cpp)
OBJ_FILES=$(patsubst %.cpp,%.o,$(SRC_FILES))
%DIR = /home/lt/program/cudd-3.0.0
M4RI_PATH=/home/lt/program/m4ri-20130416
OBJ_LINK_FILES = $(M4RI_PATH)/brilliantrussian.o $(M4RI_PATH)/echelonform.o $(M4RI_PATH)/graycode.o \
	 $(M4RI_PATH)/misc.o $(M4RI_PATH)/mmc.o \
	$(M4RI_PATH)/mzd.o $(M4RI_PATH)/mzp.o $(M4RI_PATH)/ple.o \
	$(M4RI_PATH)/ple_russian.o $(M4RI_PATH)/solve.o $(M4RI_PATH)/strassen.o \
	$(M4RI_PATH)/triangular.o $(M4RI_PATH)/triangular_russian.o
CFLAGS = -c -O3 -I/$(M4RI_PATH) 
LDFLAG= -lpthread 
TARGET= keccak400

all:
	$(CC) $(CFLAGS) $(SRC_FILES) 
	$(CC) -o $(TARGET)  $(OBJ_FILES) $(OBJ_LINK_FILES) $(LDFLAG)
.PHONY: clean
clean:
	rm $(OBJ_FILES) $(TARGET) -rf
