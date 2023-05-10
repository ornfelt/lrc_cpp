# Variables to control Makefile operation
CC=g++
CFLAGS=-I. -Wall -g

SRC = main.cpp Lrc.cpp LrcTesting.cpp
OBJ = ${SRC:.cpp=.o}

# The build target 
TARGET = lrc
all: $(TARGET)

# Targets needed to bring the executable up to date
$(TARGET): ${OBJ}
	$(CC) $(CFLAGS) -o $(TARGET) ${OBJ}

clean:
	$(RM) $(TARGET)
	# Optionally remove object files...
	$(RM) *.o

install: $(TARGET)
