#include "aes_ctr.h"
#include <string.h>

int aes_ctr_init(AES_CTR_ctx *ctx, const unsigned char *key, const unsigned char *iv) {
    ctx->ctx = EVP_CIPHER_CTX_new();
    if(ctx->ctx == NULL) return 0;

    memcpy(ctx->key, key, KEY_SIZE);
    memcpy(ctx->iv, iv, IV_SIZE);

    if(1 != EVP_EncryptInit_ex(ctx->ctx, EVP_aes_256_ctr(), NULL, ctx->key, ctx->iv))
        return 0;

    return 1;
}

int aes_ctr_encrypt(AES_CTR_ctx *ctx, const unsigned char *plaintext, int plaintext_len, unsigned char *ciphertext) {
    int len;
    int ciphertext_len;

    if(1 != EVP_EncryptUpdate(ctx->ctx, ciphertext, &len, plaintext, plaintext_len))
        return -1;
    ciphertext_len = len;

    if(1 != EVP_EncryptFinal_ex(ctx->ctx, ciphertext + len, &len))
        return -1;
    ciphertext_len += len;

    return ciphertext_len;
}

int aes_ctr_decrypt(AES_CTR_ctx *ctx, const unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext) {
    return aes_ctr_encrypt(ctx, ciphertext, ciphertext_len, plaintext);
}

void aes_ctr_cleanup(AES_CTR_ctx *ctx) {
    EVP_CIPHER_CTX_free(ctx->ctx);
}
