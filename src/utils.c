#include <stdio.h>
#include <stdlib.h>
#include <openssl/err.h>
#include "utils.h"

/**
 * @brief Maneja los errores de OpenSSL y muestra los mensajes correspondientes.
 */
void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}
