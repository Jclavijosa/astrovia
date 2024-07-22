#include "satellite.h"
#include "file_io.h"
#include "dynamic_key.h"
#include "hash.h"
#include <string.h>
#include <openssl/rand.h>
#include <stdio.h>

#define CHUNK_SIZE (1024 * 1024)

int init_satellite(Satellite *sat) {
    return RAND_bytes(sat->satellite_id, sizeof(sat->satellite_id));
}

int encrypt_image(Satellite *sat, const char *input_file, const char *output_file) {
    printf("Iniciando cifrado de imagen\n");
    
    FileStream in_stream, out_stream;
    unsigned char key[KEY_SIZE];
    unsigned char iv[IV_SIZE];
    unsigned char buffer[CHUNK_SIZE];
    unsigned char encrypted[CHUNK_SIZE];
    int bytes_read, bytes_encrypted;
    unsigned char hash[HASH_SIZE];

    if (!generate_dynamic_key(key, sat->satellite_id, sizeof(sat->satellite_id))) {
        printf("Error al generar la clave dinámica\n");
        return 0;
    }

    if (RAND_bytes(iv, IV_SIZE) != 1) {
        printf("Error al generar el IV\n");
        return 0;
    }

    if (!init_read_stream(&in_stream, input_file)) {
        printf("Error al iniciar la lectura del archivo de entrada\n");
        return 0;
    }
    if (!init_write_stream(&out_stream, output_file)) {
        printf("Error al iniciar la escritura del archivo de salida\n");
        close_stream(&in_stream);
        return 0;
    }

    if (!aes_ctr_init(&sat->crypto_ctx, key, iv)) {
        printf("Error al inicializar el contexto AES-CTR\n");
        close_stream(&in_stream);
        close_stream(&out_stream);
        return 0;
    }

    // Escribir IV al inicio del archivo de salida
    if (write_stream(&out_stream, iv, IV_SIZE) != IV_SIZE) {
        printf("Error al escribir el IV en el archivo de salida\n");
        close_stream(&in_stream);
        close_stream(&out_stream);
        return 0;
    }

    while ((bytes_read = read_stream(&in_stream, buffer, CHUNK_SIZE)) > 0) {
        bytes_encrypted = aes_ctr_encrypt(&sat->crypto_ctx, buffer, bytes_read, encrypted);
        if (bytes_encrypted < 0 || write_stream(&out_stream, encrypted, bytes_encrypted) != bytes_encrypted) {
            printf("Error durante el cifrado o escritura de datos\n");
            close_stream(&in_stream);
            close_stream(&out_stream);
            return 0;
        }
    }

    close_stream(&in_stream);
    close_stream(&out_stream);
    aes_ctr_cleanup(&sat->crypto_ctx);

    // Calcular y añadir el hash al final del archivo cifrado
    if (!calculate_file_hash(output_file, hash)) {
        printf("Error al calcular el hash del archivo cifrado\n");
        return 0;
    }

    printf("Hash calculado al cifrar: ");
    print_hash(hash);

    FILE *out_file = fopen(output_file, "ab");
    if (!out_file) {
        printf("Error al abrir el archivo de salida para añadir el hash\n");
        return 0;
    }
    if (fwrite(hash, 1, HASH_SIZE, out_file) != HASH_SIZE) {
        printf("Error al escribir el hash en el archivo de salida\n");
        fclose(out_file);
        return 0;
    }
    fclose(out_file);

    printf("Cifrado completado con éxito\n");
    return 1;
}
