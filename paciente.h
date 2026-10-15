#ifndef PACIENTE_H
#define PACIENTE_H
#include <stdio.h>
#include <string.h>
typedef struct{
    char nombre [30];
    char apellido [30];
    long dni;
    int osocial; // aca defino si tiene. 1 = si tiene y 0 = no tiene

}paciente; // defini la estructura de pacientes.

void initpaciente (paciente *p){
    strcpy(p->nombre, "");
    strcpy(p->apellido, "");
    p->dni = 0;
    p->osocial = 0;
}
void modificarpaciente (paciente *p, char nuevonombre[], char nuevoapellido[], long nuevodni, int obrasocial){
    strcpy(p->nombre,nuevonombre);
    strcpy(p->apellido,nuevoapellido);
    p->dni = nuevodni;
    p->osocial = obrasocial;
}

void mostrarpaciente (paciente p){
    printf("datos del paciente: \n");
    printf("nombre: %s\n", p.nombre);
    printf("apellido: %s\n", p.apellido);
    printf("DNI: %ld\n", p.dni);
    if (p.osocial == 1){
        printf("si tiene obra social\n");
    }
    else{
        printf("no tiene obra social\n");
    }
    printf("----------------------");
}

#endif // termino de declarar paciente.h
