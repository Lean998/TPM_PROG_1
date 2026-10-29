#ifndef PACIENTE_H_INCLUDED
#define PACIENTE_H_INCLUDED

#include <string.h>
#include <malloc.h>

typedef struct{
    char nombre[100];
    char apellido[100];
    long int dni;
    int oSocial; //[0] No tiene [1] Tiene
}Paciente;

// FUNCION DE INICIAR PACIENTE
void init_paciente(Paciente *p) {
    strcpy(p->nombre, "");
    strcpy(p->apellido, "");
    p->dni = 0;
    p->oSocial = 0; // inicializamos por defecto que no cuenta con obra social
}

// Setters
void set_nombre_paciente(Paciente *p, char *nombre){
    if (strlen(nombre) > 0) {
        strcpy(p->nombre, nombre);
    }
}

void set_apellido_paciente(Paciente *p, char *apellido){
    if (strlen(apellido) > 0) {
        strcpy(p->apellido, apellido);
    }
}

void set_dni_paciente(Paciente *p, long int dni){
    if (dni > 0) {
        p->dni = dni;
    }
}

void set_o_social_paciente(Paciente *p, int oSocial){
    if (oSocial >= 0 && oSocial <= 4) {
        p->oSocial = oSocial;
    }
}

// Getters
char* get_nombre_paciente(Paciente p){
    char *nombre = (char*)malloc(sizeof(char) * strlen(p.nombre) + 1);
    if (nombre != NULL) {
        strcpy(nombre, p.nombre);
    }
    return nombre;
}

char* get_apellido_paciente(Paciente p){
    char *apellido = (char*)malloc(sizeof(char) * strlen(p.apellido) + 1);
    if (apellido != NULL) {
        strcpy(apellido, p.apellido);
    }
    return apellido;
}

long int get_dni_paciente(Paciente p){
    return p.dni;
}

int get_o_social_paciente(Paciente p){
    return p.oSocial;
}

#endif // PACIENTE_H_INCLUDED
