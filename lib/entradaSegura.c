/*
Copyright (c) <2015> <Tiago Possato>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */
#include "entradaSegura.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Lê um número inteiro do teclado.
 * @param msg Mensagem para ser exibida ao usuário
 * @return Um número inteiro
 */
extern int leInt(char *msg) {
    char tmp[15];
    int k = 0;
entrada:
    fprintf(stdout, "%s", msg);
    fscanf(stdin, " %s", tmp);
    if (tmp[0] != '-') {
        if ((tmp[0] < '0' || tmp[0] > '9')) {//verifica se é número
            fprintf(stdout, "Utilize somente numeros!\n");
            goto entrada;
        }
    }

    for (k = 1; k < strlen(tmp); k++) {
        if ((tmp[k] < '0' || tmp[k] > '9')) {//verifica se é número
            fprintf(stdout, "Utilize somente numeros!\n");
            goto entrada;
        }
    }

    return atoi(tmp);
}

/**
 * Lê um número inteiro sem sinal do teclado
 * @param msg Mensagem para ser exibida ao usuário
 * @return Um número inteiro sem sinal
 */
extern unsigned int leUInt(char *msg) {
    char tmp[15];
    int k;
entrada:
    fprintf(stdout, "%s", msg);
    fscanf(stdin, " %s", tmp);

    for (k = 0; k < strlen(tmp); k++) {
        if ((tmp[k] < '0' || tmp[k] > '9')) {//verifica se é número
            fprintf(stdout, "Utilize somente numeros positivos!\n");
            goto entrada;
        }
    }
    return (unsigned int) atoi(tmp);
}
