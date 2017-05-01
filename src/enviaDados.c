#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "../lib/filaSaida.h"
#include "../lib/definicoes.h"
#include "../lib/util.h"
//porta serial
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h>  /* File control definitions */

typedef struct sPR
{
    FilaDados *fila;
    int portaSerial;
} ParametrosRecebe;

/*
Padrão da URI:
idRede/tipoGrandeza/grandeza/valor
*/
void *recebeDados(void *args)
{
    //pega os parametros enviados por argumento para a thread
    ParametrosRecebe *params = (ParametrosRecebe *)args;
    //pega a fila de dados
    FilaDados *fila = params->fila;
    //pega o descritor da porta serial
    int fd = params->portaSerial;

    unsigned int idRede;
    unsigned int tipoGrandeza;
    unsigned int grandeza;
    float valor;

    char buf[64];
    char uri[64];
    int inicio = -1;
    int fim = -1;
    while (true)
    {
        //limpa as variaveis
        memset(buf, 1, sizeof(buf));
        memset(uri, '\0', sizeof(uri));
        //le porta serial
        if (read(fd, buf, sizeof(buf)) > 0) //chamada bloqueante
        {
            //verifica o início e final da string de dados
            inicio = buscaCaracter(buf, '[');
            fim = buscaCaracter(buf, ']');
            //validação 1 da entrada
            if (inicio < 0 || fim <= inicio)
            {
                continue;
            }
            //extração 1 dos dados
            memcpy(uri, &buf[inicio + 1], fim - (inicio + 1));
        }
        else
        {
            continue;
        }

        printf("\n++++++++++++++++++\n%s", uri);
        //--------------LE O ID DE REDE-----------------------------------
        idRede = (unsigned int)extraiParte(uri);
        //--------------LE O TIPO DE GRANDEZA-----------------------------------
        tipoGrandeza = (unsigned int)extraiParte(uri);
        //-------------LE A GRANDEZA------------------------------------
        grandeza = (unsigned int)extraiParte(uri);
        //------------LE O VALOR DA GRANDEZA-------------------------------------
        valor = extraiParte(uri);
        //-------------------------------------------------
        //VALIDA TIPO DA GRANDEZA
        if (!validaTipoGrandeza(tipoGrandeza))
        {
            printf("Tipo de grandeza: %d não reconhecido!\n", tipoGrandeza);
            continue;
        }
        //VALIDA GRANDEZA
        if (!validaGrandeza(grandeza, tipoGrandeza))
        {
            printf("Grandeza: %d não reconhecida!\n", grandeza);
            continue;
        }

        insereDados(idRede, tipoGrandeza, grandeza, valor, fila);
        mostraDados(fila->tail);
    }
}