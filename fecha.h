#ifndef FECHA_H_INCLUDED
#define FECHA_H_INCLUDED
#define MAX_AGENDA 390
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


void inicializarAgenda(Fecha agenda[MAX_AGENDA]) {
    int dia = 1;
    int mes = 11;
    int anio = 2026;

    // ya que el codigo nos dice que empiza el 1 de noviembre y cae domingo nos quedaria asi:
    // 0=Dom, 1=Lun, 2=Mar, 3=Mie, 4=Jue, 5=Vie, 6=Sab
    int diaSemana = 0;

    int index = 0; // creamos un indice para agende de 0 al 389

    while (index < MAX_AGENDA) {
        // como trabaja de lunes a viernes nuestro diaSemana tiene que estar entre esos rangos
        if (diaSemana >= 1 && diaSemana <= 5) {
            int hora = 8;
            int minuto = 0;
            int t;

            // aca generamos los 10 turnos que va a contener ese dia. desde las 8:00am hasta las 12:30pm
            for (t = 0; t < 10 && index < MAX_AGENDA; t++) {
                agenda[index].dia = dia;
                agenda[index].mes = mes;
                agenda[index].anio = anio;
                agenda[index].hora = hora;
                agenda[index].minuto = minuto;
                agenda[index].disponible = 1; // anteriormente tenemos que 1 significaba libre

                index++; // aca paso al siguiente casillero de la agenda

                // como los turnos son cada 30 minutos, le incremento 30 para el siguiente turno
                minuto += 30;
                if (minuto >= 60) { // comparo los minutos y en caso de ser mayor o igual a60
                    minuto = 0;     //restaurmanos minutos a 0 y le sumamos +1 a horas
                    hora++;
                }
            }
        }

        // pasamos al siguiente dia del calendiario
        dia++;
        diaSemana = (diaSemana + 1) % 7; // aca avanza el dia de la semana de una forma circular de 0 a 6

        // controlo fin de mes ya que noviembre tiene 30 dias y diciembre tiene 31
        int diasMaximos = (mes == 11) ? 30 : 31;
        if (dia > diasMaximos) {
            dia = 1;
            mes++;
            if (mes > 12) {
                mes = 1;
                anio++;
            }
        }
    }
}

int fecha_disponible(Fecha f, int tamanio_agenda, Fecha agenda[]) {
    int i;
    for(i = 0; i < tamanio_agenda; i++) {
            if(agenda[i].dia == f.dia &&
                agenda[i].mes == f.mes &&
                agenda[i].hora == f.hora &&
                agenda[i].minuto == f.minuto) {
                if(agenda[i].disponible == 1) {
                    return 1; // Disponible
                } else {
                    return 0; // No disponible
                }
            }
        }
    return 0; // La fecha no existe en la agenda
}

int indice_fecha(Fecha f, int tamanio_agenda, Fecha agenda[]) {
    int i;
    for(i = 0; i < tamanio_agenda; i++) {
        if(agenda[i].dia == f.dia &&
            agenda[i].mes == f.mes &&
            agenda[i].hora == f.hora &&
            agenda[i].minuto == f.minuto) {
            return i; // Retorna el índice de la fecha en la agenda
        }
    }
    return -1; // La fecha no existe en la agenda
}


int fecha_valida(Fecha f) {
    int dia = get_dia(f);
    int mes = get_mes(f);
    int anio = get_anio(f);
    int hora = get_hora(f);
    int minuto = get_minuto(f);

    // Validar año
    if(anio != 2026) return 0;

    // Validar mes
    if(mes < 11 || mes > 12) return 0;

    // Validar día según el mes
    if(mes == 11 && (dia < 1 || dia > 30)) return 0; // Noviembre tiene 30 días
    if(mes == 12 && (dia < 1 || dia > 31)) return 0; // Diciembre tiene 31 días

    // Validar hora y minuto
    if(hora < 8 || hora > 12) return 0; // Horario permitido: 8 a 12 hrs (a consultar si se dan turnos a las 13)
    if(minuto != 0 && minuto != 30) return 0; // Minutos permitidos: 0 o 30

    return 1; // Fecha válida
}

#endif // FECHA_H_INCLUDED
