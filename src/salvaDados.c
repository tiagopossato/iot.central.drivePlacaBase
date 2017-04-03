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