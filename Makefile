
CC = g++
CFLAGS = -g
BIN_NATIVE = ec
OBJ_NATIVE = main.o

.SUFFIXES:
.SUFFIXES: .cpp .c .o .h

all: ec

$(BIN_NATIVE): $(OBJ_NATIVE)
	$(CC) -o $(BIN_NATIVE) $(OBJ_NATIVE) -lpthread

$(OBJ_NATIVE): main.cpp
	$(CC) $(CFLAGS) -c main.cpp
clean:
	/bin/rm -f *.o $(BIN_NATIVE)
