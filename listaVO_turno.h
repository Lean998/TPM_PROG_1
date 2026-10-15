#ifndef LISTAVO_H
#define LISTAVO_H

#include <stdio.h>
#include <stdlib.h>
#include "fecha.h"
#include "turno.h"

typedef struct NodoTurno{
    turno dato; // Usamos minúscula para que coincida con el tipo 'turno'
    struct NodoTurno *sig;
} NodoTurno;

typedef struct{
    NodoTurno *acc;
    NodoTurno *cur;
    NodoTurno *ant;
} ListaVOTurno;

int compararFechasVO(fecha f1, fecha f2){
    return (f1.anio != f2.anio) ? (f1.anio - f2.anio):
           (f1.mes != f2.mes)   ? (f1.mes - f2.mes):
           (f1.dia != f2.dia)   ? (f1.dia - f2.dia):
           (f1.hora != f2.hora) ? (f1.hora - f2.hora):
           (f1.minutos - f2.minutos);
}

void initListaVOTurno(ListaVOTurno *l){
    l->acc = NULL;
    l->cur = NULL;
    l->ant = NULL;
}

int EmptyListaVOTurno(ListaVOTurno l){
    return (l.acc == NULL);
}

int FullListaVOTurno(ListaVOTurno l){
    NodoTurno *temp = (NodoTurno*)malloc(sizeof(NodoTurno));
    if (temp == NULL){
        return 1; // Memoria llena
    }
    free(temp);
    return 0; // Hay memoria disponible
}

void ResetListaVOTurno(ListaVOTurno *l){
    l->cur = l->acc;
    l->ant = NULL;
}

int isoosListaVOTurno(ListaVOTurno l){
    return (l.cur == NULL);
}

void forwardListaVOTurno(ListaVOTurno *l){
    if (!isoosListaVOTurno(*l)){
        l->ant = l->cur;
        l->cur = l->cur->sig;
    }
}

void copyListaVOTurno(ListaVOTurno l, turno *t){
    if (!isoosListaVOTurno(l)){
        *t = l.cur->dato;
    }
}

void insertListaVOTurno(ListaVOTurno *l, turno nuevoTurno){
    NodoTurno *nuevo = (NodoTurno *)malloc(sizeof(NodoTurno));
    if (nuevo == NULL){
        printf("Memoria insuficiente para insertar el turno.\n");
        return;
    }
    nuevo->dato = nuevoTurno;
    nuevo->sig = NULL;

    if (l->acc == NULL || compararFechasVO(nuevoTurno.fecha, l->acc->dato.fecha) <= 0){
        nuevo->sig = l->acc;
        l->acc = nuevo;
    } else {
        NodoTurno *aux = l->acc;
        NodoTurno *anterior = NULL;

        while (aux != NULL && compararFechasVO(aux->dato.fecha, nuevoTurno.fecha) <= 0){
            anterior = aux;
            aux = aux->sig;
        }
        nuevo->sig = aux;
        anterior->sig = nuevo;
    }
    l->cur = l->acc;
    l->ant = NULL;
}

int suppressListaVOTurno(ListaVOTurno *l){
    if (EmptyListaVOTurno(*l) || isoosListaVOTurno(*l)){
        printf("No hay ningun turno seleccionado para eliminar.\n");
        return 0;
    }
    NodoTurno *aEliminar = l->cur;
    if (l->cur == l->acc){
        l->acc = l->acc->sig;
    } else {
        l->ant->sig = l->cur->sig;
    }
    l->cur = l->cur->sig;
    free(aEliminar);
    return 1;
}

#endif // LISTAVO_H
