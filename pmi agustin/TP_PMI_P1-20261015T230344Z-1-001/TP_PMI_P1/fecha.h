#ifndef FECHA_H_INCLUDED
#define FECHA_H_INCLUDED

typedef struct{
    int dia;            // Día según el mes
    int mes;            // 1 a 12 (Noviembre/Diciembre para el PMI)
    int anio;           // Fijo en 2026
    int hora;           // 0 a 23 (Atención de 8 a 13hs)
    int minuto;         // 0 o 30 (Fraccionado cada 30 minutos)
    int disponible;     // [1] Libre [0] Ocupado. Por defecto Libre
} Fecha;

// Inicializa una fecha con valores por defecto (1/1/2026, libre)
void init_fecha(Fecha *f) {
    f->dia = 1;
    f->mes = 1;
    f->anio = 2026; // Siempre 2026
    f->hora = 0;
    f->minuto = 0;
    f->disponible = 1; // Libre por defecto
}

// Setters individuales
void set_dia(Fecha *f, int dia){
    f->dia = dia;
}

void set_mes(Fecha *f, int mes) {
    f->mes = mes;
}

void set_hora(Fecha *f, int hora){
    f->hora = hora;
}

void set_minutos(Fecha *f, int minutos){
    f->minuto = minutos;
}

void set_disponible(Fecha *f, int disponible){
    f->disponible = disponible;
}

// Getters individuales
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

// Cargar fecha completa asegurando el año 2026
void set_fecha(Fecha *f, int dia, int mes, int hora, int minuto, int disponible){
    f->anio = 2026;
    set_mes(f, mes);
    set_dia(f, dia);
    set_hora(f, hora);
    set_minutos(f, minuto);
    set_disponible(f, disponible);
}

// Función robusta para calcular si es fin de semana (Sábado o Domingo) en Nov/Dic 2026
int es_fin_de_semana(int dia, int mes) {
    long diasTotales = 0;

    int m = 11;
    int d = 1;

    while (m < mes || (m == mes && d < dia)) {
        diasTotales++;
        d++;
        int diasMes = (m == 11) ? 30 : 31;
        if (d > diasMes) {
            d = 1;
            m++;
        }
    }

    int dia_semana = diasTotales % 7; // 0 = Domingo, 1 = Lunes, ..., 6 = Sábado
    return (dia_semana == 0 || dia_semana == 6);
}

// Feriados nacionales de Noviembre y Diciembre 2026
int es_feriado(int dia, int mes) {
    if (mes == 11 && dia == 9) return 1;
    if (mes == 11 && dia == 23) return 1;
    if (mes == 12 && dia == 7) return 1;
    if (mes == 12 && dia == 8) return 1;
    if (mes == 12 && dia == 25) return 1;
    return 0;
}

// Función para la validación interna del tda Fecha según el PMI
int es_fecha_valida(Fecha f) {
    if (f.anio != 2026) return 0;
    if (f.mes < 11 || f.mes > 12) return 0;

    int diasMax = (f.mes == 11) ? 30 : 31;
    if (f.dia < 1 || f.dia > diasMax) return 0;

    // Horario de atención: 8 a 13hs
    if (f.hora < 8 || f.hora > 13) return 0;
    if (f.hora == 13 && f.minuto > 0) return 0;

    // Minutos fraccionados cada 30 minutos
    if (f.minuto != 0 && f.minuto != 30) return 0;

    // Validar que no caiga en fin de semana ni feriado
    if (es_fin_de_semana(f.dia, f.mes) || es_feriado(f.dia, f.mes)) return 0;

    return 1;
}

// Comparación cronológica para la lista ordenada
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
