#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../lib/sqlite3/sqlite3.h"
#include "db.h"

#define DB(ptr) ((sqlite3*)(ptr))


static int exec_simple(sqlite3 *db, const char *sql) {
    char *err = NULL;
    if (sqlite3_exec(db, sql, NULL, NULL, &err) != SQLITE_OK) {
        fprintf(stderr, "[DB ERROR] %s\n", err);
        sqlite3_free(err);
        return -1;
    }
    return 0;
}



void *db_abrir(const char *ruta) {
    sqlite3 *db = NULL;
    if (sqlite3_open(ruta, &db) != SQLITE_OK) {
        fprintf(stderr, "Error BD: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    return db;
}

void db_cerrar(void *db) {
    if (db) sqlite3_close(DB(db));
}

int db_activar_fk(void *db) {
    return exec_simple(DB(db), "PRAGMA foreign_keys = ON;");
}



int db_inicializar(void *db) {
    const char *sql =
        "CREATE TABLE IF NOT EXISTS Usuario ("
        "ID_US INTEGER PRIMARY KEY AUTOINCREMENT,"
        "NOMBRE TEXT, EMAIL TEXT UNIQUE, TLF INTEGER,"
        "CONTRASENA TEXT, ROL INTEGER);"

        "CREATE TABLE IF NOT EXISTS Aeropuerto ("
        "ID_A INTEGER PRIMARY KEY AUTOINCREMENT,"
        "CODIGO TEXT UNIQUE, NOMBRE TEXT, CIUDAD TEXT);"

        "CREATE TABLE IF NOT EXISTS Vuelo ("
        "ID_VUELO INTEGER PRIMARY KEY AUTOINCREMENT,"
        "COD_VUELO TEXT, ID_ORIGEN INTEGER, ID_DESTINO INTEGER,"
        "FECHA_HORA TEXT, PRECIO REAL, CAPACIDAD INTEGER);"

        "CREATE TABLE IF NOT EXISTS Equipaje ("
        "ID_EQ INTEGER PRIMARY KEY AUTOINCREMENT,"
        "ID_BILLETE INTEGER, TIPO TEXT, PESO REAL, DESCRIPCION TEXT);";

    return exec_simple(DB(db), sql);
}



int db_usuario_insertar(void *db,
                        const char *nombre, const char *email,
                        long long tlf, const char *contrasena, int rol,
                        int *id_out) {

    sqlite3_stmt *stmt;
    const char *sql =
        "INSERT INTO Usuario(NOMBRE,EMAIL,TLF,CONTRASENA,ROL) VALUES(?,?,?,?,?)";

    sqlite3_prepare_v2(DB(db), sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt,1,nombre,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,2,email,-1,SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt,3,tlf);
    sqlite3_bind_text(stmt,4,contrasena,-1,SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt,5,rol);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        if (id_out) *id_out = (int)sqlite3_last_insert_rowid(DB(db));
    }

    sqlite3_finalize(stmt);
    return 0;
}

int db_aeropuerto_insertar(void *db,
                           const char *codigo, const char *nombre,
                           const char *ciudad, int *id_out) {

    sqlite3_stmt *stmt;
    const char *sql =
        "INSERT INTO Aeropuerto(CODIGO,NOMBRE,CIUDAD) VALUES(?,?,?)";

    sqlite3_prepare_v2(DB(db), sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt,1,codigo,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,2,nombre,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,3,ciudad,-1,SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        if (id_out) *id_out = (int)sqlite3_last_insert_rowid(DB(db));
    }

    sqlite3_finalize(stmt);
    return 0;
}

int db_vuelo_insertar(void *db,
                      const char *cod_vuelo,
                      int id_origen, int id_destino,
                      const char *fecha_hora,
                      double precio, int capacidad,
                      int *id_out) {

    sqlite3_stmt *stmt;
    const char *sql =
        "INSERT INTO Vuelo(COD_VUELO,ID_ORIGEN,ID_DESTINO,FECHA_HORA,PRECIO,CAPACIDAD)"
        "VALUES(?,?,?,?,?,?)";

    sqlite3_prepare_v2(DB(db), sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt,1,cod_vuelo,-1,SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt,2,id_origen);
    sqlite3_bind_int(stmt,3,id_destino);
    sqlite3_bind_text(stmt,4,fecha_hora,-1,SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt,5,precio);
    sqlite3_bind_int(stmt,6,capacidad);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        if (id_out) *id_out = (int)sqlite3_last_insert_rowid(DB(db));
    }

    sqlite3_finalize(stmt);
    return 0;
}

