#include "hash.h"
#include "file_io.h"
#include <stdio.h>
#include <openssl/evp.h>

int calculate_file_hash(const char *filename, unsigned char *hash) {
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    unsigned int hash_len;

    FileStream stream;
    if (!init_read_stream(&stream, filename)) return 0;

    md = EVP_sha256();
    mdctx = EVP_MD_CTX_new();
    if(mdctx == NULL) {
        close_stream(&stream);
        return 0;
    }

    if(1 != EVP_DigestInit_ex(mdctx, md, NULL)) goto err;

    unsigned char buffer[BUFFER_SIZE];
    int bytes_read;
    while ((bytes_read = read_stream(&stream, buffer, BUFFER_SIZE)) > 0) {
        if(1 != EVP_DigestUpdate(mdctx, buffer, bytes_read)) goto err;
    }

    if(1 != EVP_DigestFinal_ex(mdctx, hash, &hash_len)) goto err;

    EVP_MD_CTX_free(mdctx);
    close_stream(&stream);
    return 1;

err:
    EVP_MD_CTX_free(mdctx);
    close_stream(&stream);
    return 0;
}

void print_hash(const unsigned char *hash) {
    for(int i = 0; i < HASH_SIZE; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
}
