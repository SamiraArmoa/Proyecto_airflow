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
        "DNI TEXT UNIQUE,"
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
        "ID_EQUIPAJE INTEGER PRIMARY KEY AUTOINCREMENT,"
        "CODIGO TEXT UNIQUE, DNI TEXT, ID_VUELO INTEGER, PESO REAL, ESTADO TEXT);"

        "CREATE TABLE IF NOT EXISTS Billete("
        "ID_BILLETE INTEGER PRIMARY KEY AUTOINCREMENT,"
        "ID_USUARIO INTEGER, ID_VUELO INTEGER,"
        "ASIENTO TEXT, FECHA_COMPRA TEXT);";

    return exec_simple(DB(db), sql);
}

int db_usuario_insertar(void *db, const char *dni,
                        const char *nombre, const char *email,
                        long long tlf, const char *contrasena, int rol,
                        int *id_out) {
    sqlite3_stmt *stmt;

    const char *sql =
        "INSERT OR IGNORE INTO Usuario(DNI,NOMBRE,EMAIL,TLF,CONTRASENA,ROL)"
        " VALUES(?,?,?,?,?,?)";

    sqlite3_prepare_v2(DB(db), sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, dni, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, nombre, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, email, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 4, tlf);
    sqlite3_bind_text(stmt, 5, contrasena, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, rol);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        if (id_out) {
            *id_out = (int)sqlite3_last_insert_rowid(DB(db));
        }
    }

    sqlite3_finalize(stmt);
    return 0;
}

int db_aeropuerto_insertar(void *db,
                           const char *codigo,
                           const char *nombre,
                           const char *ciudad,
                           int *id_out) {
    sqlite3_stmt *stmt;

    const char *sql =
        "INSERT OR IGNORE INTO Aeropuerto(CODIGO,NOMBRE,CIUDAD) VALUES(?,?,?)";

    sqlite3_prepare_v2(DB(db), sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, codigo, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, nombre, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, ciudad, -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        if (id_out) {
            *id_out = (int)sqlite3_last_insert_rowid(DB(db));
        }
    }

    sqlite3_finalize(stmt);
    return 0;
}

int db_vuelo_insertar(void *db,
                      const char *cod_vuelo,
                      int id_origen,
                      int id_destino,
                      const char *fecha_hora,
                      double precio,
                      int capacidad,
                      int *id_out) {
    sqlite3_stmt *stmt;

    const char *sql =
        "INSERT INTO Vuelo(COD_VUELO,ID_ORIGEN,ID_DESTINO,FECHA_HORA,PRECIO,CAPACIDAD)"
        "VALUES(?,?,?,?,?,?)";

    sqlite3_prepare_v2(DB(db), sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, cod_vuelo, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, id_origen);
    sqlite3_bind_int(stmt, 3, id_destino);
    sqlite3_bind_text(stmt, 4, fecha_hora, -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 5, precio);
    sqlite3_bind_int(stmt, 6, capacidad);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        if (id_out) {
            *id_out = (int)sqlite3_last_insert_rowid(DB(db));
        }
    }

    sqlite3_finalize(stmt);
    return 0;
}

int db_equipaje_insertar(void *db,
                         const char *codigo,
                         const char *dni,
                         int id_vuelo,
                         double peso,
                         const char *estado,
                         int *id_out) {
    sqlite3_stmt *stmt;

    const char *sql =
        "INSERT OR IGNORE INTO Equipaje(CODIGO,DNI,ID_VUELO,PESO,ESTADO)"
        " VALUES(?,?,?,?,?)";

    sqlite3_prepare_v2(DB(db), sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, codigo, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, dni, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, id_vuelo);
    sqlite3_bind_double(stmt, 4, peso);
    sqlite3_bind_text(stmt, 5, estado, -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        if (id_out) {
            *id_out = (int)sqlite3_last_insert_rowid(DB(db));
        }
    }

    sqlite3_finalize(stmt);
    return 0;
}

