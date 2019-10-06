#gcc -o tdsr_main  m1.c m2.c m3.c  -L. -I.  -lm -g  -lnewt 
#source file
#源文
SOURCE  := $(wildcard *.c) $(wildcard *.cpp) 
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

COMM_SOURCE  := $(wildcard ../common/*.c) $(wildcard ../common/*.cpp) 
COMM_OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(COMM_SOURCE)))

#target you can change test to what you want
#目标文件名，输入任意你想要的执行文件名
TARGET  := DAEMON

#compile and lib parameter
#编译参数
CC      :=g++
LIBS    :=
LDFLAGS :=
DEFINES :=-lpthread -lcrypto
INCLUDE := -I  ../common/
CFLAGS  := -g -Wall  -O0 $(DEFINES) $(INCLUDE)  
CXXFLAGS:= $(CFLAGS) -DHAVE_CONFIG_H


#下面的基本上不需要做任何改动了
.PHONY :  objs clean 


all : $(TARGET)

objs : $(OBJS) $(COMM_OBJS)

clean :
	rm -fr *.o
	rm -fr *.so
	rm -rf $(TARGET)
	rm -fr ../common/*.o

$(TARGET) : $(OBJS) $(COMM_OBJS)
	$(CC) $(CXXFLAGS) -o $@ $(OBJS) $(COMM_OBJS)
