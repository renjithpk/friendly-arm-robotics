CC=arm-linux-g++
CFLAGS=-I../inc
LIBS=-lpthread
BIN_DIR = ../bin
INC_DIR = ../inc
_DEPS = wheel.h Thread.h engine.h ioport.h types.h
DEPS = $(patsubst %,$(INC_DIR)/%,$(_DEPS))

_OBJ = Thread.o testApp.o wheel.o ioport.o engine.o 
OBJ = $(patsubst %,$(BIN_DIR)/%,$(_OBJ))

$(BIN_DIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

appbin: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
.PHONY: clean
clean:
	rm -rf *.o
