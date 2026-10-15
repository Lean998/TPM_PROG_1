#ifndef TURNO_H
#define TURNO_H

#include <stdio.h>
#include <string.h>
#include "fecha.h"

typedef struct{
    int id_turno;
    long int dni_paciente;
    float monto;
    char mutual[30];
    fecha fecha; // minúscula para que coincida con fecha.h
    char estado[30];
} turno;

void initTurno(turno *t){
    t->id_turno = -1;
    t->dni_paciente = 0;
    t->monto = 0.0;
    strcpy(t->mutual, "");
    strcpy(t->estado, "solicitado");
}

int mutualValida (char mutual[]){
    if (strcasecmp(mutual, "opcion 1")== 0 ||
        strcasecmp(mutual, "opcion 2")== 0 ||
        strcasecmp(mutual, "opcion 3")== 0 ||
        strcasecmp(mutual, "opcion 4")== 0){
            return 1;
        }
        return 0;
}

void cargaTurno(turno *t, fecha *f, int id, long int dni, float monto, char mutual[], char estado[]){
    t->id_turno = id;
    t->dni_paciente = dni;
    t->monto = monto;
    if (strlen(mutual)>0){
        if(mutualValida(mutual)){
            strcpy(t->mutual,mutual);
        }
        else{
            strcpy(t->mutual, "opcion 1"), //si no es valida la opcion por defecto seleccionamos opcion 1
            printf("la obra social no es valida (mutual). se le asigna opcion 1.");
        }
    }
    else {
        strcpy(t->mutual,"");
    }
    strcpy(t->estado, estado);
    t->fecha = *f; // Desreferenciamos el puntero f
}

void modificarestadoTurno(turno *t, char nuevoestado[], char nuevamutual[], int nuevomonto){
    if (strcmp(t->estado, "atendido") == 0){
        printf("\nEl turno ya esta atendido. No se permite modificar.");
        return;
    }
    strcpy(t->estado, nuevoestado);
    if (strlen(nuevamutual) > 0){
        if(mutualValida(nuevamutual)){
            strcpy(t->mutual, nuevamutual);
        }
    }
    t->monto = nuevomonto;
}

long int RetornarDniPaciente(turno t){
    return t.dni_paciente;
}

int ReturnIdPaciente(turno t){
    return t.id_turno;
}

char* RetornarEstado(turno t, char destinoEstado[]){
    strcpy(destinoEstado, t.estado);
    return destinoEstado;
}

void mostrarTurno(turno t){
    printf("Informacion sobre el turno: \n");
    printf("ID turno: %d\n", t.id_turno);
    printf("DNI paciente: %ld\n", t.dni_paciente);
    printf("Estado: %s\n", t.estado);
    if (strlen(t.mutual) > 0){
        printf("Obra social: %s\n", t.mutual);
    } else {
        printf("Monto pagado: %.2f\n", t.monto);
    }
    printf("------------------------\n");
}

#endif // TURNO_H
