#include "ground_station.h"
#include "file_io.h"
#include "dynamic_key.h"
#include "hash.h"
#include <string.h>
#include <stdio.h>

#define CHUNK_SIZE (1024 * 1024)

int init_ground_station(GroundStation *gs) {
    (void)gs; // Marcar como intencionalmente no utilizado
    return 1;
}

int decrypt_image(GroundStation *gs, const char *input_file, const char *output_file, const unsigned char *satellite_id) {
    printf("Iniciando descifrado de imagen\n");
    
    FileStream in_stream, out_stream;
    unsigned char key[KEY_SIZE];
    unsigned char iv[IV_SIZE];
    unsigned char buffer[CHUNK_SIZE];
    unsigned char decrypted[CHUNK_SIZE];
    int bytes_read, bytes_decrypted;
    unsigned char file_hash[HASH_SIZE];
    unsigned char calculated_hash[HASH_SIZE];

    if (!init_read_stream(&in_stream, input_file)) {
        printf("Error al iniciar la lectura del archivo de entrada\n");
        return 0;
    }

    // Verificar la longitud del archivo cifrado
    fseek(in_stream.file, 0, SEEK_END);
    long file_size = ftell(in_stream.file);
    fseek(in_stream.file, 0, SEEK_SET);  // Volver al inicio del archivo
    printf("Tamaño del archivo cifrado: %ld bytes\n", file_size);

    if (file_size <= IV_SIZE + HASH_SIZE) {
        printf("El archivo cifrado es demasiado pequeño\n");
        close_stream(&in_stream);
        return 0;
    }

    if (!init_write_stream(&out_stream, output_file)) {
        printf("Error al iniciar la escritura del archivo de salida\n");
        close_stream(&in_stream);
        return 0;
    }

    // Leer IV del inicio del archivo
    if (read_stream(&in_stream, iv, IV_SIZE) != IV_SIZE) {
        printf("Error al leer el IV del archivo de entrada\n");
        close_stream(&in_stream);
        close_stream(&out_stream);
        return 0;
    }

    if (!generate_dynamic_key(key, satellite_id, 32)) {
        printf("Error al generar la clave dinámica\n");
        close_stream(&in_stream);
        close_stream(&out_stream);
        return 0;
    }

    if (!aes_ctr_init(&gs->crypto_ctx, key, iv)) {
        printf("Error al inicializar el contexto AES-CTR\n");
        close_stream(&in_stream);
        close_stream(&out_stream);
        return 0;
    }

    long bytes_to_decrypt = file_size - IV_SIZE - HASH_SIZE;
    while (bytes_to_decrypt > 0) {
        bytes_read = read_stream(&in_stream, buffer, (bytes_to_decrypt < CHUNK_SIZE) ? bytes_to_decrypt : CHUNK_SIZE);
        if (bytes_read <= 0) break;
        
        bytes_decrypted = aes_ctr_decrypt(&gs->crypto_ctx, buffer, bytes_read, decrypted);
        if (bytes_decrypted < 0 || write_stream(&out_stream, decrypted, bytes_decrypted) != bytes_decrypted) {
            printf("Error durante el descifrado o escritura de datos\n");
            close_stream(&in_stream);
            close_stream(&out_stream);
            return 0;
        }
        bytes_to_decrypt -= bytes_read;
    }

    // Leer el hash del final del archivo
    if (read_stream(&in_stream, file_hash, HASH_SIZE) != HASH_SIZE) {
        printf("Error al leer el hash del archivo cifrado\n");
        close_stream(&in_stream);
        close_stream(&out_stream);
        return 0;
    }

    close_stream(&in_stream);
    close_stream(&out_stream);
    aes_ctr_cleanup(&gs->crypto_ctx);

    printf("Hash leído del archivo cifrado: ");
    print_hash(file_hash);

    // Verificar el hash
    if (!calculate_file_hash(output_file, calculated_hash)) {
        printf("Error al calcular el hash del archivo descifrado\n");
        return 0;
    }

    printf("Hash calculado al descifrar: ");
    print_hash(calculated_hash);

    if (memcmp(file_hash, calculated_hash, HASH_SIZE) != 0) {
        printf("El hash no coincide, la integridad del archivo está comprometida\n");
        return 0;
    }

    printf("Descifrado completado con éxito\n");
    return 1;
}
