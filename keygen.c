/**
 * @file keygen.c
 * @brief Este archivo contiene las funciones para generar claves simétricas e IVs (vectores de inicialización) de manera determinística
 *        utilizando HMAC-SHA256 y un número de secuencia.
 *
 * @details La clave y el IV se generan dinámicamente a partir de un secreto compartido y un número de secuencia. 
 *          Esto garantiza que las mismas entradas generen la misma clave y el mismo IV, haciendo el sistema reproducible para cifrado y descifrado.
 *          Las funciones utilizan la biblioteca de OpenSSL.
 * 
 * @author Astrovia
 * @date 2024-09-28
 */

#include <openssl/hmac.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/sha.h>  // Incluir SHA256 para generar el secreto compartido
#include <string.h>
#include "keygen.h"
#include "utils.h"

/**
 * @brief Genera un secreto compartido dinámico basado en una fórmula matemática utilizando SHA256.
 *
 * @param shared_secret Buffer donde se almacenará el secreto compartido generado.
 * @param sequence_number Número de secuencia usado para generar el secreto compartido.
 */
void generate_dynamic_shared_secret(unsigned char *shared_secret, int sequence_number) {
    char base_secret[] = "base_secret";  // Secreto base que se combinará con el número de secuencia
    char buffer[64];
    
    // Combinar el secreto base con el número de secuencia para crear un valor único
    snprintf(buffer, 64, "%s_%d", base_secret, sequence_number);

    // Aplicar SHA256 sobre el buffer para generar el secreto compartido
    SHA256((unsigned char*)buffer, strlen(buffer), shared_secret);
}

/**
 * @brief Genera una clave simétrica y un IV (vector de inicialización) usando HMAC-SHA256 y un número de secuencia.
 *        La función utiliza un secreto compartido dinámico generado por `generate_dynamic_shared_secret`.
 *
 * @param key Buffer donde se almacenará la clave generada (32 bytes para AES-256).
 * @param iv Buffer donde se almacenará el IV generado (16 bytes para AES-256).
 * @param sequence_number Número de secuencia usado para generar la clave y el IV.
 */
void generate_symmetric_key_and_iv(unsigned char *key, unsigned char *iv, int sequence_number) {
    unsigned char salt[16];  // Salt para asegurar que el proceso sea determinístico
    unsigned char shared_secret[32];  // Buffer para almacenar el secreto compartido

    // Generar el secreto compartido dinámico basado en la fórmula matemática y el número de secuencia
    generate_dynamic_shared_secret(shared_secret, sequence_number);

    // Crear un salt basado en el número de secuencia
    for (int i = 0; i < 16; i++) {
        salt[i] = (unsigned char)(sequence_number >> (i * 8));  // Shift bits del número de secuencia
    }

    // Crear un contexto HMAC para generar la clave
    HMAC_CTX *hctx = HMAC_CTX_new();
    if (!hctx) handleErrors();

    // Usar HMAC-SHA256 con el secreto compartido para generar la clave
    if (1 != HMAC_Init_ex(hctx, shared_secret, 32, EVP_sha256(), NULL)) handleErrors();
    if (1 != HMAC_Update(hctx, salt, sizeof(salt))) handleErrors();
    if (1 != HMAC_Final(hctx, key, NULL)) handleErrors();

    // Generar el IV usando HMAC-SHA256 con un contexto diferente
    unsigned char iv_context[] = "IV_GENERATION";  // Contexto para diferenciar entre la generación de la clave y el IV
    if (1 != HMAC_Init_ex(hctx, shared_secret, 32, EVP_sha256(), NULL)) handleErrors();
    if (1 != HMAC_Update(hctx, iv_context, sizeof(iv_context))) handleErrors();
    if (1 != HMAC_Update(hctx, salt, sizeof(salt))) handleErrors();
    if (1 != HMAC_Final(hctx, iv, NULL)) handleErrors();

    // Liberar el contexto HMAC
    HMAC_CTX_free(hctx);
}
