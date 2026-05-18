#ifndef SERVER_SRC_EQUIPAJE_EQUIPAJE_H_
#define SERVER_SRC_EQUIPAJE_EQUIPAJE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_EQUIPAJES     3000
#define MAX_ID_EQ         20
#define MAX_TIPO_EQ       30

typedef struct {
    char id_equipaje[MAX_ID_EQ];
    char id_pasajero[20];
    char tipo[MAX_TIPO_EQ];
    float peso_kg;
    int  facturado;
} Equipaje;

int equipaje_registrar(Equipaje lista[], int *total,
                       const char *id_eq, const char *dni_pasajero,
                       const char *tipo, float peso_kg, int facturado);

int equipaje_eliminar(Equipaje lista[], int *total, const char *id_eq);

void equipaje_ver(const Equipaje lista[], int total);

int equipaje_buscar(const Equipaje lista[], int total, const char *id_eq);

void equipaje_ver_por_pasajero(const Equipaje lista[], int total,
                               const char *dni_pasajero);

int equipaje_cargar_csv(Equipaje lista[], int *total, const char *ruta);

int equipaje_guardar_csv(const Equipaje lista[], int total, const char *ruta);

#ifdef __cplusplus
}
#endif

#endif /* SERVER_SRC_EQUIPAJE_EQUIPAJE_H_ */