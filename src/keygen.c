#include <openssl/evp.h>
#include "keygen.h"
#include "utils.h"

/**
 * @brief Genera una clave dinámica a partir de una clave maestra.
 *
 * @param key Un puntero donde se almacenará la clave generada.
 * @param master_key Un puntero a la clave maestra.
 * @param master_key_len La longitud de la clave maestra.
 */
void generate_dynamic_key(unsigned char *key, unsigned char *master_key, int master_key_len) {
    // Crear contexto para hash SHA-256
    EVP_MD_CTX *mdctx;
    if (!(mdctx = EVP_MD_CTX_new())) handleErrors();

    // Inicializar el contexto con SHA-256
    if (1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL)) handleErrors();

    // Actualizar el contexto con la clave maestra
    if (1 != EVP_DigestUpdate(mdctx, master_key, master_key_len)) handleErrors();

    // Finalizar y obtener la clave dinámica
    if (1 != EVP_DigestFinal_ex(mdctx, key, NULL)) handleErrors();

    // Liberar el contexto
    EVP_MD_CTX_free(mdctx);
}
