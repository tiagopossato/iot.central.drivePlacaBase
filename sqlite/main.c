#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG

int rc;
char *zErrMsg = 0;
int ambienteId;

void status(char *info);
static int callback_select(void *data, int argc, char **argv, char **azColName);
void insert_central_leitura(sqlite3 *db, int sensor);

void status(char *info){
    if( rc != SQLITE_OK ){
        printf("ERRO -> %s SQL error: %s\n", info, zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        printf("SUCESSO -> %s executado no Banco de Dados!\n", info);
    }
}

static int callback_select(void *data, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        ambienteId = atoi(argv[i] ? argv[i] : "NULL");
        #if defined(DEBUG)
            printf("\t%s = %d\n", azColName[i], ambienteId);
        #endif
    }
    return 0;
}

void insert_central_leitura(sqlite3 *db, int sensor) {
    
    int sensorIdRede = sensor;
    char *sql;
    const char* data = "";
    
    sprintf(sql, "SELECT ambiente_id FROM central_sensor WHERE idRede = %d;", sensorIdRede);
    rc = sqlite3_exec(db, sql, callback_select, (void*)data, &zErrMsg);
    #if defined(DEBUG)
        status("SELECT");
    #endif
    
    sprintf(sql, "INSERT INTO central_leitura (valor, grandeza_id, sensor_id, ambiente_id) "\
        "VALUES (25.0, 1, 10, %d);", ambienteId);
    rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    #if defined(DEBUG)
        status("INSERT");
    #endif
    
}

int main(int argc, char* argv[]){
    sqlite3 *db;

    rc = sqlite3_open("banco.db", &db);

    #if defined(DEBUG)
        if( rc ){
            printf("ERRO -> Problema na conexão com o Banco de Dados: %s\n", sqlite3_errmsg(db));
            return(0);
        }else{
            printf("SUCESSO -> Conexão com o Banco de Dados realizada!\n");
        }   
    #endif
    
    insert_central_leitura(db, 30);
   
    sqlite3_close(db);
    #if defined(DEBUG)
        printf("SUCESSO -> Banco de Dados desconectado!\n");    
    #endif
    
    return 0;
}