/**
 * @file keygen.h
 * @brief Declaraciones de las funciones para generar claves simétricas e IVs (vectores de inicialización) de manera determinística
 *        utilizando HMAC-SHA256 y un número de secuencia.
 *
 * @details Estas funciones permiten generar claves y IVs consistentes para cifrado y descifrado usando un número de secuencia. 
 *          Las funciones utilizan la biblioteca OpenSSL para realizar las operaciones criptográficas.
 * 
 * @author Astrovia
 * @date 2024-09-28
 */

#ifndef KEYGEN_H
#define KEYGEN_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Genera un secreto compartido dinámico basado en SHA256.
 *
 * @param shared_secret Buffer donde se almacenará el secreto compartido generado.
 * @param sequence_number Número de secuencia usado para generar el secreto compartido.
 */
void generate_dynamic_shared_secret(unsigned char *shared_secret, int sequence_number);

/**
 * @brief Genera una clave simétrica y un IV (vector de inicialización) utilizando HMAC-SHA256 y un número de secuencia.
 *
 * @param key Buffer donde se almacenará la clave generada (32 bytes para AES-256).
 * @param iv Buffer donde se almacenará el IV generado (16 bytes).
 * @param sequence_number Número de secuencia usado para generar la clave y el IV.
 */
void generate_symmetric_key_and_iv(unsigned char *key, unsigned char *iv, int sequence_number);

#ifdef __cplusplus
}
#endif

#endif // KEYGEN_H
