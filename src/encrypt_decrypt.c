#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include "../include/encrypt_decrypt.h"
#include "../include/utils.h"

#define CHUNK_SIZE (1024 * 1024) // 1MB

void encrypt_decrypt(const char *input_filename, const char *output_filename, unsigned char *key, int key_len, int enc) {
    FILE *input_file = fopen(input_filename, "rb");
    FILE *output_file = fopen(output_filename, "wb");
    if (!input_file || !output_file) {
        perror("File open error");
        exit(EXIT_FAILURE);
    }

    EVP_CIPHER_CTX *ctx;
    unsigned char inbuf[CHUNK_SIZE];
    unsigned char outbuf[CHUNK_SIZE + EVP_MAX_BLOCK_LENGTH];
    int inlen, outlen;
    unsigned char iv[16]; // 128 bits IV

    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    if(enc) {
        if (!RAND_bytes(iv, sizeof(iv))) handleErrors();
        fwrite(iv, 1, sizeof(iv), output_file);
        EVP_EncryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv);
    } else {
        size_t iv_len = EVP_CIPHER_iv_length(EVP_aes_256_ctr());
        if (fread(iv, 1, iv_len, input_file) != iv_len) {
            perror("Error reading IV");
            exit(EXIT_FAILURE);
        }
        EVP_DecryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv);
    }

    while ((inlen = fread(inbuf, 1, CHUNK_SIZE, input_file)) > 0) {
        if(enc) {
            if(!EVP_EncryptUpdate(ctx, outbuf, &outlen, inbuf, inlen)) handleErrors();
        } else {
            if(!EVP_DecryptUpdate(ctx, outbuf, &outlen, inbuf, inlen)) handleErrors();
        }
        fwrite(outbuf, 1, outlen, output_file);
    }

    if(enc) {
        if(!EVP_EncryptFinal_ex(ctx, outbuf, &outlen)) handleErrors();
    } else {
        if(!EVP_DecryptFinal_ex(ctx, outbuf, &outlen)) handleErrors();
    }
    fwrite(outbuf, 1, outlen, output_file);

    EVP_CIPHER_CTX_free(ctx);
    fclose(input_file);
    fclose(output_file);
}
