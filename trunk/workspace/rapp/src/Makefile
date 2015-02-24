#CC=arm-linux-g++
CC=/home/renjith/ARM/arm-hf-gcc/bin/arm-bcm2708hardfp-linux-gnueabi-g++
CFLAGS=-I../inc
LIBS=-lpthread 
BIN_DIR = ../bin
INC_DIR = ../inc
_DEPS = wheel.h Thread.h engine.h ioport.h types.h Event.h epoll.h
DEPS = $(patsubst %,$(INC_DIR)/%,$(_DEPS))

_OBJ = Thread.o RoboApp.o wheel.o ioport.o engine.o Event.o Epoll.o my_socket.o
OBJ = $(patsubst %,$(BIN_DIR)/%,$(_OBJ))

$(BIN_DIR)/%.o: %.cpp $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

$(BIN_DIR)/appbin: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) $(LIBS)
.PHONY: clean
clean:
	rm -rf ../bin/*.o ../bin/appbin