int db_aeropuerto_buscar_codigo(void *db, const char *codigo,
                                int *id_out, char *nombre_out, char *ciudad_out) {
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(DB(db),
        "SELECT ID_A, NOMBRE, CIUDAD FROM Aeropuerto WHERE CODIGO=?",
        -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, codigo, -1, SQLITE_TRANSIENT);

    int rc = -1;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        if (id_out) {
            *id_out = sqlite3_column_int(stmt, 0);
        }

        if (nombre_out) {
            strcpy(nombre_out, (const char*)sqlite3_column_text(stmt, 1));
        }

        if (ciudad_out) {
            strcpy(ciudad_out, (const char*)sqlite3_column_text(stmt, 2));
        }

        rc = 0;
    }

    sqlite3_finalize(stmt);
    return rc;
}

/* CARGAR CSV */

int db_cargar_aeropuertos_csv(void *db, const char *ruta) {
    FILE *f = fopen(ruta, "r");
    if (!f) return -1;

    char linea[256];
    fgets(linea, sizeof(linea), f);

    int count = 0;

    while (fgets(linea, sizeof(linea), f)) {
        linea[strcspn(linea, "\r\n")] = '\0';

        char *cod = strtok(linea, ",");
        char *nom = strtok(NULL, ",");
        char *ciu = strtok(NULL, ",");

        if (!cod || !nom || !ciu) continue;

        db_aeropuerto_insertar(db, cod, nom, ciu, NULL);
        count++;
    }

    fclose(f);
    return count;
}

int db_cargar_vuelos_csv(void *db, const char *ruta) {
    FILE *f = fopen(ruta, "r");
    if (!f) return -1;

    char linea[256];
    fgets(linea, sizeof(linea), f);

    int count = 0;

    while (fgets(linea, sizeof(linea), f)) {
        linea[strcspn(linea, "\r\n")] = '\0';

        char *cod = strtok(linea, ",");
        char *aerolinea = strtok(NULL, ",");
        char *orig = strtok(NULL, ",");
        char *dest = strtok(NULL, ",");
        char *fecha = strtok(NULL, ",");
        char *h_salida = strtok(NULL, ",");
        char *h_llegada = strtok(NULL, ",");
        char *cap = strtok(NULL, ",");

        if (!cod || !orig || !dest || !fecha) continue;

        (void)aerolinea;
        (void)h_llegada;

        char fecha_hora[64] = "";

        if (fecha && h_salida) {
            snprintf(fecha_hora, sizeof(fecha_hora), "%s %s", fecha, h_salida);
        }

        int id_orig = -1;
        int id_dest = -1;
        char nom[128];
        char ciu[128];

        db_aeropuerto_buscar_codigo(db, orig, &id_orig, nom, ciu);
        db_aeropuerto_buscar_codigo(db, dest, &id_dest, nom, ciu);

        if (id_orig == -1 || id_dest == -1) {
            fprintf(stderr, "[CSV Vuelos] Aeropuerto no encontrado: '%s' o '%s'\n", orig, dest);
            continue;
        }

        db_vuelo_insertar(db, cod, id_orig, id_dest,
                          fecha_hora,
                          cap ? atof(cap) : 0.0,
                          cap ? atoi(cap) : 0,
                          NULL);

        count++;
    }

    fclose(f);
    return count;
}

int db_cargar_usuarios_csv(void *db, const char *ruta) {
    FILE *f = fopen(ruta, "r");
    if (!f) return -1;

    char linea[256];
    fgets(linea, sizeof(linea), f);

    int count = 0;

    while (fgets(linea, sizeof(linea), f)) {
        linea[strcspn(linea, "\r\n")] = '\0';

        char *dni = strtok(linea, ",");
        char *nom = strtok(NULL, ",");
        char *ape = strtok(NULL, ",");
        char *email = strtok(NULL, ",");
        char *tlf = strtok(NULL, ",");

        if (!dni || !nom || !email) continue;

        char nombreCompleto[128];
        snprintf(nombreCompleto, sizeof(nombreCompleto), "%s %s", nom, ape ? ape : "");

        db_usuario_insertar(db, dni, nombreCompleto, email,
                            tlf ? atoll(tlf) : 0,
                            "1234",
                            2,
                            NULL);

        count++;
    }

    fclose(f);
    return count;
}

