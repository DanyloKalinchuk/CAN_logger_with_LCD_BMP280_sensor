TARGET = cpp_can_logger

CC = gcc
CPP = g++
FLAGS = -O2 -c
LIBS = -li2c -lgpiod

SRC_C = $(shell find . -name '*.c')
SRC_CPP = $(shell find . -name '*.cpp')

OBJ_C = $(SRC_C:.c=.o)
OBJ_CPP = $(SRC_CPP:.cpp=.o)

final: $(TARGET)

$(TARGET): $(OBJ_C) $(OBJ_CPP)
	$(CPP) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(FLAGS) $< -o $@

%.o: %.cpp
	$(CPP) $(FLAGS) $< -o $@

clean:
	rm -f $(OBJ_C) $(OBJ_CPP)
