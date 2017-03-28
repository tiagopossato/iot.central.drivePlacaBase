#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "../lib/dados.h"

/*
 * Testando a biblioteca
 */
int main(int argc, char **argv)
{
    
    FilaDados *DADOS = iniciaFila();
    
    unsigned int idRede = 2;
    unsigned int tipoGrandeza = 3202;
    unsigned int grandeza = 3303;
    float valor = 25.69;
    
    insereDados(idRede, tipoGrandeza, grandeza, valor, DADOS);

    idRede = 2;
    tipoGrandeza = 3202;
    grandeza = 3304;
    valor = 56.89;
    insereDados(idRede, tipoGrandeza, grandeza, valor, DADOS);
    imprimeFilaDados(DADOS);
    
    Dados *tmp;
    tmp = peekDados(DADOS);
    mostraDados(tmp);
    removeDados(tmp,DADOS);
    imprimeFilaDados(DADOS);
   
    return (EXIT_SUCCESS);
}
