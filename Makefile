# -----------------------------------------------
# Makefile para el proyecto de cifrado y descifrado con AES-256 en C/C++
# Compila el código en C y C++ usando los compiladores g++ y gcc
# Incluye las librerías de OpenSSL (libcrypto y libssl)
# -----------------------------------------------

# Nombre del ejecutable
TARGET = main

# Compilador para archivos C++ y sus flags
CXX = g++
# Flags para g++: 
# -Wall: muestra todas las advertencias
# -std=c++17: usa el estándar C++17
# -O2: optimización del código
CXXFLAGS = -Wall -std=c++17 -O2

# Compilador para archivos C y sus flags
CC = gcc
# Flags para gcc:
# -Wall: muestra todas las advertencias
# -O2: optimización del código
CFLAGS = -Wall -O2

# Librerías y paths
# Bibliotecas de OpenSSL necesarias para operaciones criptográficas
LIBS = -lcrypto -lssl
# Directorio donde se encuentran las bibliotecas necesarias
LDFLAGS = -L/usr/local/lib

# Archivos fuente de C++ y C
SRCS_CPP = main.cpp           # Archivos fuente C++
SRCS_C = keygen.c utils.c      # Archivos fuente C

# Definición de los archivos objeto (resultado de la compilación de los fuentes)
OBJS_CPP = $(SRCS_CPP:.cpp=.o)  # Archivos .o generados desde .cpp
OBJS_C = $(SRCS_C:.c=.o)        # Archivos .o generados desde .c

# -----------------------------------------------
# Regla principal: compilar todo
# -----------------------------------------------
all: $(TARGET)   # La regla principal es generar el archivo ejecutable

# Regla para crear el ejecutable a partir de los archivos objeto
$(TARGET): $(OBJS_CPP) $(OBJS_C)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)
	# $(CXX): Compilador C++
	# $(CXXFLAGS): Flags de compilación
	# -o $@: Crear el ejecutable (TARGET)
	# $^: Archivos objeto
	# $(LDFLAGS): Flags de vinculación para buscar librerías
	# $(LIBS): Librerías de OpenSSL necesarias

# -----------------------------------------------
# Reglas para compilar archivos .cpp y .c a .o
# -----------------------------------------------

# Regla para compilar archivos .cpp en archivos .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
	# $<: Nombre del archivo fuente .cpp
	# $@: Nombre del archivo objeto (.o)

# Regla para compilar archivos .c en archivos .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	# $(CC): Compilador C
	# $(CFLAGS): Flags de compilación para C
	# -c: Compilar pero no enlazar
	# $<: Nombre del archivo fuente .c
	# $@: Nombre del archivo objeto (.o)

# -----------------------------------------------
# Regla para limpiar los archivos generados
# -----------------------------------------------
clean:
	rm -f $(TARGET) *.o  # Elimina el ejecutable y todos los archivos objeto

# -----------------------------------------------
# PHONY: "all" y "clean" no generan archivos, por lo que se marcan como .PHONY
# -----------------------------------------------
.PHONY: all clean
