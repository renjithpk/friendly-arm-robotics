all: appbin server 

CC=/home/renjith/ARM/arm-hf-gcc/bin/arm-bcm2708hardfp-linux-gnueabi-g++

CFLAGS=-I./inc
LIBS=-lpthread 
BIN_DIR = ./obj_arm
INC_DIR = ./inc
SRC_DIR = ./src
_DEPS = wheel.h thread.h engine.h ioport.h types.h epoll.h tcpsocket.h stateMachine.h appFW.h terminal.h
DEPS = $(patsubst %,$(INC_DIR)/%,$(_DEPS))

_OBJ = thread.o roboApp.o wheel.o ioport.o engine.o  epoll.o tcpsocket.o stateMachine.o appFW.o terminal.o
OBJ = $(patsubst %,$(BIN_DIR)/%,$(_OBJ))


$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

appbin: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) $(LIBS)

#g++ -g -o server rserver.cpp tcpsocket.cpp thread.cpp epoll.cpp cvInterface.cpp -I../inc -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -lpthread -lX11

LIBS_H=-lpthread -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -lX11 
BIN_DIR_H = ./obj_h
CC_HOST=g++
CFLAGS_H=-I./inc

_DEPS_H = thread.h types.h epoll.h tcpsocket.h 
DEPS_H = $(patsubst %,$(INC_DIR)/%,$(_DEPS_H))

_OBJ_H = rserver.o cvInterface.o thread.o epoll.o tcpsocket.o  terminal.o
OBJ_H = $(patsubst %,$(BIN_DIR_H)/%,$(_OBJ_H))

$(BIN_DIR_H)/%.o: $(SRC_DIR)/%.cpp $(DEPS_H)
	$(CC_HOST) -g -c -o $@ $< $(CFLAGS_H)

server: $(OBJ_H)
	$(CC_HOST) -g -o $@ $^ $(CFLAGS_H) $(LIBS_H)

.PHONY: clean
clean:
	rm -rf $(BIN_DIR_H)/*.o $(BIN_DIR)/*.o server appbin