int db_cargar_equipajes_csv(void *db, const char *ruta) {
    FILE *f = fopen(ruta, "r");
    if (!f) return -1;

    char linea[256];
    fgets(linea, sizeof(linea), f);

    int count = 0;

    while (fgets(linea, sizeof(linea), f)) {
        linea[strcspn(linea, "\r\n")] = '\0';

        char *codigo = strtok(linea, ",");
        char *dni = strtok(NULL, ",");
        char *id_vuelo = strtok(NULL, ",");
        char *peso = strtok(NULL, ",");
        char *estado = strtok(NULL, ",");

        if (!codigo || !dni || !id_vuelo) continue;

        db_equipaje_insertar(db, codigo, dni,
                             atoi(id_vuelo),
                             peso ? atof(peso) : 0.0,
                             estado ? estado : "",
                             NULL);

        count++;
    }

    fclose(f);
    return count;
}

/* LISTAR POR CONSOLA */

int db_aeropuerto_listar(void *db) {
    char *texto = db_aeropuerto_listar_texto(db);

    if (texto) {
        printf("%s", texto);
        free(texto);
    }

    return 0;
}

int db_vuelo_listar(void *db) {
    char *texto = db_vuelo_listar_texto(db);

    if (texto) {
        printf("%s", texto);
        free(texto);
    }

    return 0;
}

int db_usuario_listar(void *db) {
    char *texto = db_usuario_listar_texto(db);

    if (texto) {
        printf("%s", texto);
        free(texto);
    }

    return 0;
}

int db_equipaje_listar(void *db) {
    char *texto = db_equipaje_listar_texto(db);

    if (texto) {
        printf("%s", texto);
        free(texto);
    }

    return 0;
}

/* LISTAR PARA SOCKETS */

static void append_text(char *buffer, int buffer_size, const char *texto) {
    if (!buffer || !texto) return;

    if ((int)(strlen(buffer) + strlen(texto) + 1) < buffer_size) {
        strcat(buffer, texto);
    }
}

char *db_aeropuerto_listar_texto(void *db) {
    sqlite3_stmt *stmt;
    char *resultado = (char *)malloc(20000);

    if (!resultado) return NULL;

    resultado[0] = '\0';

    sqlite3_prepare_v2(DB(db),
        "SELECT ID_A,CODIGO,NOMBRE,CIUDAD FROM Aeropuerto",
        -1, &stmt, NULL);

    append_text(resultado, 20000, "\nAEROPUERTOS:\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        char linea[512];

        snprintf(linea, sizeof(linea),
            "%d | %s | %s | %s\n",
            sqlite3_column_int(stmt, 0),
            sqlite3_column_text(stmt, 1),
            sqlite3_column_text(stmt, 2),
            sqlite3_column_text(stmt, 3));

        append_text(resultado, 20000, linea);
    }

    sqlite3_finalize(stmt);
    return resultado;
}

char *db_vuelo_listar_texto(void *db) {
    sqlite3_stmt *stmt;
    char *resultado = (char *)malloc(30000);

    if (!resultado) return NULL;

    resultado[0] = '\0';

    sqlite3_prepare_v2(DB(db),
        "SELECT v.ID_VUELO, v.COD_VUELO, "
        "a1.CODIGO, a2.CODIGO, "
        "v.FECHA_HORA, v.PRECIO, v.CAPACIDAD "
        "FROM Vuelo v "
        "LEFT JOIN Aeropuerto a1 ON v.ID_ORIGEN = a1.ID_A "
        "LEFT JOIN Aeropuerto a2 ON v.ID_DESTINO = a2.ID_A",
        -1, &stmt, NULL);

    append_text(resultado, 30000,
        "\nID | CODIGO | ORIGEN | DESTINO | FECHA | PRECIO | CAPACIDAD\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        char linea[512];

        snprintf(linea, sizeof(linea),
            "%d | %s | %s | %s | %s | %.2f | %d\n",
            sqlite3_column_int(stmt, 0),
            sqlite3_column_text(stmt, 1),
            sqlite3_column_text(stmt, 2),
            sqlite3_column_text(stmt, 3),
            sqlite3_column_text(stmt, 4),
            sqlite3_column_double(stmt, 5),
            sqlite3_column_int(stmt, 6));

        append_text(resultado, 30000, linea);
    }

    sqlite3_finalize(stmt);
    return resultado;
}

