# Nombre del ejecutable
TARGET = main

# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -O2
CC = gcc
CFLAGS = -Wall -O2

# Librerías y paths
LIBS = -lcrypto -lssl
LDFLAGS = -L/usr/local/lib

# Archivos fuente y objetos
SRCS_CPP = main.cpp
SRCS_C = keygen.c utils.c
OBJS_CPP = $(SRCS_CPP:.cpp=.o)
OBJS_C = $(SRCS_C:.c=.o)

# Reglas
all: $(TARGET)

$(TARGET): $(OBJS_CPP) $(OBJS_C)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
