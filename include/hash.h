#ifndef HASH_H
#define HASH_H

#include <openssl/sha.h>

#define HASH_SIZE SHA256_DIGEST_LENGTH

int calculate_file_hash(const char *filename, unsigned char *hash);
void print_hash(const unsigned char *hash);

#endif