char *db_usuario_listar_texto(void *db) {
    sqlite3_stmt *stmt;
    char *resultado = (char *)malloc(20000);

    if (!resultado) return NULL;

    resultado[0] = '\0';

    sqlite3_prepare_v2(DB(db),
        "SELECT ID_US,NOMBRE,EMAIL,ROL FROM Usuario",
        -1, &stmt, NULL);

    append_text(resultado, 20000, "\nUSUARIOS:\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        char linea[512];

        snprintf(linea, sizeof(linea),
            "%d | %s | %s | Rol:%d\n",
            sqlite3_column_int(stmt, 0),
            sqlite3_column_text(stmt, 1),
            sqlite3_column_text(stmt, 2),
            sqlite3_column_int(stmt, 3));

        append_text(resultado, 20000, linea);
    }

    sqlite3_finalize(stmt);
    return resultado;
}

char *db_equipaje_listar_texto(void *db) {
    sqlite3_stmt *stmt;
    char *resultado = (char *)malloc(20000);

    if (!resultado) return NULL;

    resultado[0] = '\0';

    sqlite3_prepare_v2(DB(db),
        "SELECT ID_EQUIPAJE,CODIGO,DNI,ID_VUELO,PESO,ESTADO FROM Equipaje",
        -1, &stmt, NULL);

    append_text(resultado, 20000, "\nEQUIPAJES:\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        char linea[512];

        snprintf(linea, sizeof(linea),
            "%d | %s | DNI:%s | Vuelo:%d | %.2f kg | %s\n",
            sqlite3_column_int(stmt, 0),
            sqlite3_column_text(stmt, 1),
            sqlite3_column_text(stmt, 2),
            sqlite3_column_int(stmt, 3),
            sqlite3_column_double(stmt, 4),
            sqlite3_column_text(stmt, 5));

        append_text(resultado, 20000, linea);
    }

    sqlite3_finalize(stmt);
    return resultado;
}

/* BUSCAR VUELO PARA SOCKETS */

char *db_vuelo_buscar_codigo_texto(void *db, const char *codigo) {
    sqlite3_stmt *stmt;
    char *resultado = (char *)malloc(4096);

    if (!resultado) return NULL;

    resultado[0] = '\0';

    sqlite3_prepare_v2(DB(db),
        "SELECT v.ID_VUELO, v.COD_VUELO, "
        "a1.CODIGO, a2.CODIGO, "
        "v.FECHA_HORA, v.PRECIO, v.CAPACIDAD "
        "FROM Vuelo v "
        "LEFT JOIN Aeropuerto a1 ON v.ID_ORIGEN = a1.ID_A "
        "LEFT JOIN Aeropuerto a2 ON v.ID_DESTINO = a2.ID_A "
        "WHERE v.COD_VUELO = ?",
        -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, codigo, -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        snprintf(resultado, 4096,
            "\n=== VUELO ENCONTRADO ===\n"
            "ID: %d\n"
            "Codigo: %s\n"
            "Origen: %s\n"
            "Destino: %s\n"
            "Fecha: %s\n"
            "Precio: %.2f\n"
            "Capacidad: %d\n",
            sqlite3_column_int(stmt, 0),
            sqlite3_column_text(stmt, 1),
            sqlite3_column_text(stmt, 2),
            sqlite3_column_text(stmt, 3),
            sqlite3_column_text(stmt, 4),
            sqlite3_column_double(stmt, 5),
            sqlite3_column_int(stmt, 6));
    }
    else {
        strcpy(resultado, "ERROR|Vuelo no encontrado");
    }

    sqlite3_finalize(stmt);
    return resultado;
}