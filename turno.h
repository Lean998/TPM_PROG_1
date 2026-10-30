#ifndef TURNO_H_INCLUDED
#define TURNO_H_INCLUDED

#include <string.h>
#include "fecha.h"

typedef struct{
    int id_turno;
    long int dni_paciente;
    float monto;        // Solo en caso de no tener obra social
    int mutual;         // [0] Ninguna [1] Mutual 1 [2] Mutual 2 [3] Mutual 3 [4] Mutual 4
    Fecha fecha;        // Fecha y hora del turno (TDA Fecha)
    int estado;         // [0] Solicitado, [1] Confirmado, [2] Atendido, [3] Cancelado
} Turno;

// Inicializa el turno con valores por defecto
void init_turno(Turno *t) {
    t->id_turno = -1;
    t->dni_paciente = 0;
    t->monto = 0.0f;
    t->mutual = 0;
    init_fecha(&(t->fecha));
    t->estado = 0; // Por defecto: Solicitado / Pendiente
}

// Sets
void set_turno_id(Turno *t, int idTurno){
    t->id_turno = idTurno;
}

void set_turno_dni_paciente(Turno *t, long int dniPaciente){
    t->dni_paciente = dniPaciente;
}

void set_turno_monto(Turno *t, float monto){
    if (monto >= 0) {
        t->monto = monto;
    }
}

void set_turno_mutual(Turno *t, int mutual){
    if (mutual >= 0 && mutual <= 4) {
        t->mutual = mutual;
    }
}

void set_turno_fecha(Turno *t, Fecha fecha){
    t->fecha = fecha;
}

void set_turno_estado(Turno *t, int estado){
    // por restriccion del pmi si ya esta atendido, no se debería modificar arbitrariamente
    t->estado = estado;
}

// Gets
int get_turno_id(Turno t){
    return t.id_turno;
}

long int get_turno_dni_paciente(Turno t){
    return t.dni_paciente;
}

float get_turno_monto(Turno t){
    return t.monto;
}

int get_turno_mutual(Turno t){
    return t.mutual;
}

Fecha get_turno_fecha(Turno t){
    return t.fecha;
}

int get_turno_estado(Turno t){
    return t.estado;
}

// funcion de validar por la restriccion de ´pmi
int es_turno_modificable(Turno t) {
    // Si el turno está en atendido, no puede carbiarse segun el enunciado
    if (t.estado == 2) {
        return 0;
    }
    return 1;
}

#endif // TURNO_H_INCLUDED
