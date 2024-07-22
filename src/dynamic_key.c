#include "dynamic_key.h"
#include <openssl/evp.h>
#include <string.h>
#include <time.h>

int generate_dynamic_key(unsigned char *key, const unsigned char *seed, int seed_len) {
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    time_t current_time;
    unsigned char time_bytes[sizeof(time_t)];

    time(&current_time);
    memcpy(time_bytes, &current_time, sizeof(time_t));

    md = EVP_sha256();
    mdctx = EVP_MD_CTX_new();
    if(mdctx == NULL) return 0;

    if(1 != EVP_DigestInit_ex(mdctx, md, NULL)) goto err;
    if(1 != EVP_DigestUpdate(mdctx, seed, seed_len)) goto err;
    if(1 != EVP_DigestUpdate(mdctx, time_bytes, sizeof(time_t))) goto err;
    if(1 != EVP_DigestFinal_ex(mdctx, hash, &hash_len)) goto err;

    memcpy(key, hash, KEY_SIZE);
    EVP_MD_CTX_free(mdctx);
    return 1;

err:
    EVP_MD_CTX_free(mdctx);
    return 0;
}
