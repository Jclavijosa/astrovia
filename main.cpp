#include <openssl/sha.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <openssl/evp.h>
#include "keygen.h"
#include "utils.h"

#define CHUNK_SIZE (1024 * 1024) // 1MB chunk size for processing large files

void encrypt(const std::string& input_path, const std::string& output_path, int sequence_number) {
    std::cout << "Cifrando archivo: " << input_path << std::endl;
    std::cout << "Número de secuencia: " << sequence_number << std::endl;

    unsigned char key[32];  // 32 bytes para AES-256
    unsigned char iv[16];
    generate_symmetric_key_and_iv(key, iv, sequence_number);

    std::cout << "Clave generada (256 bits) en cifrado: ";
    for (int i = 0; i < 32; i++) {
        printf("%02x", key[i]);
    }
    std::cout << std::endl;

    std::cout << "IV generado en cifrado: ";
    for (int i = 0; i < 16; i++) {
        printf("%02x", iv[i]);
    }
    std::cout << std::endl;

    FILE *input_file = fopen(input_path.c_str(), "rb");
    FILE *output_file = fopen(output_path.c_str(), "wb");
    if (!input_file || !output_file) {
        perror("Error al abrir archivos");
        exit(EXIT_FAILURE);
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv))
        handleErrors();

    unsigned char buffer_in[CHUNK_SIZE];
    unsigned char buffer_out[CHUNK_SIZE + EVP_CIPHER_block_size(EVP_aes_256_ctr())];
    int len, cipher_len;

    while ((len = fread(buffer_in, 1, CHUNK_SIZE, input_file)) > 0) {
        if (1 != EVP_EncryptUpdate(ctx, buffer_out, &cipher_len, buffer_in, len))
            handleErrors();
        fwrite(buffer_out, 1, cipher_len, output_file);
    }

    if (1 != EVP_EncryptFinal_ex(ctx, buffer_out + cipher_len, &len))
        handleErrors();
    fwrite(buffer_out + cipher_len, 1, len, output_file);

    EVP_CIPHER_CTX_free(ctx);
    fclose(input_file);
    fclose(output_file);

    std::cout << "Cifrado completado: " << output_path << std::endl;
}

void decrypt(const std::string& input_path, const std::string& output_path, int sequence_number) {
    std::cout << "Descifrando archivo: " << input_path << std::endl;
    std::cout << "Número de secuencia: " << sequence_number << std::endl;

    unsigned char key[32];  // 32 bytes para AES-256
    unsigned char iv[16];
    generate_symmetric_key_and_iv(key, iv, sequence_number);

    std::cout << "Clave generada (256 bits) en descifrado: ";
    for (int i = 0; i < 32; i++) {
        printf("%02x", key[i]);
    }
    std::cout << std::endl;

    std::cout << "IV generado en descifrado: ";
    for (int i = 0; i < 16; i++) {
        printf("%02x", iv[i]);
    }
    std::cout << std::endl;

    FILE *input_file = fopen(input_path.c_str(), "rb");
    FILE *output_file = fopen(output_path.c_str(), "wb");
    if (!input_file || !output_file) {
        perror("Error al abrir archivos");
        exit(EXIT_FAILURE);
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv))
        handleErrors();

    unsigned char buffer_in[CHUNK_SIZE];
    unsigned char buffer_out[CHUNK_SIZE + EVP_CIPHER_block_size(EVP_aes_256_ctr())];
    int len, plain_len;

    while ((len = fread(buffer_in, 1, CHUNK_SIZE, input_file)) > 0) {
        if (1 != EVP_DecryptUpdate(ctx, buffer_out, &plain_len, buffer_in, len))
            handleErrors();
        fwrite(buffer_out, 1, plain_len, output_file);
    }

    if (1 != EVP_DecryptFinal_ex(ctx, buffer_out + plain_len, &len))
        handleErrors();
    fwrite(buffer_out + plain_len, 1, len, output_file);

    EVP_CIPHER_CTX_free(ctx);
    fclose(input_file);
    fclose(output_file);

    std::cout << "Descifrado completado: " << output_path << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Uso: " << argv[0] << " <operacion> <archivo_entrada> <archivo_salida> <numero_secuencia>" << std::endl;
        return 1;
    }

    std::string operation = argv[1];
    std::string input_path = argv[2];
    std::string output_path = argv[3];
    int sequence_number = std::stoi(argv[4]);

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
