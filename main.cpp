/**
 * @file main.cpp
 * @brief Este archivo contiene las funciones principales para el cifrado y descifrado de archivos usando AES-256 en modo CTR.
 *        Las claves y el IV se generan de manera determinística utilizando un número de secuencia.
 *
 * @details Este programa implementa dos operaciones principales: cifrado y descifrado. Las funciones utilizan las bibliotecas de OpenSSL para
 *          generar claves AES-256 y vectores de inicialización (IV) basados en un número de secuencia. Las operaciones criptográficas se realizan 
 *          en bloques de 1MB para optimizar el rendimiento y evitar problemas de memoria al manejar archivos grandes.
 *
 * @author Astrovia
 * @date 2024-09-28
 */

#include <openssl/sha.h>  // Encabezado necesario para usar la función SHA256
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <openssl/evp.h>
#include "keygen.h"
#include "utils.h"

#define CHUNK_SIZE (1024 * 1024) // Definición del tamaño de los bloques de procesamiento (1MB)

/**
 * @brief Cifra un archivo utilizando AES-256 en modo CTR y guarda el resultado en un archivo de salida.
 *
 * @param input_path Ruta del archivo de entrada que se quiere cifrar.
 * @param output_path Ruta del archivo donde se guardará el archivo cifrado.
 * @param sequence_number Número de secuencia utilizado para generar la clave y el IV.
 */
void encrypt(const std::string& input_path, const std::string& output_path, int sequence_number) {
    std::cout << "Cifrando archivo: " << input_path << std::endl;
    std::cout << "Número de secuencia: " << sequence_number << std::endl;

    unsigned char key[32];  // Buffer para la clave AES-256
    unsigned char iv[16];   // Buffer para el IV (vector de inicialización)
    
    // Generar la clave y el IV de manera determinística usando el número de secuencia
    generate_symmetric_key_and_iv(key, iv, sequence_number);

    // Mostrar la clave generada
    std::cout << "Clave generada (256 bits) en cifrado: ";
    for (int i = 0; i < 32; i++) {
        printf("%02x", key[i]);
    }
    std::cout << std::endl;

    // Mostrar el IV generado
    std::cout << "IV generado en cifrado: ";
    for (int i = 0; i < 16; i++) {
        printf("%02x", iv[i]);
    }
    std::cout << std::endl;

    // Abrir el archivo de entrada para leer y el archivo de salida para escribir los datos cifrados
    FILE *input_file = fopen(input_path.c_str(), "rb");
    FILE *output_file = fopen(output_path.c_str(), "wb");
    if (!input_file || !output_file) {
        perror("Error al abrir archivos");
        exit(EXIT_FAILURE);
    }

    // Crear e inicializar el contexto de cifrado
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    // Iniciar el proceso de cifrado usando AES-256 en modo CTR
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv))
        handleErrors();

    unsigned char buffer_in[CHUNK_SIZE];  // Buffer para almacenar los datos leídos del archivo
    unsigned char buffer_out[CHUNK_SIZE + EVP_CIPHER_block_size(EVP_aes_256_ctr())];  // Buffer para los datos cifrados
    int len, cipher_len;

    // Leer el archivo de entrada por bloques y cifrar cada bloque
    while ((len = fread(buffer_in, 1, CHUNK_SIZE, input_file)) > 0) {
        if (1 != EVP_EncryptUpdate(ctx, buffer_out, &cipher_len, buffer_in, len))
            handleErrors();
        fwrite(buffer_out, 1, cipher_len, output_file);
    }

    // Finalizar el proceso de cifrado y escribir los últimos datos
    if (1 != EVP_EncryptFinal_ex(ctx, buffer_out + cipher_len, &len))
        handleErrors();
    fwrite(buffer_out + cipher_len, 1, len, output_file);

    // Liberar el contexto de cifrado y cerrar los archivos
    EVP_CIPHER_CTX_free(ctx);
    fclose(input_file);
    fclose(output_file);

    std::cout << "Cifrado completado: " << output_path << std::endl;
}

