/**
 * @file utils.h
 * @brief Declaración de la función de manejo de errores para las operaciones criptográficas.
 * 
 * @details Esta función se utiliza para capturar y manejar cualquier error que ocurra
 *          durante las operaciones criptográficas realizadas con OpenSSL.
 * 
 * @author Astrovia
 * @date 2024-09-28
 */

#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Maneja los errores que ocurren durante las operaciones criptográficas.
 */
void handleErrors(void);

#ifdef __cplusplus
}
#endif

#endif // UTILS_H
