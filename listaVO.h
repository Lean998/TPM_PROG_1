#ifndef LISTAVO_H_INCLUDED
#define LISTAVO_H_INCLUDED
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "turno.h"
//Lista de turnos

typedef struct NodoTurno{
    Turno turno;
    struct NodoTurno *siguiente;
}NodoTurno;

typedef struct{
    NodoTurno *acc;
    NodoTurno *cur;
    NodoTurno *aux;
}ListaVo;

// Inicializar la lista
void inicializarLista(ListaVo *lista){
    lista->acc = NULL;
    lista->cur = NULL;
    lista->aux = NULL;
}

void agregarTurno(ListaVo *lista, Turno turno) {
    NodoTurno *nuevoNodo = (NodoTurno*)malloc(sizeof(NodoTurno));
    nuevoNodo->turno = turno;
    nuevoNodo->siguiente = NULL;

    Fecha fechaNueva = get_turno_fecha(turno);
    NodoTurno *anterior = NULL;
    NodoTurno *actual = lista->acc;

    // Avanzar mientras la fecha del nodo actual sea menor o igual a la nueva
    while(actual != NULL && comparar_fechas(get_turno_fecha(actual->turno), fechaNueva) <= 0){
        // Avanzar en la lista
        anterior = actual;
        actual = actual->siguiente;
    }

    // Agregar el nuevo nodo
    nuevoNodo->siguiente = actual;
    if(anterior == NULL){ // Va al principio
        lista->acc = nuevoNodo;
    } else {// Va en el medio o al final
        anterior->siguiente = nuevoNodo;
    }
    lista->cur = nuevoNodo;
    lista->aux = anterior;
}

// Comprobar si la lista está vacía
int isEmpty(ListaVo lista){
    return (lista.acc == NULL);
}

// Comprobar si la lista está llena
int isFull(ListaVo lista){
    NodoTurno *nuevoNodo = (NodoTurno*)malloc(sizeof(NodoTurno));
    if(nuevoNodo == NULL){
        return 1; // Lista llena
    } else {
        free(nuevoNodo);
        return 0; // Lista no llena
    }
}

//Comprobar si el cursor está fuera de la lista
int isOos(ListaVo lista){
    return (lista.cur == NULL);
}

// Eliminar el turno actual
void suppressTurno(ListaVo *lista){
    if(lista->cur == lista->acc){
        lista->acc = lista->cur->siguiente;
        free(lista->cur);
        lista->cur = lista->acc;
    }
    else{
        lista->aux->siguiente = lista->cur->siguiente;
        free(lista->cur);
        lista->cur = lista->aux->siguiente;
    }
}

Turno copyTurno(ListaVo lista){
    return lista.cur->turno;
}

void reset(ListaVo *lista) {
    lista->cur = lista->acc;
    lista->aux = NULL;
}

void forward(ListaVo *lista){
    lista->aux = lista->cur;
    lista->cur = lista->cur->siguiente;
}

//Buscar un id_turno e indicar si existe o no.
int buscarIdTurno(ListaVo lista, int id_buscar) {
    reset(&lista);
    while (!isOos(lista)) {
        Turno turno = copyTurno(lista);
        if (get_turno_id(turno) == id_buscar) {
            return 1; // Encontrado
        }
        forward(&lista);
    }
    return 0; // No encontrado
}

#endif // LISTAVO_H_INCLUDED
