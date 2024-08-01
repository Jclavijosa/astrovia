#ifndef KEYGEN_H
#define KEYGEN_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Genera una clave dinámica a partir de una clave maestra.
 *
 * @param key Un puntero donde se almacenará la clave generada.
 * @param master_key Un puntero a la clave maestra.
 * @param master_key_len La longitud de la clave maestra.
 */
void generate_dynamic_key(unsigned char *key, unsigned char *master_key, int master_key_len);

#ifdef __cplusplus
}
#endif

#endif // KEYGEN_H
