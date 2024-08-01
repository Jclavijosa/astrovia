#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include "keygen.h"
#include "utils.h"

#define CHUNK_SIZE (1024 * 1024) // Tamaño de chunk de 1MB para procesar archivos grandes
#define MASTER_KEY "your-master-key"

// Declaraciones de funciones
void encrypt(const std::string& input_path, const std::string& output_path);
void decrypt(const std::string& input_path, const std::string& output_path);

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <operation> <input_path> <output_path>" << std::endl;
        return 1;
    }

    std::string operation = argv[1];
    std::string input_path = argv[2];
    std::string output_path = argv[3];

    if (operation == "encrypt") {
        encrypt(input_path, output_path);
    } else if (operation == "decrypt") {
        decrypt(input_path, output_path);
    } else {
        std::cerr << "Invalid operation: " << operation << std::endl;
        return 1;
    }

    return 0;
}

/**
 * @brief Cifra una imagen utilizando el algoritmo AES-256-CTR.
 *
 * @param input_path La ruta del archivo de imagen de entrada.
 * @param output_path La ruta del archivo de imagen cifrada de salida.
 */
void encrypt(const std::string& input_path, const std::string& output_path) {
    std::cout << "input_path=" << input_path << std::endl;
    std::cout << "output_path=" << output_path << std::endl;

    // Generar clave dinámica
    unsigned char key[32];
    unsigned char master_key[32] = MASTER_KEY;
    generate_dynamic_key(key, master_key, sizeof(master_key));

    // Abrir archivos de entrada y salida
    FILE *input_file = fopen(input_path.c_str(), "rb");
    FILE *output_file = fopen(output_path.c_str(), "wb");
    if (!input_file || !output_file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Inicializar contexto de cifrado
    EVP_CIPHER_CTX *ctx;
    unsigned char inbuf[CHUNK_SIZE];
    unsigned char outbuf[CHUNK_SIZE + EVP_MAX_BLOCK_LENGTH];
    int inlen, outlen;
    unsigned char iv[16]; // Vector de inicialización de 128 bits

    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    // Generar un nuevo IV aleatorio para cifrado
    if (!RAND_bytes(iv, sizeof(iv))) handleErrors();
    fwrite(iv, 1, sizeof(iv), output_file);
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv)) handleErrors();

    // Procesar el archivo en bloques
    while ((inlen = fread(inbuf, 1, CHUNK_SIZE, input_file)) > 0) {
        if (1 != EVP_EncryptUpdate(ctx, outbuf, &outlen, inbuf, inlen)) handleErrors();
        fwrite(outbuf, 1, outlen, output_file);
    }

    // Finalizar el proceso de cifrado
    if (1 != EVP_EncryptFinal_ex(ctx, outbuf, &outlen)) handleErrors();
    fwrite(outbuf, 1, outlen, output_file);

    // Liberar recursos y cerrar archivos
    EVP_CIPHER_CTX_free(ctx);
    fclose(input_file);
    fclose(output_file);

    std::cout << "Encrypted image" << std::endl;
}

/**
 * @brief Descifra una imagen utilizando el algoritmo AES-256-CTR.
 *
 * @param input_path La ruta del archivo de imagen cifrada de entrada.
 * @param output_path La ruta del archivo de imagen descifrada de salida.
 */
void decrypt(const std::string& input_path, const std::string& output_path) {
    std::cout << "input_path=" << input_path << std::endl;
    std::cout << "output_path=" << output_path << std::endl;

    // Generar clave dinámica
    unsigned char key[32];
    unsigned char master_key[32] = MASTER_KEY;
    generate_dynamic_key(key, master_key, sizeof(master_key));

    // Abrir archivos de entrada y salida
    FILE *input_file = fopen(input_path.c_str(), "rb");
    FILE *output_file = fopen(output_path.c_str(), "wb");
    if (!input_file || !output_file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Inicializar contexto de descifrado
    EVP_CIPHER_CTX *ctx;
    unsigned char inbuf[CHUNK_SIZE];
    unsigned char outbuf[CHUNK_SIZE + EVP_MAX_BLOCK_LENGTH];
    int inlen, outlen;
    unsigned char iv[16]; // Vector de inicialización de 128 bits

    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    // Leer el IV del archivo de entrada
    size_t iv_len = EVP_CIPHER_iv_length(EVP_aes_256_ctr());
    if (fread(iv, 1, iv_len, input_file) != iv_len) {
        perror("Error reading IV");
        exit(EXIT_FAILURE);
    }
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv)) handleErrors();

    // Procesar el archivo en bloques
    while ((inlen = fread(inbuf, 1, CHUNK_SIZE, input_file)) > 0) {
        if (1 != EVP_DecryptUpdate(ctx, outbuf, &outlen, inbuf, inlen)) handleErrors();
        fwrite(outbuf, 1, outlen, output_file);
    }

    // Finalizar el proceso de descifrado
    if (1 != EVP_DecryptFinal_ex(ctx, outbuf, &outlen)) handleErrors();
    fwrite(outbuf, 1, outlen, output_file);

    // Liberar recursos y cerrar archivos
    EVP_CIPHER_CTX_free(ctx);
    fclose(input_file);
    fclose(output_file);

    std::cout << "Decrypted image" << std::endl;
}
