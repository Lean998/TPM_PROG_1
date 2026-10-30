#ifndef LISTAVO_H_INCLUDED
#define LISTAVO_H_INCLUDED

#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "turno.h"

typedef struct NodoTurno{
    Turno turno;
    struct NodoTurno *siguiente;
} NodoTurno;

typedef struct{
    NodoTurno *acc;
    NodoTurno *cur;
    NodoTurno *aux;
}ListaVo;

// Inicializar la lista
void inicializarLista(ListaVo *lista){
  //  if (lista != NULL) {
        lista->acc = NULL;
        lista->cur = NULL;
        lista->aux = NULL;
   // }
}

// compruebo si la lista esta vacia
int isEmpty(ListaVo lista){
    return (lista.acc == NULL);
}

// Ccompruebo si la lista esta llena
int isFull(ListaVo lista){
    NodoTurno *nuevoNodo = (NodoTurno*)malloc(sizeof(NodoTurno));
    if(nuevoNodo == NULL){
        return 1; // memoria insuficiente
    } else {
        free(nuevoNodo);
        return 0; // todavia hay memoria disponible
    }
}

// compruebo si el cursor esta fuera de la lista
int isOos(ListaVo lista){
    return (lista.cur == NULL);
}

void agregarTurno(ListaVo *l, Turno turno) {
   // if (lista == NULL) return;

    NodoTurno *nuevoNodo = (NodoTurno*)malloc(sizeof(NodoTurno));
    if (nuevoNodo == NULL) return; // controlo la memoria

    nuevoNodo->turno = turno;
    nuevoNodo->siguiente = l->cur;

    if(l->aux == NULL){ // Va al principio l->cursor==l->aux
        l->acc = nuevoNodo;
        l->cur=l->acc;
        l->aux=NULL;
    } else { // Va en el medio o al final
        l->aux->siguiente = nuevoNodo;
    }
    l->cur = nuevoNodo;
}

// creo la funcion suppressturno para eliminar el turno actual posicionado en el cursor
void suppressTurno(ListaVo *lista){
    if (lista == NULL || isOos(*lista)) return;

    if(lista->cur == lista->acc){
        lista->acc = lista->cur->siguiente;
        free(lista->cur);
        lista->cur = lista->acc;
        lista->aux= lista->acc;
    } else {
        lista->aux->siguiente = lista->cur->siguiente;
        free(lista->cur);
        lista->cur = lista->aux->siguiente;
    }
}

// Retornar el turno actual (con validación de seguridad)
Turno copyTurno(ListaVo lista){
    Turno t;
    init_turno(&t); // Turno vacío por defecto si está OOS
    if (!isOos(lista)) {
        t = lista.cur->turno;
    }
    return t;
}

// Reiniciar el cursor al inicio de la lista
void reset(ListaVo *lista) {
    if (lista != NULL) {
        lista->cur = lista->acc;
        lista->aux = NULL;
    }
}

// Avanzar el cursor al siguiente nodo
void forward(ListaVo *lista){
    if (lista != NULL && !isOos(*lista)) {
        lista->aux = lista->cur;
        lista->cur = lista->cur->siguiente;
   }
}

// Buscar un id_turno e indicar si existe o no (1 = Encontrado, 0 = No)
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
