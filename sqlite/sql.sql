--sqlite3 banco.db < sql.sql

--cria tabelas
create table central_ambiente(id INTEGER PRIMARY KEY AUTOINCREMENT, nome TEXT);
create table central_grandeza(codigo INTEGER PRIMARY KEY, nome TEXT, unidade TEXT);
create table central_sensor(id INTEGER PRIMARY KEY AUTOINCREMENT, idRede INTEGER UNIQUE, ambiente_id REFERENCES central_ambiente(id), descricao TEXT);
create table central_sensorgrandeza(id INTEGER PRIMARY KEY AUTOINCREMENT, sensor_id REFERENCES central_sensor(idRede), grandeza_id REFERENCES central_grandeza(codigo), UNIQUE(sensor_id, grandeza_id) ON CONFLICT ROLLBACK);
create table central_leitura(id INTEGER PRIMARY KEY AUTOINCREMENT, valor REAL, createdAt INTEGER, sync INTEGER DEFAULT 0, grandeza_id REFERENCES central_grandeza(codigo), sensor_id REFERENCES central_sensor(idRede), ambiente_id REFERENCES central_ambiente(id));
create table central_log (id integer NOT NULL PRIMARY KEY AUTOINCREMENT, tipo varchar(6) NOT NULL, mensagem varchar(255) NOT NULL, sync bool NOT NULL DEFAULT 0, tempo datetime NOT NULL);

--insert _ambiente
insert into central_ambiente (nome) values ('Ambiente A');
insert into central_ambiente (nome) values ('Ambiente B');
insert into central_ambiente (nome) values ('Ambiente C');

--insert _grandeza
insert into central_grandeza (codigo, nome, unidade) values (3303, 'Temperatura', '°C');
insert into central_grandeza (codigo, nome, unidade) values (3304, 'Umidade do ar', '%');
insert into central_grandeza (codigo, nome, unidade) values (3320, 'Umidade do solo', '%');

--insert _sensor
insert into central_sensor (idRede, ambiente_id, descricao) values (1, 1, 'Sensor 1');
insert into central_sensor (idRede, ambiente_id, descricao) values (2, 2, 'Sensor 2');
insert into central_sensor (idRede, ambiente_id, descricao) values (3, 3, 'Sensor 3');
insert into central_sensor (idRede, ambiente_id, descricao) values (4, 1, 'Sensor 4');
insert into central_sensor (idRede, ambiente_id, descricao) values (5, 1, 'Sensor 5');

--insert _sensorgrandeza
insert into central_sensorgrandeza (sensor_id, grandeza_id) values (1, 3303);
insert into central_sensorgrandeza (sensor_id, grandeza_id) values (1, 3304);
insert into central_sensorgrandeza (sensor_id, grandeza_id) values (1, 3320);

insert into central_sensorgrandeza (sensor_id, grandeza_id) values (2, 3303);
insert into central_sensorgrandeza (sensor_id, grandeza_id) values (2, 3304);
insert into central_sensorgrandeza (sensor_id, grandeza_id) values (2, 3320);

insert into central_sensorgrandeza (sensor_id, grandeza_id) values (3, 3303);
insert into central_sensorgrandeza (sensor_id, grandeza_id) values (3, 3304);
insert into central_sensorgrandeza (sensor_id, grandeza_id) values (3, 3320);

insert into central_sensorgrandeza (sensor_id, grandeza_id) values (4, 3303);
insert into central_sensorgrandeza (sensor_id, grandeza_id) values (4, 3304);
insert into central_sensorgrandeza (sensor_id, grandeza_id) values (4, 3320);

insert into central_sensorgrandeza (sensor_id, grandeza_id) values (5, 3303);
insert into central_sensorgrandeza (sensor_id, grandeza_id) values (5, 3304);
insert into central_sensorgrandeza (sensor_id, grandeza_id) values (5, 3320);
