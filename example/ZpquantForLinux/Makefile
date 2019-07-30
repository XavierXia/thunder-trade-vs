## ==> make -f Makefile

TARGET := zpquant_client_main_demo
SRCS := $(wildcard *.cpp)  
OBJS := $(patsubst %cpp,%o,$(SRCS))

INC_DIR   =  ../redox_reader
INC_DIR1   =  ./

CC_CFLAGS = -g -Wall -std=c++11 -fno-strict-aliasing
CC_LFLAGS = -std=c++11 -lredox -lev -lhiredis -lpthread -lm

all : $(TARGET) cleanObjs

$(TARGET) : $(OBJS)
	g++ -o $(TARGET) $(OBJS) $(CC_LFLAGS)

%.o : %.cpp
	g++ $(CC_CFLAGS) -I$(INC_DIR) -I$(INC_DIR1) -o $@ -c $< 

.PHONY : cleanObjs
cleanObjs:
	rm -f $(OBJS)

.PHONY : clean
clean:
	rm -f $(OBJS) $(TARGET)