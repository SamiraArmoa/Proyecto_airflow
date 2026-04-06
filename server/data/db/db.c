/*
 * db.c
 *
 *  Created on: 5 abr 2026
 *      Author: oier.artabe
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../lib/sqlite3/sqlite3.h"
#include "db.h"


#define DB(ptr)  ((sqlite3*)(ptr))

// Preparar un stmt; imprimir error y retornar el código si nos falla
#define PREP(db, sql, stmt)                                             \
    do {                                                                \
        if (sqlite3_prepare_v2(DB(db), (sql), -1, &(stmt), NULL)       \
                != SQLITE_OK) {                                         \
            fprintf(stderr, "[DB] prepare: %s\n",                      \
                    sqlite3_errmsg(DB(db)));                            \
            return -1;                                                  \
        }                                                               \
    } while(0)

#define FINALIZE_ERR(stmt, db)                                          \
    do {                                                                \
        fprintf(stderr, "[DB] step: %s\n", sqlite3_errmsg(DB(db)));    \
        sqlite3_finalize(stmt);                                         \
        return -1;                                                      \
    } while(0)



static void trim_nl(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r'))
        s[--n] = '\0';
}

static int exec_simple(sqlite3 *db, const char *sql) {
    char *err = NULL;
    if (sqlite3_exec(db, sql, NULL, NULL, &err) != SQLITE_OK) {
        fprintf(stderr, "[DB] exec: %s\n", err);
        sqlite3_free(err);
        return -1;
    }
    return 0;
}

static void safe_copy(char *dst, const unsigned char *src, int max) {
    if (!dst) return;
    if (src) strncpy(dst, (const char*)src, max - 1);
    else     dst[0] = '\0';
    if (dst) dst[max-1] = '\0';
}

//Coneción con la bd

void *db_abrir(const char *ruta) {
    sqlite3 *db = NULL;
    if (sqlite3_open(ruta, &db) != SQLITE_OK) {
        fprintf(stderr, "[DB] No se pudo abrir '%s': %s\n",
                ruta, sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    return (void*)db;
}

void db_cerrar(void *db) {
    if (db) sqlite3_close(DB(db));
}

int db_activar_fk(void *db) {
    return exec_simple(DB(db), "PRAGMA foreign_keys = ON;");
}

int db_inicializar(void *db) {
    const char *sql =
        "BEGIN TRANSACTION;"
        "CREATE TABLE IF NOT EXISTS \"Usuario\" ("
        "  ID_US      INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  NOMBRE     TEXT NOT NULL,"
        "  EMAIL      TEXT UNIQUE NOT NULL,"
        "  TLF        INTEGER,"
        "  CONTRASENA TEXT NOT NULL,"
        "  ROL        INTEGER NOT NULL"
        ");"
        "CREATE TABLE IF NOT EXISTS \"Aeropuerto\" ("
        "  ID_A   INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  CODIGO TEXT UNIQUE NOT NULL,"
        "  NOMBRE TEXT,"
        "  CIUDAD TEXT"
        ");"
        "CREATE TABLE IF NOT EXISTS \"Vuelo\" ("
        "  ID_VUELO   INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  COD_VUELO  TEXT NOT NULL,"
        "  ID_ORIGEN  INTEGER,"
        "  ID_DESTINO INTEGER,"
        "  FECHA_HORA TEXT,"
        "  PRECIO     REAL,"
        "  CAPACIDAD  INTEGER,"
        "  FOREIGN KEY(ID_ORIGEN)  REFERENCES Aeropuerto(ID_A) ON DELETE CASCADE,"
        "  FOREIGN KEY(ID_DESTINO) REFERENCES Aeropuerto(ID_A) ON DELETE CASCADE"
        ");"
        "CREATE TABLE IF NOT EXISTS \"Billete\" ("
        "  ID_BILLETE   INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  ID_USUARIO   INTEGER,"
        "  ID_VUELO     INTEGER,"
        "  ASIENTO      TEXT,"
        "  FECHA_COMPRA TEXT,"
        "  FOREIGN KEY(ID_USUARIO) REFERENCES Usuario(ID_US)    ON DELETE CASCADE,"
        "  FOREIGN KEY(ID_VUELO)   REFERENCES Vuelo(ID_VUELO)   ON DELETE CASCADE"
        ");"
        "CREATE TABLE IF NOT EXISTS \"Equipaje\" ("
        "  ID_EQUIPAJE INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  ID_BILLETE  INTEGER,"
        "  TIPO        TEXT,"
        "  PESO        REAL,"
        "  DESCRIPCION TEXT,"
        "  FOREIGN KEY(ID_BILLETE) REFERENCES Billete(ID_BILLETE) ON DELETE CASCADE"
        ");"
        "COMMIT;";
    return exec_simple(DB(db), sql);
}

//Usuario

int db_usuario_insertar(void *db,
                        const char *nombre, const char *email,
                        long long tlf, const char *contrasena, int rol,
                        int *id_out)
{
    sqlite3_stmt *stmt;
    PREP(db, "INSERT INTO Usuario(NOMBRE,EMAIL,TLF,CONTRASENA,ROL) "
             "VALUES(?,?,?,?,?);", stmt);

    sqlite3_bind_text (stmt, 1, nombre,    -1, SQLITE_STATIC);
    sqlite3_bind_text (stmt, 2, email,     -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 3, tlf);
    sqlite3_bind_text (stmt, 4, contrasena,-1, SQLITE_STATIC);
    sqlite3_bind_int  (stmt, 5, rol);

    if (sqlite3_step(stmt) != SQLITE_DONE) FINALIZE_ERR(stmt, db);
    if (id_out) *id_out = (int)sqlite3_last_insert_rowid(DB(db));
    sqlite3_finalize(stmt);
    printf("Usuario creado correctamente.\n");
    return 0;
}

int db_usuario_eliminar(void *db, int id_us) {
    sqlite3_stmt *stmt;
    PREP(db, "DELETE FROM Usuario WHERE ID_US=?;", stmt);
    sqlite3_bind_int(stmt, 1, id_us);
    if (sqlite3_step(stmt) != SQLITE_DONE) FINALIZE_ERR(stmt, db);
    sqlite3_finalize(stmt);
    printf("Usuario eliminado correctamente.\n");
    return 0;
}

int db_usuario_actualizar(void *db, int id_us,
                          const char *nombre, const char *email,
                          long long tlf, const char *contrasena, int rol)
{
    sqlite3_stmt *stmt;
    PREP(db, "UPDATE Usuario SET NOMBRE=?,EMAIL=?,TLF=?,CONTRASENA=?,ROL=? "
             "WHERE ID_US=?;", stmt);
    sqlite3_bind_text (stmt, 1, nombre,    -1, SQLITE_STATIC);
    sqlite3_bind_text (stmt, 2, email,     -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 3, tlf);
    sqlite3_bind_text (stmt, 4, contrasena,-1, SQLITE_STATIC);
    sqlite3_bind_int  (stmt, 5, rol);
    sqlite3_bind_int  (stmt, 6, id_us);
    if (sqlite3_step(stmt) != SQLITE_DONE) FINALIZE_ERR(stmt, db);
    sqlite3_finalize(stmt);
    printf("Usuario actualizado correctamente.\n");
    return 0;
}

int db_usuario_buscar_id(void *db, int id_us,
                         char *nombre_out, char *email_out,
                         long long *tlf_out, char *contrasena_out,
                         int *rol_out)
{
    sqlite3_stmt *stmt;
    PREP(db, "SELECT NOMBRE,EMAIL,TLF,CONTRASENA,ROL FROM Usuario "
             "WHERE ID_US=?;", stmt);
    sqlite3_bind_int(stmt, 1, id_us);
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        safe_copy(nombre_out,    sqlite3_column_text(stmt,0), 128);
        safe_copy(email_out,     sqlite3_column_text(stmt,1), 128);
        if (tlf_out)        *tlf_out  = sqlite3_column_int64(stmt,2);
        safe_copy(contrasena_out,sqlite3_column_text(stmt,3), 256);
        if (rol_out)         *rol_out = sqlite3_column_int(stmt,4);
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 1 : -1;
}

int db_usuario_buscar_email(void *db, const char *email,
                            int *id_out, char *nombre_out,
                            long long *tlf_out, char *contrasena_out,
                            int *rol_out)
{
    sqlite3_stmt *stmt;
    PREP(db, "SELECT ID_US,NOMBRE,TLF,CONTRASENA,ROL FROM Usuario "
             "WHERE EMAIL=?;", stmt);
    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        if (id_out)    *id_out  = sqlite3_column_int(stmt,0);
        safe_copy(nombre_out,    sqlite3_column_text(stmt,1), 128);
        if (tlf_out)  *tlf_out  = sqlite3_column_int64(stmt,2);
        safe_copy(contrasena_out,sqlite3_column_text(stmt,3), 256);
        if (rol_out)  *rol_out  = sqlite3_column_int(stmt,4);
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 1 : -1;
}

int db_usuario_listar(void *db) {
    sqlite3_stmt *stmt;
    PREP(db, "SELECT ID_US,NOMBRE,EMAIL,TLF,ROL FROM Usuario;", stmt);
    printf("\n%-5s %-25s %-30s %-15s %s\n",
           "ID","Nombre","Email","Teléfono","Rol");
    printf("%-90s\n","---"
                     "---");
    int rc;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("%-5d %-25s %-30s %-15lld %d\n",
               sqlite3_column_int(stmt,0),
               sqlite3_column_text(stmt,1),
               sqlite3_column_text(stmt,2),
               sqlite3_column_int64(stmt,3),
               sqlite3_column_int(stmt,4));
    }
    sqlite3_finalize(stmt);
    fflush(stdout);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

//Aeropuerto

int db_aeropuerto_insertar(void *db,
                           const char *codigo, const char *nombre,
                           const char *ciudad, int *id_out)
{
    sqlite3_stmt *stmt;
    PREP(db, "INSERT INTO Aeropuerto(CODIGO,NOMBRE,CIUDAD) VALUES(?,?,?);",
         stmt);
    sqlite3_bind_text(stmt,1,codigo,-1,SQLITE_STATIC);
    sqlite3_bind_text(stmt,2,nombre,-1,SQLITE_STATIC);
    sqlite3_bind_text(stmt,3,ciudad,-1,SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE) FINALIZE_ERR(stmt, db);
    if (id_out) *id_out = (int)sqlite3_last_insert_rowid(DB(db));
    sqlite3_finalize(stmt);
    printf("Aeropuerto creado correctamente.\n");
    return 0;
}

int db_aeropuerto_eliminar(void *db, int id_a) {
    sqlite3_stmt *stmt;
    PREP(db, "DELETE FROM Aeropuerto WHERE ID_A=?;", stmt);
    sqlite3_bind_int(stmt,1,id_a);
    if (sqlite3_step(stmt) != SQLITE_DONE) FINALIZE_ERR(stmt, db);
    sqlite3_finalize(stmt);
    printf("Aeropuerto eliminado correctamente.\n");
    return 0;
}

int db_aeropuerto_actualizar(void *db, int id_a,
                             const char *codigo, const char *nombre,
                             const char *ciudad)
{
    sqlite3_stmt *stmt;
    PREP(db, "UPDATE Aeropuerto SET CODIGO=?,NOMBRE=?,CIUDAD=? "
             "WHERE ID_A=?;", stmt);
    sqlite3_bind_text(stmt,1,codigo,-1,SQLITE_STATIC);
    sqlite3_bind_text(stmt,2,nombre,-1,SQLITE_STATIC);
    sqlite3_bind_text(stmt,3,ciudad,-1,SQLITE_STATIC);
    sqlite3_bind_int (stmt,4,id_a);
    if (sqlite3_step(stmt) != SQLITE_DONE) FINALIZE_ERR(stmt, db);
    sqlite3_finalize(stmt);
    printf("Aeropuerto actualizado correctamente.\n");
    return 0;
}

int db_aeropuerto_buscar_id(void *db, int id_a,
                            char *codigo_out, char *nombre_out,
                            char *ciudad_out)
{
    sqlite3_stmt *stmt;
    PREP(db, "SELECT CODIGO,NOMBRE,CIUDAD FROM Aeropuerto WHERE ID_A=?;",
         stmt);
    sqlite3_bind_int(stmt,1,id_a);
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        safe_copy(codigo_out, sqlite3_column_text(stmt,0), 16);
        safe_copy(nombre_out, sqlite3_column_text(stmt,1), 128);
        safe_copy(ciudad_out, sqlite3_column_text(stmt,2), 128);
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 1 : -1;
}

int db_aeropuerto_buscar_codigo(void *db, const char *codigo,
                                int *id_out, char *nombre_out,
                                char *ciudad_out)
{
    sqlite3_stmt *stmt;
    PREP(db, "SELECT ID_A,NOMBRE,CIUDAD FROM Aeropuerto WHERE CODIGO=?;",
         stmt);
    sqlite3_bind_text(stmt,1,codigo,-1,SQLITE_STATIC);
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        if (id_out) *id_out = sqlite3_column_int(stmt,0);
        safe_copy(nombre_out, sqlite3_column_text(stmt,1), 128);
        safe_copy(ciudad_out, sqlite3_column_text(stmt,2), 128);
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 1 : -1;
}

int db_aeropuerto_listar(void *db) {
    sqlite3_stmt *stmt;
    PREP(db, "SELECT ID_A,CODIGO,NOMBRE,CIUDAD FROM Aeropuerto;", stmt);
    printf("\n%-5s %-8s %-35s %s\n","ID","Código","Nombre","Ciudad");
    printf("%-65s\n","---");
    int rc;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("%-5d %-8s %-35s %s\n",
               sqlite3_column_int(stmt,0),
               sqlite3_column_text(stmt,1),
               sqlite3_column_text(stmt,2),
               sqlite3_column_text(stmt,3));
    }
    sqlite3_finalize(stmt);
    fflush(stdout);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

//Vuelo

int db_vuelo_insertar(void *db,
                      const char *cod_vuelo,
                      int id_origen, int id_destino,
                      const char *fecha_hora,
                      double precio, int capacidad,
                      int *id_out)
{
    sqlite3_stmt *stmt;
    PREP(db, "INSERT INTO Vuelo(COD_VUELO,ID_ORIGEN,ID_DESTINO,"
             "FECHA_HORA,PRECIO,CAPACIDAD) VALUES(?,?,?,?,?,?);", stmt);
    sqlite3_bind_text  (stmt,1,cod_vuelo, -1,SQLITE_STATIC);
    sqlite3_bind_int   (stmt,2,id_origen);
    sqlite3_bind_int   (stmt,3,id_destino);
    sqlite3_bind_text  (stmt,4,fecha_hora,-1,SQLITE_STATIC);
    sqlite3_bind_double(stmt,5,precio);
    sqlite3_bind_int   (stmt,6,capacidad);
    if (sqlite3_step(stmt) != SQLITE_DONE) FINALIZE_ERR(stmt, db);
    if (id_out) *id_out = (int)sqlite3_last_insert_rowid(DB(db));
    sqlite3_finalize(stmt);
    printf("Vuelo creado correctamente.\n");
    return 0;
}

int db_vuelo_eliminar(void *db, int id_vuelo) {
    sqlite3_stmt *stmt;
    PREP(db, "DELETE FROM Vuelo WHERE ID_VUELO=?;", stmt);
    sqlite3_bind_int(stmt,1,id_vuelo);
    if (sqlite3_step(stmt) != SQLITE_DONE) FINALIZE_ERR(stmt, db);
    sqlite3_finalize(stmt);
    printf("Vuelo eliminado correctamente.\n");
    return 0;
}

int db_vuelo_actualizar(void *db, int id_vuelo,
                        const char *cod_vuelo,
                        int id_origen, int id_destino,
                        const char *fecha_hora,
                        double precio, int capacidad)
{
    sqlite3_stmt *stmt;
    PREP(db, "UPDATE Vuelo SET COD_VUELO=?,ID_ORIGEN=?,ID_DESTINO=?,"
             "FECHA_HORA=?,PRECIO=?,CAPACIDAD=? WHERE ID_VUELO=?;", stmt);
    sqlite3_bind_text  (stmt,1,cod_vuelo, -1,SQLITE_STATIC);
    sqlite3_bind_int   (stmt,2,id_origen);
    sqlite3_bind_int   (stmt,3,id_destino);
    sqlite3_bind_text  (stmt,4,fecha_hora,-1,SQLITE_STATIC);
    sqlite3_bind_double(stmt,5,precio);
    sqlite3_bind_int   (stmt,6,capacidad);
    sqlite3_bind_int   (stmt,7,id_vuelo);
    if (sqlite3_step(stmt) != SQLITE_DONE) FINALIZE_ERR(stmt, db);
    sqlite3_finalize(stmt);
    printf("Vuelo actualizado correctamente.\n");
    return 0;
}

int db_vuelo_buscar_id(void *db, int id_vuelo,
                       char *cod_vuelo_out, int *id_origen_out,
                       int *id_destino_out, char *fecha_hora_out,
                       double *precio_out, int *capacidad_out)
{
    sqlite3_stmt *stmt;
    PREP(db, "SELECT COD_VUELO,ID_ORIGEN,ID_DESTINO,FECHA_HORA,PRECIO,CAPACIDAD "
             "FROM Vuelo WHERE ID_VUELO=?;", stmt);
    sqlite3_bind_int(stmt,1,id_vuelo);
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        safe_copy(cod_vuelo_out,  sqlite3_column_text(stmt,0), 32);
        if (id_origen_out)  *id_origen_out  = sqlite3_column_int(stmt,1);
        if (id_destino_out) *id_destino_out = sqlite3_column_int(stmt,2);
        safe_copy(fecha_hora_out, sqlite3_column_text(stmt,3), 32);
        if (precio_out)     *precio_out     = sqlite3_column_double(stmt,4);
        if (capacidad_out)  *capacidad_out  = sqlite3_column_int(stmt,5);
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 1 : -1;
}

int db_vuelo_listar(void *db) {
    sqlite3_stmt *stmt;
    PREP(db, "SELECT ID_VUELO,COD_VUELO,ID_ORIGEN,ID_DESTINO,"
             "FECHA_HORA,PRECIO,CAPACIDAD FROM Vuelo;", stmt);
    printf("\n%-5s %-10s %-6s %-6s %-20s %-8s %s\n",
           "ID","Código","Orig","Dest","Fecha/Hora","Precio","Cap.");
    printf("%-70s\n","---");
    int rc;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("%-5d %-10s %-6d %-6d %-20s %-8.2f %d\n",
               sqlite3_column_int   (stmt,0),
               sqlite3_column_text  (stmt,1),
               sqlite3_column_int   (stmt,2),
               sqlite3_column_int   (stmt,3),
               sqlite3_column_text  (stmt,4),
               sqlite3_column_double(stmt,5),
               sqlite3_column_int   (stmt,6));
    }
    sqlite3_finalize(stmt);
    fflush(stdout);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

int db_vuelo_asientos_disponibles(void *db, int id_vuelo, int *disp_out) {
    sqlite3_stmt *stmt;
    PREP(db, "SELECT v.CAPACIDAD - COUNT(b.ID_BILLETE) "
             "FROM Vuelo v LEFT JOIN Billete b ON b.ID_VUELO=v.ID_VUELO "
             "WHERE v.ID_VUELO=? GROUP BY v.ID_VUELO;", stmt);
    sqlite3_bind_int(stmt,1,id_vuelo);
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        if (disp_out) *disp_out = sqlite3_column_int(stmt,0);
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);
    return 1;
}

//Billete

int db_billete_insertar(void *db,
                        int id_usuario, int id_vuelo,
                        const char *asiento, const char *fecha_compra,
                        int *id_out)
{
    sqlite3_stmt *stmt;
    PREP(db, "INSERT INTO Billete(ID_USUARIO,ID_VUELO,ASIENTO,FECHA_COMPRA) "
             "VALUES(?,?,?,?);", stmt);
    sqlite3_bind_int (stmt,1,id_usuario);
    sqlite3_bind_int (stmt,2,id_vuelo);
    sqlite3_bind_text(stmt,3,asiento,     -1,SQLITE_STATIC);
    sqlite3_bind_text(stmt,4,fecha_compra,-1,SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE) FINALIZE_ERR(stmt, db);
    if (id_out) *id_out = (int)sqlite3_last_insert_rowid(DB(db));
    sqlite3_finalize(stmt);
    printf("Billete creado correctamente.\n");
    return 0;
}

int db_billete_eliminar(void *db, int id_billete) {
    sqlite3_stmt *stmt;
    PREP(db, "DELETE FROM Billete WHERE ID_BILLETE=?;", stmt);
    sqlite3_bind_int(stmt,1,id_billete);
    if (sqlite3_step(stmt) != SQLITE_DONE) FINALIZE_ERR(stmt, db);
    sqlite3_finalize(stmt);
    printf("Billete eliminado correctamente.\n");
    return 0;
}

int db_billete_buscar_id(void *db, int id_billete,
                         int *id_usuario_out, int *id_vuelo_out,
                         char *asiento_out, char *fecha_compra_out)
{
    sqlite3_stmt *stmt;
    PREP(db, "SELECT ID_USUARIO,ID_VUELO,ASIENTO,FECHA_COMPRA "
             "FROM Billete WHERE ID_BILLETE=?;", stmt);
    sqlite3_bind_int(stmt,1,id_billete);
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        if (id_usuario_out)  *id_usuario_out = sqlite3_column_int(stmt,0);
        if (id_vuelo_out)    *id_vuelo_out   = sqlite3_column_int(stmt,1);
        safe_copy(asiento_out,      sqlite3_column_text(stmt,2), 16);
        safe_copy(fecha_compra_out, sqlite3_column_text(stmt,3), 32);
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 1 : -1;
}

int db_billete_listar_por_usuario(void *db, int id_usuario) {
    sqlite3_stmt *stmt;
    PREP(db, "SELECT ID_BILLETE,ID_VUELO,ASIENTO,FECHA_COMPRA "
             "FROM Billete WHERE ID_USUARIO=?;", stmt);
    sqlite3_bind_int(stmt,1,id_usuario);
    printf("\n%-8s %-8s %-8s %s\n","Billete","Vuelo","Asiento","Fecha compra");
    printf("%-50s\n","---");
    int rc;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("%-8d %-8d %-8s %s\n",
               sqlite3_column_int (stmt,0),
               sqlite3_column_int (stmt,1),
               sqlite3_column_text(stmt,2),
               sqlite3_column_text(stmt,3));
    }
    sqlite3_finalize(stmt);
    fflush(stdout);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

int db_billete_listar_por_vuelo(void *db, int id_vuelo) {
    sqlite3_stmt *stmt;
    PREP(db, "SELECT ID_BILLETE,ID_USUARIO,ASIENTO,FECHA_COMPRA "
             "FROM Billete WHERE ID_VUELO=?;", stmt);
    sqlite3_bind_int(stmt,1,id_vuelo);
    printf("\n%-8s %-8s %-8s %s\n","Billete","Usuario","Asiento","Fecha compra");
    printf("%-50s\n","---");
    int rc;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("%-8d %-8d %-8s %s\n",
               sqlite3_column_int (stmt,0),
               sqlite3_column_int (stmt,1),
               sqlite3_column_text(stmt,2),
               sqlite3_column_text(stmt,3));
    }
    sqlite3_finalize(stmt);
    fflush(stdout);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

//Equipaje

int db_equipaje_insertar(void *db,
                         int id_billete,
                         const char *tipo, double peso,
                         const char *descripcion,
                         int *id_out)
{
    sqlite3_stmt *stmt;
    PREP(db, "INSERT INTO Equipaje(ID_BILLETE,TIPO,PESO,DESCRIPCION) "
             "VALUES(?,?,?,?);", stmt);
    sqlite3_bind_int   (stmt,1,id_billete);
    sqlite3_bind_text  (stmt,2,tipo,       -1,SQLITE_STATIC);
    sqlite3_bind_double(stmt,3,peso);
    sqlite3_bind_text  (stmt,4,descripcion,-1,SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE) FINALIZE_ERR(stmt, db);
    if (id_out) *id_out = (int)sqlite3_last_insert_rowid(DB(db));
    sqlite3_finalize(stmt);
    printf("Equipaje registrado correctamente.\n");
    return 0;
}

int db_equipaje_eliminar(void *db, int id_equipaje) {
    sqlite3_stmt *stmt;
    PREP(db, "DELETE FROM Equipaje WHERE ID_EQUIPAJE=?;", stmt);
    sqlite3_bind_int(stmt,1,id_equipaje);
    if (sqlite3_step(stmt) != SQLITE_DONE) FINALIZE_ERR(stmt, db);
    sqlite3_finalize(stmt);
    printf("Equipaje eliminado correctamente.\n");
    return 0;
}

int db_equipaje_buscar_id(void *db, int id_equipaje,
                          int *id_billete_out, char *tipo_out,
                          double *peso_out, char *descripcion_out)
{
    sqlite3_stmt *stmt;
    PREP(db, "SELECT ID_BILLETE,TIPO,PESO,DESCRIPCION "
             "FROM Equipaje WHERE ID_EQUIPAJE=?;", stmt);
    sqlite3_bind_int(stmt,1,id_equipaje);
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        if (id_billete_out) *id_billete_out = sqlite3_column_int(stmt,0);
        safe_copy(tipo_out,        sqlite3_column_text(stmt,1), 32);
        if (peso_out)       *peso_out       = sqlite3_column_double(stmt,2);
        safe_copy(descripcion_out, sqlite3_column_text(stmt,3), 256);
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 1 : -1;
}

int db_equipaje_listar_por_billete(void *db, int id_billete) {
    sqlite3_stmt *stmt;
    PREP(db, "SELECT ID_EQUIPAJE,TIPO,PESO,DESCRIPCION "
             "FROM Equipaje WHERE ID_BILLETE=?;", stmt);
    sqlite3_bind_int(stmt,1,id_billete);
    printf("\n%-8s %-12s %-8s %s\n","ID","Tipo","Peso(kg)","Descripción");
    printf("%-60s\n","---");
    int rc;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("%-8d %-12s %-8.2f %s\n",
               sqlite3_column_int   (stmt,0),
               sqlite3_column_text  (stmt,1),
               sqlite3_column_double(stmt,2),
               sqlite3_column_text  (stmt,3));
    }
    sqlite3_finalize(stmt);
    fflush(stdout);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

//Consultas join

int db_pasajeros_por_vuelo(void *db, int id_vuelo) {
    sqlite3_stmt *stmt;
    PREP(db,
        "SELECT DISTINCT u.ID_US, u.NOMBRE, u.EMAIL, u.TLF "
        "FROM Usuario u "
        "JOIN Billete b ON b.ID_USUARIO = u.ID_US "
        "WHERE b.ID_VUELO = ?;", stmt);
    sqlite3_bind_int(stmt,1,id_vuelo);
    printf("\nPasajeros del vuelo %d:\n", id_vuelo);
    printf("%-5s %-25s %-30s %s\n","ID","Nombre","Email","Teléfono");
    printf("%-70s\n","---");
    int rc;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("%-5d %-25s %-30s %lld\n",
               sqlite3_column_int  (stmt,0),
               sqlite3_column_text (stmt,1),
               sqlite3_column_text (stmt,2),
               sqlite3_column_int64(stmt,3));
    }
    sqlite3_finalize(stmt);
    fflush(stdout);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

int db_equipaje_por_usuario(void *db, int id_usuario) {
    sqlite3_stmt *stmt;
    PREP(db,
        "SELECT e.ID_EQUIPAJE, e.TIPO, e.PESO, e.DESCRIPCION "
        "FROM Equipaje e "
        "JOIN Billete b ON b.ID_BILLETE = e.ID_BILLETE "
        "WHERE b.ID_USUARIO = ?;", stmt);
    sqlite3_bind_int(stmt,1,id_usuario);
    printf("\nEquipaje del usuario %d:\n", id_usuario);
    printf("%-8s %-12s %-8s %s\n","ID","Tipo","Peso(kg)","Descripción");
    printf("%-60s\n","---");
    int rc;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("%-8d %-12s %-8.2f %s\n",
               sqlite3_column_int   (stmt,0),
               sqlite3_column_text  (stmt,1),
               sqlite3_column_double(stmt,2),
               sqlite3_column_text  (stmt,3));
    }
    sqlite3_finalize(stmt);
    fflush(stdout);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

//Carga de csvs

int db_cargar_aeropuertos_csv(void *db, const char *ruta) {
    FILE *f = fopen(ruta, "r");
    if (!f) { fprintf(stderr,"[DB] No se pudo abrir '%s'.\n", ruta); return -1; }
    char linea[512];
    if (!fgets(linea, sizeof(linea), f)) { fclose(f); return 0; } /* cabecera */

    exec_simple(DB(db), "BEGIN TRANSACTION;");
    int n = 0;
    while (fgets(linea, sizeof(linea), f)) {
        trim_nl(linea);
        if (!linea[0]) continue;
        char codigo[16]={0}, nombre[128]={0}, ciudad[128]={0};
        char *tok = strtok(linea, ","); if (!tok) continue;
        strncpy(codigo, tok, 15);
        tok = strtok(NULL,","); if (tok) strncpy(nombre, tok, 127);
        tok = strtok(NULL,","); if (tok) strncpy(ciudad, tok, 127);
        if (db_aeropuerto_insertar(db, codigo, nombre, ciudad, NULL) == 0) n++;
    }
    exec_simple(DB(db), "COMMIT;");
    fclose(f);
    printf("[DB] %d aeropuertos cargados desde '%s'.\n", n, ruta);
    return n;
}

