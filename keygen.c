#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <string.h>
#include "keygen.h"
#include "utils.h"

// Función para generar un secreto compartido dinámico basado en una fórmula matemática
void generate_dynamic_shared_secret(unsigned char *shared_secret, int sequence_number) {
    // Combinar un valor base con el número de secuencia usando SHA256
    char base_secret[] = "base_secret";
    char buffer[64];
    snprintf(buffer, 64, "%s_%d", base_secret, sequence_number);

    // Aplicar SHA256 sobre el buffer
    SHA256((unsigned char*)buffer, strlen(buffer), shared_secret);
}

void generate_symmetric_key_and_iv(unsigned char *key, unsigned char *iv, int sequence_number) {
    unsigned char salt[16];
    unsigned char shared_secret[32];

    // Generar el secreto compartido dinámicamente basado en la fórmula matemática
    generate_dynamic_shared_secret(shared_secret, sequence_number);

    // Usar sequence_number para generar un salt determinístico
    for (int i = 0; i < 16; i++) {
        salt[i] = (unsigned char)(sequence_number >> (i * 8));
    }

    // Usar HMAC-SHA256 para derivar la clave (32 bytes para AES-256)
    HMAC_CTX *hctx = HMAC_CTX_new();
    if (!hctx) handleErrors();

    if (1 != HMAC_Init_ex(hctx, shared_secret, 32, EVP_sha256(), NULL)) handleErrors();
    if (1 != HMAC_Update(hctx, salt, sizeof(salt))) handleErrors();
    if (1 != HMAC_Final(hctx, key, NULL)) handleErrors();

    // Generar IV usando HMAC con el mismo secreto compartido dinámico
    unsigned char iv_context[] = "IV_GENERATION";
    if (1 != HMAC_Init_ex(hctx, shared_secret, 32, EVP_sha256(), NULL)) handleErrors();
    if (1 != HMAC_Update(hctx, iv_context, sizeof(iv_context))) handleErrors();
    if (1 != HMAC_Update(hctx, salt, sizeof(salt))) handleErrors();
    if (1 != HMAC_Final(hctx, iv, NULL)) handleErrors();

    HMAC_CTX_free(hctx);
}
