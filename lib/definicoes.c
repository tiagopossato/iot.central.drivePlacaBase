#include "definicoes.h"
#include <stdbool.h>

extern bool validaGrandeza(unsigned int grandeza, unsigned int tipo)
{
    switch (tipo)
    {
    case entradaAnalogica:
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
        break;
    case entradaDigital:
        if (grandeza >= 0 && grandeza <= 7)
            return true;
        else
            return false;
        break;
    case saidaDigital:
        if (grandeza >= 0 && grandeza <= 7)
            return true;
        else
            return false;
        break;
    case especial:
        return true;
        break;
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
    case especial:
        return true;
    default:
        return false;
    }
}