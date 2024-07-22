#include <stdio.h>
#include <string.h>
#include "satellite.h"
#include "ground_station.h"

#define MAX_FILENAME 256

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <modo> <archivo_entrada> <archivo_salida>\n", argv[0]);
        printf("Modo: encrypt o decrypt\n");
        return 1;
    }

    char *mode = argv[1];
    char *input_file = argv[2];
    char *output_file = argv[3];

    if (strcmp(mode, "encrypt") == 0) {
        Satellite sat;
        if (!init_satellite(&sat)) {
            printf("Error al inicializar el satélite.\n");
            return 1;
        }

        printf("ID del satélite: ");
        for (int i = 0; i < 32; i++) {
            printf("%02x", sat.satellite_id[i]);
        }
        printf("\n");

        if (encrypt_image(&sat, input_file, output_file)) {
            printf("Imagen cifrada exitosamente.\n");
        } else {
            printf("Error al cifrar la imagen.\n");
            return 1;
        }
    } else if (strcmp(mode, "decrypt") == 0) {
        GroundStation gs;
        if (!init_ground_station(&gs)) {
            printf("Error al inicializar la estación terrestre.\n");
            return 1;
        }

        unsigned char satellite_id[32];
        printf("Ingrese el ID del satélite (64 caracteres hexadecimales): ");
        char hex_input[65];
        if (scanf("%64s", hex_input) != 1 || strlen(hex_input) != 64) {
            printf("ID de satélite inválido.\n");
            return 1;
        }

        for (int i = 0; i < 32; i++) {
            sscanf(hex_input + (i * 2), "%2hhx", &satellite_id[i]);
        }

        printf("ID del satélite utilizado para descifrar: ");
        for (int i = 0; i < 32; i++) {
            printf("%02x", satellite_id[i]);
        }
        printf("\n");

        if (decrypt_image(&gs, input_file, output_file, satellite_id)) {
            printf("Imagen descifrada exitosamente.\n");
        } else {
            printf("Error al descifrar la imagen.\n");
            return 1;
        }
    } else {
        printf("Modo inválido. Use 'encrypt' o 'decrypt'.\n");
        return 1;
    }

    return 0;
}
