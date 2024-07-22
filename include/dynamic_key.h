#ifndef DYNAMIC_KEY_H
#define DYNAMIC_KEY_H

#include <openssl/evp.h>

#define KEY_SIZE 32

int generate_dynamic_key(unsigned char *key, const unsigned char *seed, int seed_len);

#endif
