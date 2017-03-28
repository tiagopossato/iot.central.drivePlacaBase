//https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sqlite3.h>
#include <errno.h>

void insert_central_leitura(sqlite3 *obj_conexao, int sensor) {
    
    char *sql;
    
    int sensorId = sensor;
    /*sql = "SELECT ambiente_id FROM central_sensor WHERE idRede = %d", sensorId;
    printf("%s", sql);*/
    char buffer[100] = "asdfg";

    int i=1;

    int n = sprintf(buffer,"%s%d",buffer,i);
    
    printf("%d", n);
    
    
    

    //*SQL_STATEMENT pode ser VETOR
    sql = "insert into central_leitura (valor, grandeza_id, sensor_id, ambiente_id) values (25.0, 1, 10, 1);";

    int status;
    char * errplace;
    
    status = sqlite3_exec(obj_conexao, sql, NULL, NULL, &errplace);
    
    if (status != SQLITE_OK) {
        printf("Ocorreu um erro ao executar a consulta \" %s \"\n", sql);
        printf("Erro: \n\t %s\n", errplace);
    }else{
        printf("Dados inseridos com sucesso!\n");
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

    if (status != SQLITE_OK) {
        printf("Erro ao abrir conexão: \n\t%s\n", sqlite3_errmsg(obj_conexao));
        return 1;
    }else{
        printf("Conectado com sucesso!\n");
    }
    
    insert_central_leitura(obj_conexao, 2);
    
    status = sqlite3_close(obj_conexao);
    
    printf("Desconectado com sucesso!\n");
    
    return 0;
}