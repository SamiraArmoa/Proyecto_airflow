#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../lib/sqlite3/sqlite3.h"
#include "db.h"

#define DB(ptr) ((sqlite3*)(ptr))

static void trim_nl(char *s) {
    s[strcspn(s, "\r\n")] = 0;
}

static int exec_simple(sqlite3 *db, const char *sql) {
    char *err = NULL;
    if (sqlite3_exec(db, sql, NULL, NULL, &err) != SQLITE_OK) {
        fprintf(stderr, "[DB] %s\n", err);
        sqlite3_free(err);
        return -1;
    }
    return 0;
}

//Conexion

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

//Tablas

int db_inicializar(void *db) {
    const char *sql =
        "BEGIN;"

        "CREATE TABLE IF NOT EXISTS Usuario ("
        "ID_US INTEGER PRIMARY KEY AUTOINCREMENT,"
        "DNI TEXT UNIQUE,"
        "NOMBRE TEXT,"
        "EMAIL TEXT UNIQUE,"
        "TLF INTEGER,"
        "CONTRASENA TEXT,"
        "ROL INTEGER);"

        "CREATE TABLE IF NOT EXISTS Aeropuerto ("
        "ID_A INTEGER PRIMARY KEY AUTOINCREMENT,"
        "CODIGO TEXT UNIQUE,"
        "NOMBRE TEXT,"
        "CIUDAD TEXT);"

        "CREATE TABLE IF NOT EXISTS Vuelo ("
        "ID_VUELO INTEGER PRIMARY KEY AUTOINCREMENT,"
        "COD_VUELO TEXT,"
        "ID_ORIGEN INTEGER,"
        "ID_DESTINO INTEGER,"
        "FECHA_HORA TEXT,"
        "PRECIO REAL,"
        "CAPACIDAD INTEGER,"
        "FOREIGN KEY (ID_ORIGEN) REFERENCES Aeropuerto(ID_A),"
        "FOREIGN KEY (ID_DESTINO) REFERENCES Aeropuerto(ID_A));"

        "CREATE TABLE IF NOT EXISTS Equipaje ("
        "ID_EQUIPAJE TEXT PRIMARY KEY,"
        "DNI_PASAJERO TEXT,"
        "ID_VUELO INTEGER,"
        "PESO REAL,"
        "ESTADO TEXT,"
        "FOREIGN KEY (DNI_PASAJERO) REFERENCES Usuario(DNI),"
        "FOREIGN KEY (ID_VUELO) REFERENCES Vuelo(ID_VUELO));"

        "COMMIT;";

    return exec_simple(DB(db), sql);
}

//Inserts

int db_aeropuerto_insertar(void *db, const char *cod,
                           const char *nom, const char *ciu) {
    sqlite3_stmt *stmt;

    const char *sql =
        "INSERT OR IGNORE INTO Aeropuerto(CODIGO,NOMBRE,CIUDAD) VALUES(?,?,?)";

    sqlite3_prepare_v2(DB(db), sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt,1,cod,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,2,nom,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,3,ciu,-1,SQLITE_TRANSIENT);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return 0;
}

int db_vuelo_insertar(void *db, const char *cod,
                      int orig, int dest,
                      const char *fh, double precio,
                      int cap) {
    sqlite3_stmt *stmt;

    const char *sql =
        "INSERT INTO Vuelo VALUES(NULL,?,?,?,?,?,?)";

    sqlite3_prepare_v2(DB(db), sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt,1,cod,-1,SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt,2,orig);
    sqlite3_bind_int(stmt,3,dest);
    sqlite3_bind_text(stmt,4,fh,-1,SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt,5,precio);
    sqlite3_bind_int(stmt,6,cap);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return 0;
}

int db_usuario_insertar(void *db,
                        const char *dni,
                        const char *nombre,
                        const char *email,
                        long long tlf,
                        const char *pass,
                        int rol) {
    sqlite3_stmt *stmt;

    const char *sql =
        "INSERT OR IGNORE INTO Usuario VALUES(NULL,?,?,?,?,?,?)";

    sqlite3_prepare_v2(DB(db), sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt,1,dni,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,2,nombre,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,3,email,-1,SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt,4,tlf);
    sqlite3_bind_text(stmt,5,pass,-1,SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt,6,rol);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return 0;
}

int db_equipaje_insertar(void *db,
    const char *id,
    const char *dni,
    int id_vuelo,
    double peso,
    const char *estado) {

    sqlite3_stmt *stmt;

    const char *sql =
        "INSERT INTO Equipaje VALUES(?,?,?,?,?)";

    sqlite3_prepare_v2(DB(db), sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt,1,id,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,2,dni,-1,SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt,3,id_vuelo);
    sqlite3_bind_double(stmt,4,peso);
    sqlite3_bind_text(stmt,5,estado,-1,SQLITE_TRANSIENT);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return 0;
}

