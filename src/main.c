#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/encrypt_decrypt.h"
#include "../include/keygen.h"

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <encrypt|decrypt> <input_file> <output_file> <master_key>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *mode = argv[1];
    const char *input_file = argv[2];
    const char *output_file = argv[3];
    unsigned char *master_key = (unsigned char *)argv[4];

    unsigned char key[32]; // 256 bits
    generate_dynamic_key(key, master_key, strlen((char *)master_key));

    printf("Master Key: %s\n", master_key);
    printf("Generated Key: ");
    for(int i = 0; i < 32; i++) printf("%02x", key[i]);
    printf("\n");

    if (strcmp(mode, "encrypt") == 0) {
        encrypt_decrypt(input_file, output_file, key, sizeof(key), 1);
    } else if (strcmp(mode, "decrypt") == 0) {
        encrypt_decrypt(input_file, output_file, key, sizeof(key), 0);
    } else {
        fprintf(stderr, "Invalid mode. Use 'encrypt' or 'decrypt'.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
