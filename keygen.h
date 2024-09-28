#ifndef KEYGEN_H
#define KEYGEN_H

#ifdef __cplusplus
extern "C" {
#endif

void generate_symmetric_key_and_iv(unsigned char *key, unsigned char *iv, int sequence_number);

#ifdef __cplusplus
}
#endif

#endif // KEYGEN_H
