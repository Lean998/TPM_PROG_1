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

void set_nombre_paciente(Paciente *p, char *nombre){
    strcpy(p->nombre, nombre);
}

void set_apellido_paciente(Paciente *p, char *apellido){
    strcpy(p->apellido, apellido);
}

void set_dni_paciente(Paciente *p, long int dni){
    p->dni = dni;
}

void set_o_social_paciente(Paciente *p, int oSocial){
    p->oSocial = oSocial;
}

char* get_nombre_paciente(Paciente p){
    char *nombre;
    nombre = (char*)malloc(sizeof(char)*strlen(p.nombre)+1);
    strcpy(nombre, p.nombre);
    return nombre;
}

char* get_apellido_paciente(Paciente p){
    char *apellido;
    apellido = (char*)malloc(sizeof(char)*strlen(p.apellido)+1);
    strcpy(apellido, p.apellido);
    return apellido;
}

long int get_dni_paciente(Paciente p){
    return p.dni;
}

int get_o_social_paciente(Paciente p){
    return p.oSocial;
}

#endif // PACIENTE_H_INCLUDED
