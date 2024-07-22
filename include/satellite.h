#ifndef SATELLITE_H
#define SATELLITE_H

#include "aes_ctr.h"

typedef struct {
    AES_CTR_ctx crypto_ctx;
    unsigned char satellite_id[32];
} Satellite;

int init_satellite(Satellite *sat);
int encrypt_image(Satellite *sat, const char *input_file, const char *output_file);

#endif
