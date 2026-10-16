#ifndef FECHA_H_INCLUDED
#define FECHA_H_INCLUDED
typedef struct{
    int dia;    // cant segun el mes
    int mes;    // 1 a 12
    int anio; // 2026
    int hora;   // 0 a 23
    int minuto; // 0 o 30
    int disponible; // [1]Libre [0]Ocupado. Por defecto Libre
}Fecha;


void init_fecha(Fecha *f) {
    f->dia = 1;
    f->mes = 1;
    f->anio = 2026; //Siempre 2026
    f->hora = 0;
    f->minuto = 0;
    f->disponible = 1; // Libre por defecto
}

void set_dia(Fecha *f, int dia){
    f->dia = dia;
}

void set_mes(Fecha *f, int mes) {
    f->mes = mes;
}

void set_hora(Fecha *f, int hora){
    // Solo se permite horario de atención de 8 a 13 horas (Se puede sacar turno para las 13?)
    f->hora = hora;
}

void set_minutos(Fecha *f, int minutos){
    f->minuto = minutos;
}

void set_disponible(Fecha *f, int disponible){
    f->disponible = disponible;
}

int get_dia(Fecha f){
    return f.dia;
}

int get_mes(Fecha f){
    return f.mes;
}

int get_anio(Fecha f){
    return f.anio;
}

int get_hora(Fecha f){
    return f.hora;
}

int get_minuto(Fecha f){
    return f.minuto;
}

int get_disponible(Fecha f){
    return f.disponible;
}

Fecha get_fecha(Fecha f){
    return f;
}

//Cargar fecha completa
void set_fecha(Fecha *f, int dia, int mes, int hora, int minuto, int disponible){
    f->anio = 2026;
    set_mes(f, mes);
    set_dia(f, dia);
    set_hora(f, hora);
    set_minutos(f, minuto);
    set_disponible(f, disponible);
}

int es_fin_de_semana(int dia) {
    // 1/11/2026 es domingo
    int dia_semana = (dia + 0) % 7; // 0=Domingo, 1=Lunes, ..., 6=Sábado
    return (dia_semana == 0 || dia_semana == 6);
}

// Feriados
int es_feriado(int dia, int mes) {
    if (mes == 11 && dia == 9) return 1;
    if (mes == 11 && dia == 23) return 1;
    if (mes == 12 && dia == 8) return 1;
    if (mes == 12 && dia == 25) return 1;
    if (mes == 12 && dia == 7) return 1;
    return 0;
}

int comparar_fechas(Fecha f1, Fecha f2) {
    if (f1.anio != f2.anio) {
        return f1.anio - f2.anio;
    }
    if (f1.mes != f2.mes) {
        return f1.mes - f2.mes;
    }
    if (f1.dia != f2.dia) {
        return f1.dia - f2.dia;
    }
    if (f1.hora != f2.hora) {
        return f1.hora - f2.hora;
    }
    return f1.minuto - f2.minuto;
}

#endif // FECHA_H_INCLUDED
