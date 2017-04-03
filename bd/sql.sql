--sqlite3 banco.db < sql.sql

--cria tabelas
create table central_ambiente(id INTEGER PRIMARY KEY AUTOINCREMENT, nome TEXT);
create table central_grandeza(id INTEGER PRIMARY KEY AUTOINCREMENT, codigo INTEGER, nome TEXT, unidade TEXT);
create table central_sensor(id INTEGER PRIMARY KEY AUTOINCREMENT, idRede INTEGER UNIQUE, ambiente_id REFERENCES central_ambiente(id), descricao TEXT);
create table central_sensorgrandeza(id INTEGER PRIMARY KEY AUTOINCREMENT, sensor_id REFERENCES central_sensor(idRede), grandeza_id REFERENCES central_grandeza(codigo));
create table central_leitura(id INTEGER PRIMARY KEY AUTOINCREMENT, valor REAL, createdAt INTEGER DEFAULT (datetime()), sync INTEGER DEFAULT 0, grandeza_id REFERENCES central_grandeza(codigo), sensor_id REFERENCES central_sensor(idRede), ambiente_id REFERENCES central_ambiente(id));

--insert _ambiente
insert into central_ambiente (nome) values ('Ambiente A');
insert into central_ambiente (nome) values ('Ambiente B');
insert into central_ambiente (nome) values ('Ambiente C');

--insert _grandeza
insert into central_grandeza (codigo, nome, unidade) values (3303, 'Temperatura', '°C');
insert into central_grandeza (codigo, nome, unidade) values (3304, 'Umidade do ar', '%');
insert into central_grandeza (codigo, nome, unidade) values (3320, 'Umidade do solo', '%');

--insert _sensor
insert into central_sensor (idRede, ambiente_id, descricao) values (10, 1, 'Sensor 1');
insert into central_sensor (idRede, ambiente_id, descricao) values (20, 2, 'Sensor 2');
insert into central_sensor (idRede, ambiente_id, descricao) values (30, 3, 'Sensor 3');
insert into central_sensor (idRede, ambiente_id, descricao) values (40, 1, 'Sensor 4');

--insert _sensorgrandeza
insert into central_sensorgrandeza (sensor_id, grandeza_id) values (1, 1);
insert into central_sensorgrandeza (sensor_id, grandeza_id) values (2, 2);
insert into central_sensorgrandeza (sensor_id, grandeza_id) values (3, 3);
insert into central_sensorgrandeza (sensor_id, grandeza_id) values (1, 2);