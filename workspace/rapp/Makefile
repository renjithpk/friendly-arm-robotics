all: appbin server 
CFLAGS=-I./inc
INC_DIR = ./inc
SRC_DIR = ./src

######### ARM #####################################################################################
CC_ARM=/home/renjith/ARM/arm-hf-gcc/bin/arm-bcm2708hardfp-linux-gnueabi-g++
ARM_OBJ_DIR = ./obj_arm
LIBS=-lpthread 
_DEPS = wheel.h thread.h engine.h ioport.h types.h epoll.h tcpsocket.h stateMachine.h appFW.h terminal.h camera.h
DEPS = $(patsubst %,$(INC_DIR)/%,$(_DEPS))

_OBJ = thread.o roboApp.o wheel.o ioport.o engine.o  epoll.o tcpsocket.o stateMachine.o appFW.o terminal.o rtimer.o camera.o
OBJ = $(patsubst %,$(ARM_OBJ_DIR)/%,$(_OBJ))


$(ARM_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CC_ARM) -g -c -o $@ $< $(CFLAGS)

appbin: $(OBJ)
	$(CC_ARM) -g -o $@ $^ $(CFLAGS) $(LIBS)


######### HOST  #####################################################################################
LIBS_HOST=-lpthread -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -lX11 
HOST_OBJ_DIR = ./obj_h
CC_HOST=g++
CFLAGS_HOST=-I./inc

_DEPS_H = thread.h types.h epoll.h tcpsocket.h rtimer.h 
DEPS_H = $(patsubst %,$(INC_DIR)/%,$(_DEPS_H))

_OBJ_H = rserver.o cvInterface.o thread.o epoll.o tcpsocket.o  terminal.o rtimer.o
OBJ_H = $(patsubst %,$(HOST_OBJ_DIR)/%,$(_OBJ_H))

$(HOST_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS_H)
	$(CC_HOST) -g -c -o $@ $< $(CFLAGS_HOST)

server: $(OBJ_H)
	$(CC_HOST) -g -o $@ $^ $(CFLAGS_HOST) $(LIBS_HOST)

.PHONY: clean
clean:
	rm -rf $(HOST_OBJ_DIR)/*.o $(ARM_OBJ_DIR)/*.o server appbin
