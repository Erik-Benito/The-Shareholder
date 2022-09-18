#include <stdio.h>
#include "error.h"

// Mensagem de error para usuario
void error_prod()
{
    printf("Opa! parece que a bolsa de valores não está disponivel!");
    return;
}

// Mensagem de erro para desenvolvedores 
void error_dev(char* error)
{
    printf(error);
    return;
}

void logError(char* error)
{
    int hasDev = 1;
    if (hasDev)
        error_dev(error);
    else
        error_prod();
    return;
}
