#ifndef GROUND_STATION_H
#define GROUND_STATION_H

#include "aes_ctr.h"

typedef struct {
    AES_CTR_ctx crypto_ctx;
} GroundStation;

int init_ground_station(GroundStation *gs);
int decrypt_image(GroundStation *gs, const char *input_file, const char *output_file, const unsigned char *satellite_id);

#endif
