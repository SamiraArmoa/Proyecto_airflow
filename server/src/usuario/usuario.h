#ifndef SERVER_SRC_USUARIO_USUARIO_H_
#define SERVER_SRC_USUARIO_USUARIO_H_

typedef struct {
    int id;
    char nombre[100];
    char email[100];
    int telefono;
    char contrasena[100];
} Usuario;

int crearUsuario();
int eliminarUsuario();
int actualizarUsuario();
//int imprimirUsuarios();

#endif /* SERVER_SRC_USUARIO_USUARIO_H_ */