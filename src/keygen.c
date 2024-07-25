#include <openssl/evp.h>
#include "../include/keygen.h"
#include "../include/utils.h"

void generate_dynamic_key(unsigned char *key, unsigned char *master_key, int master_key_len) {
    EVP_MD_CTX *mdctx;
    if(!(mdctx = EVP_MD_CTX_new())) handleErrors();
    if(1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL)) handleErrors();
    if(1 != EVP_DigestUpdate(mdctx, master_key, master_key_len)) handleErrors();
    if(1 != EVP_DigestFinal_ex(mdctx, key, NULL)) handleErrors();
    EVP_MD_CTX_free(mdctx);
}
