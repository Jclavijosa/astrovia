#include <stdio.h>
#include <stdlib.h>
#include <openssl/err.h>
#include "../include/utils.h"

void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}
