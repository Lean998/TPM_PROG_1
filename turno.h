#ifndef TURNO_H_INCLUDED
#define TURNO_H_INCLUDED
#include "fecha.h"
typedef struct{
    int id_turno;
    long int dni_paciente;
    float monto; //Solo en caso de no tener mutual
    int mutual; //[0] Ninguna [1] Mutual 1 [2] Mutual 2 [3]Mutual 3 [4] Mutual 4
    Fecha fecha; // Solo valido Noviembre a Diciembre Lun a Vier. 8 a 13hrs
    int estado; // [0] Solicitado [1] Confirmado [2] Atendido [3] Cancelado
}Turno;

void init_turno(Turno* t){
    t->id_turno = -1;
    t->dni_paciente = -1;
    t->monto = -1;
    t->mutual = -1;
    init_fecha(&t->fecha);
    t->estado = -1;
}

void set_turno_id(Turno *t, int idTurno){
    t->id_turno = idTurno;
}
void set_turno_dni_paciente(Turno *t, long int dniPaciente){
    t->dni_paciente = dniPaciente;
}
void set_turno_monto(Turno *t, float monto){
    t->monto = monto;
}
void set_turno_mutual(Turno *t, int mutual){
    t->mutual = mutual;
}
void set_turno_fecha(Turno *t, Fecha fecha){
    t->fecha = fecha;
}
void set_turno_estado(Turno *t, int estado){
    t->estado = estado;
}

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



#endif // TURNO_H_INCLUDED
