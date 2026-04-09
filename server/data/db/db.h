
#ifndef SERVER_DATA_DB_DB_H_
#define SERVER_DATA_DB_DB_H_

#ifdef __cplusplus
extern "C"
{
#endif

//Conexión
void *db_abrir(const char *ruta); // devuelve sqlite3* casteado a void*
void  db_cerrar(void *db);
int   db_inicializar(void *db);   // crea tablas si no existen
int   db_activar_fk(void *db);

//Usuario
int db_usuario_insertar(void *db, const char *dni,
                        const char *nombre, const char *email,
                        long long tlf, const char *contrasena, int rol,
                        int *id_out);

int db_usuario_eliminar(void *db, int id_us);

int db_usuario_actualizar(void *db, int id_us,
                          const char *nombre, const char *email,
                          long long tlf, const char *contrasena, int rol);

int db_usuario_buscar_id(void *db, int id_us,
                         char *nombre_out,
                         char *email_out,
                         long long *tlf_out,
                         char *contrasena_out,
                         int  *rol_out);

int db_usuario_buscar_email(void *db, const char *email,
                            int  *id_out,
                            char *nombre_out,
                            long long *tlf_out,
                            char *contrasena_out,
                            int  *rol_out);

int db_usuario_listar(void *db);

//Aeropuerto
int db_aeropuerto_insertar(void *db,
                           const char *codigo, const char *nombre,
                           const char *ciudad, int *id_out);

int db_aeropuerto_eliminar(void *db, int id_a);

int db_aeropuerto_actualizar(void *db, int id_a,
                             const char *codigo, const char *nombre,
                             const char *ciudad);

int db_aeropuerto_buscar_id(void *db, int id_a,
                            char *codigo_out,
                            char *nombre_out,
                            char *ciudad_out);

int db_aeropuerto_buscar_codigo(void *db, const char *codigo,
                                int  *id_out,
                                char *nombre_out,
                                char *ciudad_out);

int db_aeropuerto_listar(void *db);

//Vuelo
int db_vuelo_insertar(void *db,
                      const char *cod_vuelo,
                      int id_origen, int id_destino,
                      const char *fecha_hora,
                      double precio, int capacidad,
                      int *id_out);

int db_vuelo_eliminar(void *db, int id_vuelo);

int db_vuelo_actualizar(void *db, int id_vuelo,
                        const char *cod_vuelo,
                        int id_origen, int id_destino,
                        const char *fecha_hora,
                        double precio, int capacidad);

int db_vuelo_buscar_id(void *db, int id_vuelo,
                       char   *cod_vuelo_out,
                       int    *id_origen_out,
                       int    *id_destino_out,
                       char   *fecha_hora_out,
                       double *precio_out,
                       int    *capacidad_out);

int db_vuelo_listar(void *db);
int db_vuelo_asientos_disponibles(void *db, int id_vuelo, int *disp_out);

//Billete
int db_billete_insertar(void *db,
                        int id_usuario, int id_vuelo,
                        const char *asiento, const char *fecha_compra,
                        int *id_out);

int db_billete_eliminar(void *db, int id_billete);

int db_billete_buscar_id(void *db, int id_billete,
                         int  *id_usuario_out,
                         int  *id_vuelo_out,
                         char *asiento_out,
                         char *fecha_compra_out);

int db_billete_listar_por_usuario(void *db, int id_usuario);
int db_billete_listar_por_vuelo(void *db, int id_vuelo);

//Equipaje
int db_equipaje_insertar(void *db,
                         const char *codigo, const char *dni,
                         int id_vuelo, double peso,
                         const char *estado,
                         int *id_out);

int db_equipaje_eliminar(void *db, int id_equipaje);

int db_equipaje_buscar_id(void *db, int id_equipaje,
                          int    *id_billete_out,
                          char   *tipo_out,
                          double *peso_out,
                          char   *descripcion_out);

int db_equipaje_listar_por_billete(void *db, int id_billete);

int db_equipaje_listar(void *db);

//Consultas join
int db_pasajeros_por_vuelo(void *db, int id_vuelo);
int db_equipaje_por_usuario(void *db, int id_usuario);

//Cragar csvs
int db_cargar_aeropuertos_csv(void *db, const char *ruta);
int db_cargar_vuelos_csv(void *db, const char *ruta);
int db_cargar_usuarios_csv(void *db, const char *ruta);
int db_cargar_equipajes_csv(void *db, const char *ruta);

#ifdef __cplusplus
}
#endif

#endif /* SERVER_DATA_DB_DB_H_ */
