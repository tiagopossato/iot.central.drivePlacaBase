#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "../lib/dados.h"

/*
 * Testando a biblioteca
 */
int main(int argc, char **argv)
{
    printf("Iniciando...\n");
    iniciaFila();

    unsigned int id = 1;
    unsigned char codigo = 70;
    unsigned char msg[7] = {};
    msg[0] = 71;
    msg[1] = 25;
    msg[2] = 57;    
    insereDados(id, codigo, msg);
    
    id = 2;
    codigo = 70;
    msg[0] = 71;
    msg[1] = 19;
    msg[2] = 32;
    insereDados(id, codigo, msg);
    
    id = 1;
    codigo = 70;
    msg[0] = 72;
    msg[1] = 84;
    msg[2] = 89;
    insereDados(id, codigo, msg);
    
    imprimeFilaDados();

    Dados *tmp;
    tmp = peekDados();
    mostraDados(tmp);
    removeDados(tmp);
    
    tmp = peekDados();
    mostraDados(tmp);
    removeDados(tmp);
    
    imprimeFilaDados();
    
    id = 2;
    codigo = 70;
    msg[0] = 71;
    msg[1] = 20;
    msg[2] = 25;
    insereDados(id, codigo, msg);
    
    imprimeFilaDados();
    return (EXIT_SUCCESS);
}
