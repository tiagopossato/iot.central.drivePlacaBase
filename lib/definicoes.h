#ifndef DEFINICOES_H_
#define DEFINICOES_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

enum TP
{
    entradaDigital = 3200,
    saidaDigital = 3201,
    entradaAnalogica = 3202,
    especial = 9999
} TipoGrandezas;

enum G
{
    temperatura = 3303,
    umidadeAr = 3304,
    umidadeSolo = 3320
} Grandezas;

extern bool validaGrandeza(unsigned int grandeza, unsigned int tipo);
extern bool validaTipoGrandeza(unsigned int tipo);

#ifdef __cplusplus
}
#endif

#endif /* DEFINICOES_H_ */