/**
 * @brief Descifra un archivo cifrado con AES-256 en modo CTR y guarda el resultado en un archivo de salida.
 *
 * @param input_path Ruta del archivo cifrado.
 * @param output_path Ruta donde se guardará el archivo descifrado.
 * @param sequence_number Número de secuencia utilizado para generar la clave y el IV.
 */
void decrypt(const std::string& input_path, const std::string& output_path, int sequence_number) {
    std::cout << "Descifrando archivo: " << input_path << std::endl;
    std::cout << "Número de secuencia: " << sequence_number << std::endl;

    unsigned char key[32];  // Buffer para la clave AES-256
    unsigned char iv[16];   // Buffer para el IV

    // Generar la clave y el IV de manera determinística usando el número de secuencia
    generate_symmetric_key_and_iv(key, iv, sequence_number);

    // Mostrar la clave generada
    std::cout << "Clave generada (256 bits) en descifrado: ";
    for (int i = 0; i < 32; i++) {
        printf("%02x", key[i]);
    }
    std::cout << std::endl;

    // Mostrar el IV generado
    std::cout << "IV generado en descifrado: ";
    for (int i = 0; i < 16; i++) {
        printf("%02x", iv[i]);
    }
    std::cout << std::endl;

    // Abrir el archivo cifrado para leer y el archivo de salida para escribir los datos descifrados
    FILE *input_file = fopen(input_path.c_str(), "rb");
    FILE *output_file = fopen(output_path.c_str(), "wb");
    if (!input_file || !output_file) {
        perror("Error al abrir archivos");
        exit(EXIT_FAILURE);
    }

    // Crear e inicializar el contexto de descifrado
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    // Iniciar el proceso de descifrado usando AES-256 en modo CTR
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv))
        handleErrors();

    unsigned char buffer_in[CHUNK_SIZE];  // Buffer para almacenar los datos cifrados
    unsigned char buffer_out[CHUNK_SIZE + EVP_CIPHER_block_size(EVP_aes_256_ctr())];  // Buffer para los datos descifrados
    int len, plain_len;

    // Leer el archivo cifrado por bloques y descifrar cada bloque
    while ((len = fread(buffer_in, 1, CHUNK_SIZE, input_file)) > 0) {
        if (1 != EVP_DecryptUpdate(ctx, buffer_out, &plain_len, buffer_in, len))
            handleErrors();
        fwrite(buffer_out, 1, plain_len, output_file);
    }

    // Finalizar el proceso de descifrado y escribir los últimos datos
    if (1 != EVP_DecryptFinal_ex(ctx, buffer_out + plain_len, &len))
        handleErrors();
    fwrite(buffer_out + plain_len, 1, len, output_file);

    // Liberar el contexto de descifrado y cerrar los archivos
    EVP_CIPHER_CTX_free(ctx);
    fclose(input_file);
    fclose(output_file);

    std::cout << "Descifrado completado: " << output_path << std::endl;
}

/**
 * @brief Función principal que determina la operación a realizar (cifrado o descifrado) basada en los argumentos de entrada.
 *
 * @param argc Número de argumentos.
 * @param argv Arreglo de argumentos (operación, archivo de entrada, archivo de salida, número de secuencia).
 * @return 0 si la operación fue exitosa, 1 en caso de error.
 */
int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Uso: " << argv[0] << " <operacion> <archivo_entrada> <archivo_salida> <numero_secuencia>" << std::endl;
        return 1;
    }

    std::string operation = argv[1];       // Operación solicitada (encrypt o decrypt)
    std::string input_path = argv[2];      // Ruta del archivo de entrada
    std::string output_path = argv[3];     // Ruta del archivo de salida
    int sequence_number = std::stoi(argv[4]);  // Número de secuencia para generar clave e IV

    // Determinar la operación a realizar
    if (operation == "encrypt") {
        encrypt(input_path, output_path, sequence_number);
    } else if (operation == "decrypt") {
        decrypt(input_path, output_path, sequence_number);
    } else {
        std::cerr << "Operación inválida: " << operation << std::endl;
        return 1;
    }

    return 0;
}
