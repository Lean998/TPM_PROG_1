#ifndef FECHA_H
#define FECHA_H
#include <stdio.h>
typedef struct{
    int dia;
    int mes;
    int anio;
    int hora; // rango hora (0 - 23)
    int minutos; // rango de minutos (0 o 30)
    int disponible; // 1 significa libre y 0 significa ocupado

}fecha;
void initFecha (fecha *f){
    f->dia = 1;
    f->mes = 11; // en noviembre inicia agenda
    f->anio = 2026; // requirimiento 2026 fijo
    f->hora = 8; // hora de apertura del consultorio
    f->minutos = 0;
    f->disponible = 1; // esta libre
}
void cargarFecha (fecha *f, int nuevoDia, int nuevoMes, int nuevaHora, int nuevominutos,int nuevoEstadoDisponible){
    f->dia = nuevoDia;
    if (nuevoMes >= 1 && nuevoMes <= 12){
        f->mes = nuevoMes;
    }
    else {
        printf("mes no valido. ingresar mes ( 1 - 12 ). ");
    }
    if (nuevaHora >=0 && nuevaHora <=23){
        f->hora = nuevaHora;
    }
    else {
        printf("hora no valida. ingresar hora ( 0 - 23 ). ");
    }
    if (nuevominutos == 0 || nuevominutos == 30){
        f->minutos = nuevominutos;
    }
    else {
        printf("minutos no validos. ingresar 0 o 30. ");
    }
    f->disponible = nuevoEstadoDisponible;
}
int retornarDia (fecha f){
    return f.dia;
}
int retornarMes (fecha f){
    return f.mes;
}
int retornarAnio (fecha f){
    return f.anio;
}
int retornarHora(fecha f){
    return f.hora;
}
int retornarMinutos(fecha f){
    return f.minutos;
}
int retornarDisponible (fecha f){
    return f.disponible;
}
void mostrarFechacompleta (fecha f){
    printf("FECHA: %02d || MES: %02d || ANIO: %d || HORA: %d || MINUTOS: %d || disponible: %s", f.dia,f.mes,f.anio, f.hora,f.minutos, f.disponible ? "libre" : "ocupado");
}
#endif // FECHA_H
