#ifndef UTIL_H_
#define UTIL_H_

#ifdef __cplusplus
extern "C" {
#endif

extern int buscaCaracter(char *buf, char caracter);
extern float extraiParte(char *entrada);
extern void logMessage(char *tipo, char *msg);

#ifdef __cplusplus
}
#endif
#endif /* UTIL_H_ */