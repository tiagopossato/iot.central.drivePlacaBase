#ifndef DADOS_H_
#define DADOS_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <stdbool.h>

struct No
{
    unsigned int idRede;
    unsigned int tipoGrandeza;                                                                                                                                                     
    unsigned int grandeza;
    float valor;
    time_t timestamp;
    struct Dados *prev;
    struct Dados *next;
};

typedef struct No Dados;

typedef struct sFilaDados
{
    Dados *head;
    Dados *tail;
    int quantidade;
    pthread_mutex_t mutex;
} FilaDados;

/*
 * Todas as funcoes publicas devem ser protegidas pelo semaforo
 * de controle de acesso aos Dados
 */
extern bool insereDados(unsigned int _idRede, unsigned int _tipoGrandeza, unsigned int _grandeza, float _valor, FilaDados *fila);
extern Dados *peekDados(FilaDados *fila);
extern bool removeDoInicio(Dados *dado, FilaDados *fila);
extern void imprimeFilaDados(FilaDados *fila);
extern void mostraDados(Dados *dado);
extern void libera(FilaDados *fila);
extern FilaDados *iniciaFila();

#ifdef	__cplusplus
}
#endif
#endif /* DADOS_H_ */