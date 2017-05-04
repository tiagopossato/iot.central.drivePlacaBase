#ifndef UTIL_H_
#define UTIL_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>
extern int buscaCaracter(char *buf, char caracter);
extern float extraiParte(char *entrada);
extern void logMessage(char *tipo, char *msg, bool salvar);

#ifdef __cplusplus
}
#endif
#endif /* UTIL_H_ */