int db_cargar_vuelos_csv(void *db, const char *ruta) {
    FILE *f = fopen(ruta, "r");
    if (!f) { fprintf(stderr,"[DB] No se pudo abrir '%s'.\n", ruta); return -1; }
    char linea[512];
    if (!fgets(linea, sizeof(linea), f)) { fclose(f); return 0; }

    exec_simple(DB(db), "BEGIN TRANSACTION;");
    int n = 0;
    while (fgets(linea, sizeof(linea), f)) {
        trim_nl(linea);
        if (!linea[0]) continue;
        char cod[32]={0}, cod_orig[16]={0}, cod_dest[16]={0}, fh[32]={0};
        double precio = 0.0;
        int cap = 0;
        char *tok = strtok(linea,","); if (!tok) continue; strncpy(cod,tok,31);
        tok = strtok(NULL,","); if (tok) strncpy(cod_orig,tok,15);
        tok = strtok(NULL,","); if (tok) strncpy(cod_dest,tok,15);
        tok = strtok(NULL,","); if (tok) strncpy(fh,tok,31);
        tok = strtok(NULL,","); if (tok) precio = atof(tok);
        tok = strtok(NULL,","); if (tok) cap    = atoi(tok);

        int id_orig = 0, id_dest = 0;
        if (db_aeropuerto_buscar_codigo(db, cod_orig, &id_orig, NULL, NULL) != 0) continue;
        if (db_aeropuerto_buscar_codigo(db, cod_dest, &id_dest, NULL, NULL) != 0) continue;
        if (db_vuelo_insertar(db, cod, id_orig, id_dest, fh, precio, cap, NULL) == 0) n++;
    }
    exec_simple(DB(db), "COMMIT;");
    fclose(f);
    printf("[DB] %d vuelos cargados desde '%s'.\n", n, ruta);
    return n;
}