int db_equipaje_insertar(void *db,
                         int id_billete,
                         const char *tipo, double peso,
                         const char *descripcion,
                         int *id_out) {

    sqlite3_stmt *stmt;
    const char *sql =
        "INSERT INTO Equipaje(ID_BILLETE,TIPO,PESO,DESCRIPCION) VALUES(?,?,?,?)";

    sqlite3_prepare_v2(DB(db), sql, -1, &stmt, NULL);

    sqlite3_bind_int(stmt,1,id_billete);
    sqlite3_bind_text(stmt,2,tipo,-1,SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt,3,peso);
    sqlite3_bind_text(stmt,4,descripcion,-1,SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        if (id_out) *id_out = (int)sqlite3_last_insert_rowid(DB(db));
    }

    sqlite3_finalize(stmt);
    return 0;
}

//Csvs

int db_cargar_aeropuertos_csv(void *db, const char *ruta) {
    FILE *f = fopen(ruta, "r");
    if (!f) return -1;

    char linea[256];
    fgets(linea,sizeof(linea),f);

    int count = 0;

    while(fgets(linea,sizeof(linea),f)){
        char *cod = strtok(linea,",");
        char *nom = strtok(NULL,",");
        char *ciu = strtok(NULL,",");

        if(!cod||!nom||!ciu) continue;

        db_aeropuerto_insertar(db,cod,nom,ciu,NULL);
        count++;
    }

    fclose(f);
    return count;
}

int db_cargar_vuelos_csv(void *db, const char *ruta) {
    FILE *f = fopen(ruta, "r");
    if (!f) return -1;

    char linea[256];
    fgets(linea,sizeof(linea),f);

    int count = 0;

    while(fgets(linea,sizeof(linea),f)){
        char *cod = strtok(linea,",");
        char *orig = strtok(NULL,",");
        char *dest = strtok(NULL,",");
        char *fh = strtok(NULL,",");
        char *precio = strtok(NULL,",");
        char *cap = strtok(NULL,",");

        if(!cod||!orig||!dest) continue;

        db_vuelo_insertar(db,cod,atoi(orig),atoi(dest),fh,atof(precio),atoi(cap),NULL);
        count++;
    }

    fclose(f);
    return count;
}

int db_cargar_usuarios_csv(void *db, const char *ruta) {
    FILE *f = fopen(ruta, "r");
    if (!f) return -1;

    char linea[256];
    fgets(linea,sizeof(linea),f);

    int count = 0;

    while(fgets(linea,sizeof(linea),f)){
        char *nom = strtok(linea,",");
        char *email = strtok(NULL,",");
        char *tlf = strtok(NULL,",");

        if(!nom||!email) continue;

        db_usuario_insertar(db,nom,email,atoll(tlf),"1234",2,NULL);
        count++;
    }

    fclose(f);
    return count;
}

int db_cargar_equipajes_csv(void *db, const char *ruta) {
    FILE *f = fopen(ruta, "r");
    if (!f) return -1;

    char linea[256];
    fgets(linea,sizeof(linea),f);

    int count = 0;

    while(fgets(linea,sizeof(linea),f)){
        char *id_billete = strtok(linea,",");
        char *tipo = strtok(NULL,",");
        char *peso = strtok(NULL,",");
        char *desc = strtok(NULL,",");

        if(!id_billete||!tipo) continue;

        db_equipaje_insertar(db,atoi(id_billete),tipo,atof(peso),desc,NULL);
        count++;
    }

    fclose(f);
    return count;
}


int db_aeropuerto_listar(void *db) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(DB(db),
        "SELECT ID_A,CODIGO,NOMBRE,CIUDAD FROM Aeropuerto",
        -1,&stmt,NULL);

    printf("\nAEROPUERTOS:\n");
    while(sqlite3_step(stmt)==SQLITE_ROW){
        printf("%d | %s | %s | %s\n",
            sqlite3_column_int(stmt,0),
            sqlite3_column_text(stmt,1),
            sqlite3_column_text(stmt,2),
            sqlite3_column_text(stmt,3));
    }

    sqlite3_finalize(stmt);
    return 0;
}

int db_vuelo_listar(void *db) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(DB(db),
        "SELECT ID_VUELO,COD_VUELO,ID_ORIGEN,ID_DESTINO FROM Vuelo",
        -1,&stmt,NULL);

    printf("\nVUELOS:\n");
    while(sqlite3_step(stmt)==SQLITE_ROW){
        printf("%d | %s | %d -> %d\n",
            sqlite3_column_int(stmt,0),
            sqlite3_column_text(stmt,1),
            sqlite3_column_int(stmt,2),
            sqlite3_column_int(stmt,3));
    }

    sqlite3_finalize(stmt);
    return 0;
}

int db_usuario_listar(void *db) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(DB(db),
        "SELECT ID_US,NOMBRE,EMAIL,ROL FROM Usuario",
        -1,&stmt,NULL);

    printf("\nUSUARIOS:\n");
    while(sqlite3_step(stmt)==SQLITE_ROW){
        printf("%d | %s | %s | Rol:%d\n",
            sqlite3_column_int(stmt,0),
            sqlite3_column_text(stmt,1),
            sqlite3_column_text(stmt,2),
            sqlite3_column_int(stmt,3));
    }

    sqlite3_finalize(stmt);
    return 0;
}
