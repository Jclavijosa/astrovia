#ifndef AES_CTR_H
#define AES_CTR_H

#include <openssl/evp.h>

#define KEY_SIZE 32
#define IV_SIZE 16

typedef struct {
    EVP_CIPHER_CTX *ctx;
    unsigned char key[KEY_SIZE];
    unsigned char iv[IV_SIZE];
} AES_CTR_ctx;

int aes_ctr_init(AES_CTR_ctx *ctx, const unsigned char *key, const unsigned char *iv);
int aes_ctr_encrypt(AES_CTR_ctx *ctx, const unsigned char *plaintext, int plaintext_len, unsigned char *ciphertext);
int aes_ctr_decrypt(AES_CTR_ctx *ctx, const unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext);
void aes_ctr_cleanup(AES_CTR_ctx *ctx);

#endif