//Busquedas

int db_aeropuerto_buscar_codigo(void *db, const char *codigo, int *id_out) {
    sqlite3_stmt *stmt;

    const char *sql = "SELECT ID_A FROM Aeropuerto WHERE CODIGO=?";

    sqlite3_prepare_v2(DB(db), sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt,1,codigo,-1,SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        *id_out = sqlite3_column_int(stmt,0);
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    return -1;
}

int db_usuario_existe(void *db, const char *dni) {
    sqlite3_stmt *stmt;

    const char *sql = "SELECT 1 FROM Usuario WHERE DNI=?";

    sqlite3_prepare_v2(DB(db), sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt,1,dni,-1,SQLITE_TRANSIENT);

    int existe = (sqlite3_step(stmt) == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return existe;
}

//Csvs

int db_cargar_aeropuertos_csv(void *db, const char *ruta) {
    FILE *f = fopen(ruta, "r");
    if (!f) return -1;

    char linea[256];
    fgets(linea,sizeof(linea),f);

    exec_simple(DB(db),"BEGIN;");
    int count=0;

    while(fgets(linea,sizeof(linea),f)){
        trim_nl(linea);

        char *id = strtok(linea,";");
        char *cod = strtok(NULL,";");
        char *nom = strtok(NULL,";");
        char *ciu = strtok(NULL,";");

        if(!cod||!nom||!ciu) continue;

        db_aeropuerto_insertar(db,cod,nom,ciu);
        count++;
    }

    exec_simple(DB(db),"COMMIT;");
    fclose(f);
    return count;
}

int db_cargar_vuelos_csv(void *db, const char *ruta) {
    FILE *f = fopen(ruta, "r");
    if (!f) return -1;

    char linea[256];
    fgets(linea,sizeof(linea),f);

    exec_simple(DB(db),"BEGIN;");
    int count=0;

    while(fgets(linea,sizeof(linea),f)){
        trim_nl(linea);

        char *id = strtok(linea,";");
        char *cod = strtok(NULL,";");
        char *orig = strtok(NULL,";");
        char *dest = strtok(NULL,";");
        char *fh = strtok(NULL,";");
        char *precio = strtok(NULL,";");
        char *cap = strtok(NULL,";");

        if(!cod||!orig||!dest) continue;

        db_vuelo_insertar(db,cod,atoi(orig),atoi(dest),fh,atof(precio),atoi(cap));
        count++;
    }

    exec_simple(DB(db),"COMMIT;");
    fclose(f);
    return count;
}

int db_cargar_usuarios_csv(void *db, const char *ruta) {
    FILE *f = fopen(ruta, "r");
    if (!f) return -1;

    char linea[256];
    fgets(linea,sizeof(linea),f);

    exec_simple(DB(db),"BEGIN;");
    int count=0;

    while(fgets(linea,sizeof(linea),f)){
        trim_nl(linea);

        char *dni = strtok(linea,",");
        char *nom = strtok(NULL,",");
        char *ape = strtok(NULL,",");
        char *email = strtok(NULL,",");
        char *tlf = strtok(NULL,",");

        if(!dni||!nom||!email) continue;

        char nombreCompleto[100];
        sprintf(nombreCompleto,"%s %s",nom,ape);

        db_usuario_insertar(db,dni,nombreCompleto,email,atoll(tlf),"1234",2);
        count++;
    }

    exec_simple(DB(db),"COMMIT;");
    fclose(f);
    return count;
}

int db_cargar_equipajes_csv(void *db, const char *ruta) {
    FILE *f = fopen(ruta, "r");
    if (!f) return -1;

    char linea[256];
    fgets(linea,sizeof(linea),f);

    exec_simple(DB(db),"BEGIN;");
    int count=0;

    while(fgets(linea,sizeof(linea),f)){
        trim_nl(linea);

        char *id = strtok(linea,",");
        char *dni = strtok(NULL,",");
        char *id_vuelo = strtok(NULL,",");
        char *peso = strtok(NULL,",");
        char *estado = strtok(NULL,",");

        if(!id||!dni||!id_vuelo) continue;

        if(!db_usuario_existe(db,dni)) continue;

        db_equipaje_insertar(db,id,dni,atoi(id_vuelo),atof(peso),estado);
        count++;
    }

    exec_simple(DB(db),"COMMIT;");
    fclose(f);
    return count;
}
