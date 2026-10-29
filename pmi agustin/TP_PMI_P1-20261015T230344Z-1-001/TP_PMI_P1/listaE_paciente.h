#ifndef LISTAE_PACIENTE_H_INCLUDED
#define LISTAE_PACIENTE_H_INCLUDED

#include "paciente.h"
#define MAX_PACIENTES 100

typedef struct{
    Paciente v[MAX_PACIENTES];
    int cant;
    int cur;
} ListaEpaciente;

// Inicializar la lista estática
void initListaE_paciente(ListaEpaciente *e){
    if (e != NULL) {
        e->cant = 0;//-1
        e->cur = -1;
    }
}

// compruebpo si la lista esta vacia
int EmptylistaE_paciente(ListaEpaciente e){
    return (e.cant == 0);//-1
}

//  compruebo si la lista esta llena
int FullListaEpaciente(ListaEpaciente e){
    return (e.cant >= MAX_PACIENTES);
}

// reinicio el cursor hasta el inicio de la lista
void resetListaEpaciente(ListaEpaciente *e){
    if (e != NULL) {
        if (e->cant > 0){
            e->cur = 0;
        } else {
            e->cur = -1;
        }
    }
}

// compruebo si el cursos esta dentro del rango
int isoosListaEpaciente(ListaEpaciente e){
    return (e.cur < 0 || e.cur > e.cant);
}

// muevo el cursor al siguiente paciente
void forwardListaEpaciente(ListaEpaciente *e){
    if (e != NULL && !isoosListaEpaciente(*e)){
        e->cur++;
      //  if (e->cur >= e->cant){
        //    e->cur = -1;
        //}
    }
}

// Copiar el paciente actual posicionado en el cursor
Paciente copyListaEpaciente(ListaEpaciente e){
    Paciente p;
    init_paciente(&p);
    if (!isoosListaEpaciente(e)){
        p = e.v[e.cur];
    }
    return p;
}

// Insertar un nuevo paciente al final de la lista estática (Retorna 1 si OK, 0 si llena)
int insertListaEpaciente(ListaEpaciente *e, Paciente nuevoPaciente){
    if (e == NULL || FullListaEpaciente(*e)){
        return 0; // Lista llena o puntero nulo
    }
    e->v[e->cant] = nuevoPaciente;
    e->cant++;
    if (e->cant == 1){
        e->cur = 0;
    }
    return 1;
}

// elimino el paciente actual del cursor
int suppressListaEpaciente(ListaEpaciente *e){
    int i;
    if (e == NULL || EmptylistaE_paciente(*e) || isoosListaEpaciente(*e)){
        return 0; // si retorna es porque esta vacio o fuera de rango
    }
    for (i = e->cur; i < e->cant - 1; i++){
        e->v[i] = e->v[i + 1];
    }
    e->cant--;
    if (e->cant == 0){
        e->cur = -1;
    } else if (e->cur >= e->cant){
        e->cur = e->cant - 1;
    }
    return 1;
}

// busco el paciente por dni usando la funcion get
int buscarPacientePorDni(ListaEpaciente e, long dniBuscado){
    int i = 0;
    while (i < e.cant){
        if(get_dni_paciente(e.v[i]) == dniBuscado){
            return 1;
        }
        i++;
    }
    return 0;
}

#endif // LISTAE_PACIENTE_H_INCLUDED
