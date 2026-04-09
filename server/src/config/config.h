#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    char admin_user[50];
    char admin_pass[50];

    char empleado_user[50];
    char empleado_pass[50];

    char pasajero_user[50];
    char pasajero_pass[50];

    char ruta_aeropuertos[200];
    char ruta_vuelos[200];
    char ruta_pasajeros[200];
    char ruta_equipajes[200];

    char ruta_db[200];
} Config;

int cargarConfig(const char *ruta, Config *cfg);

#endif


/*
#ifndef SERVER_CONFIG_H_
#define SERVER_CONFIG_H_
#define CLIENT_DB_PATH "../data/db/airflow.db"
#define CSV_PATH_AEROPUERTOS "data/csv/aeropuertos.csv"
#define CSV_PATH_VUELOS "data/csv/vuelos.csv"
//#define CSV_PATH_EQUIPAJES "data/csv/equipajes.csv"
#define LOGS_PATH_ "data/logs/error.log"
#endif /* SERVER_CONFIG_H_ */