int db_cargar_usuarios_csv(void *db, const char *ruta) {
    FILE *f = fopen(ruta, "r");
    if (!f) { fprintf(stderr,"[DB] No se pudo abrir '%s'.\n", ruta); return -1; }
    char linea[512];
    if (!fgets(linea, sizeof(linea), f)) { fclose(f); return 0; }

    exec_simple(DB(db), "BEGIN TRANSACTION;");
    int n = 0;
    while (fgets(linea, sizeof(linea), f)) {
        trim_nl(linea);
        if (!linea[0]) continue;
        char nombre[128]={0}, email[128]={0}, contrasena[256]={0};
        long long tlf = 0;
        int rol = 2;
        char *tok = strtok(linea,","); if (!tok) continue; strncpy(nombre,tok,127);
        tok = strtok(NULL,","); if (tok) strncpy(email,tok,127);
        tok = strtok(NULL,","); if (tok) tlf = atoll(tok);
        tok = strtok(NULL,","); if (tok) strncpy(contrasena,tok,255);
        tok = strtok(NULL,","); if (tok) rol = atoi(tok);
        if (db_usuario_insertar(db, nombre, email, tlf, contrasena, rol, NULL) == 0) n++;
    }
    exec_simple(DB(db), "COMMIT;");
    fclose(f);
    printf("[DB] %d usuarios cargados desde '%s'.\n", n, ruta);
    return n;
}
