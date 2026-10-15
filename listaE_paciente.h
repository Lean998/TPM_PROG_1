#ifndef LISTAE_PACIENTE_H
#define LISTAE_PACIENTE_H

#include <stdio.h>
#include <stdlib.h>
#include "paciente.h"

#define MAX_PACIENTES 100

typedef struct{
    paciente v[MAX_PACIENTES];
    int cant;
    int cur;
} listaEpaciente;

void initListaE_paciente(listaEpaciente *e){
    e->cant = 0;
    e->cur = -1;
}

int EmptylistaE_paciente(listaEpaciente e){
    return (e.cant == 0);
}

int FulllistaEpaciente(listaEpaciente e){
    return (e.cant >= MAX_PACIENTES);
}

void ResetlistaEpaciente(listaEpaciente *e){
    if (e->cant > 0){
        e->cur = 0;
    } else {
        e->cur = -1;
    }
}

int isooslistaEpaciente(listaEpaciente e){
    return (e.cur == -1 || e.cur >= e.cant);
}

void forwardlistaEpaciente(listaEpaciente *e){
    if (!isooslistaEpaciente(*e)){
        e->cur++;
        if (e->cur >= e->cant){
            e->cur = -1;
        }
    }
}

void copylistaEpaciente(listaEpaciente e, paciente *p){
    if (!isooslistaEpaciente(e)){
        *p = e.v[e.cur];
    }
}

int insertlistaEpaciente(listaEpaciente *e, paciente nuevoPaciente){
    if (FulllistaEpaciente(*e)){
        printf("La lista estatica de pacientes esta llena.\n");
        return 0;
    }
    e->v[e->cant] = nuevoPaciente;
    e->cant++;
    if (e->cant == 1){
        e->cur = 0;
    }
    return 1;
}

int suppresslistaEpaciente(listaEpaciente *e){
    int i;
    if (EmptylistaE_paciente(*e) || isooslistaEpaciente(*e)){
        printf("No hay ningun paciente seleccionado para borrar.\n");
        return 0;
    }
    for (i = e->cur; i < e->cant - 1; i++){
        e->v[i] = e->v[i + 1];
    }
    e->cant--;
    if (e->cant == 0){
        e->cur = -1;
    } else if(e->cur >= e->cant){
        e->cur = e->cant - 1;
    }
    return 1;
}

int buscarPacientePorDni(listaEpaciente e, long dniBuscado, paciente *pEncontrado){
    int i = 0;
    while (i < e.cant){
        if(e.v[i].dni == dniBuscado){
            *pEncontrado = e.v[i];
            return 1;
        }
        i++;
    }
    return 0;
}

#endif // LISTAE_PACIENTE_H
