# Nombre del ejecutable
TARGET = main

# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -O2
CC = gcc
CFLAGS = -Wall -O2

# Librerías y paths
LIBS = -lcrypto -lssl
INCLUDES = -I./include
LDFLAGS = -L/usr/local/lib

# Archivos fuente y objetos
SRCS_CPP = src/main.cpp
SRCS_C = src/keygen.c src/utils.c
OBJS_CPP = $(SRCS_CPP:.cpp=.o)
OBJS_C = $(SRCS_C:.c=.o)

# Reglas
all: $(TARGET)

$(TARGET): $(OBJS_CPP) $(OBJS_C)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

src/main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

src/keygen.o: src/keygen.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

src/utils.o: src/utils.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(TARGET) src/*.o

.PHONY: all clean
