#include "definicoes.h"
#include <stdbool.h>

extern bool validaGrandeza(unsigned int grandeza)
{
    switch (grandeza)
    {
    case temperatura:
        return true;
    case umidadeAr:
        return true;
    case umidadeSolo:
        return true;
    default:
        return false;
    }
}

extern bool validaTipoGrandeza(unsigned int tipo)
{
    switch (tipo)
    {
    case entradaDigital:
        return true;
    case saidaDigital:
        return true;
    case entradaAnalogica:
        return true;
    default:
        return false;
    }
}