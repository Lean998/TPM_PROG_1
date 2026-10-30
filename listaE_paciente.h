#ifndef LISTAE_PACIENTE_H_INCLUDED
#define LISTAE_PACIENTE_H_INCLUDED
#include <stdio.h>
#define MAX_PACIENTES 100

typedef struct{
    Paciente v[MAX_PACIENTES];
    int cant;
    int cur;
} ListaEpaciente;

void initListaE_paciente(ListaEpaciente *e){
    e->cant = 0;
    e->cur = -1;
}

int EmptylistaE_paciente(ListaEpaciente e){
    return (e.cant == 0);
}

int FullListaEpaciente(ListaEpaciente e){
    return (e.cant >= MAX_PACIENTES);
}

void resetListaEpaciente(ListaEpaciente *e){
    if (e->cant > 0){
        e->cur = 0;
    } else {
        e->cur = -1;
    }
}

int isoosListaEpaciente(ListaEpaciente e){
    return (e.cur < 0 || e.cur >= e.cant);
}

void forwardListaEpaciente(ListaEpaciente *e){
    if (!isoosListaEpaciente(*e)){
        e->cur++;
        if (e->cur >= e->cant){
            e->cur = -1;
        }
    }
}

Paciente copyListaEpaciente(ListaEpaciente e){
    Paciente p;
    if (!isoosListaEpaciente(e)){
        p = e.v[e.cur];
    }
    return p;
}

int insertListaEpaciente(ListaEpaciente *e, Paciente nuevoPaciente){
    if (FullListaEpaciente(*e)){
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

int suppressListaEpaciente(ListaEpaciente *e){
    int i;
    if (EmptylistaE_paciente(*e) || isoosListaEpaciente(*e)){
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

#endif // LISTAE_PACIENTE_H_INCLUDED
