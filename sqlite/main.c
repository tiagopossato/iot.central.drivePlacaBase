#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include <errno.h>

void insert_into_ambiente(sqlite3 *obj_conexao) {

    const char *SQL_STATEMENT[] = { "insert into ambiente (nome) values ('Ambiente');"};
    int i;
    int status;
    char * errplace;
    
    for (i = 0; i < 3; i++) {
        status = sqlite3_exec(obj_conexao, SQL_STATEMENT[i], NULL, NULL, &errplace);
        
        if (status != SQLITE_OK) {
            printf("Ocorreu um erro ao executar a consulta \" %s \"\n", SQL_STATEMENT[i]);
            printf("Erro: \n\t %s\n", errplace);
        }
    }

}

int main() {

    const char *arquivo = "banco.db";
    sqlite3 *obj_conexao;
    
    if (!obj_conexao) {
        printf("%m\n");
        return 1;
    }
    
    int status = sqlite3_open_v2(arquivo, &obj_conexao, SQLITE_OPEN_READWRITE, NULL); // Utiliza o objeto sqlite3_vfs padrão.
    
    if(status == SQLITE_OK){
        
    }
    
    if (status != SQLITE_OK) {
        printf("Erro ao abrir conexão: \n\t%s\n", sqlite3_errmsg(obj_conexao));
        return 1;
    }else{
        printf("Conectado com sucesso!\n");
    }
    
    //insert_into_ambiente(obj_conexao);
    
    status = sqlite3_close(obj_conexao);
    return 0;
}