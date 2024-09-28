/**
 * @file utils.c
 * @brief Funciones de utilidades para manejar errores en las operaciones criptográficas utilizando OpenSSL.
 * 
 * @details Este archivo contiene la función `handleErrors`, que se utiliza para manejar errores que ocurren
 *          durante las operaciones criptográficas. La función imprime los errores y finaliza el programa.
 * 
 * @author Astrovia
 * @date 2024-09-28
 */

#include <stdio.h>
#include <stdlib.h>
#include <openssl/err.h>
#include "utils.h"

/**
 * @brief Maneja los errores que ocurren durante las operaciones criptográficas de OpenSSL.
 *
 * @details Imprime los errores en la salida de errores estándar (stderr) y termina el programa
 *          utilizando la función `abort()`.
 */
void handleErrors(void) {
    ERR_print_errors_fp(stderr);  // Imprime los errores detallados en stderr
    abort();  // Finaliza la ejecución del programa
}
