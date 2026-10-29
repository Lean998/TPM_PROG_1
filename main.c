#include <stdio.h>
#include <stdlib.h>
#include "paciente.h"
#include "listaE_paciente.h"
#include "listaVO.h"
#include "turno.h"
#define MAX_PACIENTES 100
#define MAX_TURNOS 390
//===================== PROTOTIPOS ======================
void inicializarAgenda(Fecha agenda[MAX_AGENDA]);
void leer_texto(char *mensaje, char *destino, int tam);
void nuevo_paciente(ListaEpaciente *lista);
void modificar_nombre_paciente(Paciente *p, char *nuevo_nombre);
void modificar_apellido_paciente(Paciente *p, char *nuevo_apellido);
void modificar_dni_paciente(Paciente *p, long int nuevo_dni);
void modificar_o_social_paciente(Paciente *p, int nuevo_oSocial);
void ver_o_social_paciente(Paciente p);
void mostrar_datos_paciente(Paciente p);
void mostrar_pacientes(ListaEpaciente *lista);
Turno nuevo_turno(ListaVo *listaT, Fecha agenda[], ListaEpaciente *listaP);
void carga_n_turnos(ListaVo *listaT, Fecha agenda[], ListaEpaciente *listaP, int n);
void modificar_id_turno(ListaVo listaT, Turno *t, int nuevo_id);
void modificar_dni_turno(Turno *t, ListaEpaciente listaP, long int nuevo_dni);
void modificar_monto_turno(Turno *t, float nuevo_monto);
void modificar_mutual_turno(Turno *t, int nuevo_mutual);
void modificar_fecha_turno(Turno *t, Fecha nueva_fecha, Fecha agenda[], int tamanio_agenda);
void modificar_estado_turno(Turno *t, int nuevo_estado);
void ver_o_social_turno(Turno t);
void mostrar_turnos_reservados(ListaVo lista, int* pagina);
void ver_estado_turno(Turno t);
void ver_datos_turno(Turno t);
void ver_fecha(Fecha f);
int fecha_valida(Fecha f);
int fecha_disponible(Fecha f, int tamanio_agenda, Fecha agenda[]);
int indice_fecha(Fecha f, int tamanio_agenda, Fecha agenda[]);
int main(void);
void mostrar_menu(void);
void pausa(void);

//====================== FUNCIONES AUXILIARES ======================
//Funcion para manejar la entrada de texto.
void leer_texto(char *mensaje, char *destino, int tam){
    do {
        printf("%s", mensaje);
        fgets(destino, tam, stdin); // Leer la entrada del usuario
        destino[strcspn(destino, "\n")] = '\0'; // sacar el \n
        if(destino[0] == '\0'){ // Verificar si la cadena está vacía
            printf("No puede estar vacio.\n");
        }
    } while(destino[0] == '\0');
}

// ====================== FUNCIONES PACIENTE ======================
//Datos de la estructura paciente: dni, nombre, apellido, oSocial
void nuevo_paciente(ListaEpaciente *lista){
    int dni, oSocial;
    char nombre[100], apellido[100];
    Paciente p;
    printf("Ingrese el nombre del paciente: ");
    leer_texto("Ingrese el nombre del paciente: ", nombre, 100);

    printf("Ingrese el apellido del paciente: ");
    leer_texto("Ingrese el apellido del paciente: ", apellido, 100);

    printf("Ingrese el DNI del paciente: ");
    do {
        scanf("%d", &dni);
        getchar();
        if(dni == 0) {
            return; // Cancelar la carga del paciente
        }
        if(dni < 10000000 || dni > 99999999) {
            printf("El DNI debe tener 8 digitos. Ingrese nuevamente: ");
        }else if(buscarPacientePorDni(*lista, dni) == 1) { // Si el paciente ya existe
            printf("El paciente con DNI %d ya existe. Ingrese otro DNI o salga (0) ", dni);
            dni = -1; // Forzar la repetición
        }
    } while(dni < 10000000 || dni > 99999999);

    printf("Tiene obra social? [1]Si [2]No: ");
    do {
        scanf("%d", &oSocial);
        getchar();
        if(oSocial < 0 || oSocial > 1) {
            printf("Opcion de obra social invalida. Ingrese nuevamente (0 o 1): ");
        }
    } while(oSocial < 0 || oSocial > 1);
    set_nombre_paciente(&p, nombre);
    set_apellido_paciente(&p, apellido);
    set_dni_paciente(&p, dni);
    set_o_social_paciente(&p, oSocial);
    insertListaEpaciente(lista, p);
}

void modificar_nombre_paciente(Paciente *p, char *nuevo_nombre) {
    if(strlen(nuevo_nombre) < 100) {
        set_nombre_paciente(p, nuevo_nombre);
    } else {
        printf("El nombre es demasiado largo.\n");
    }
}

void modificar_apellido_paciente(Paciente *p, char *nuevo_apellido) {
    if(strlen(nuevo_apellido) < 100) {
        set_apellido_paciente(p, nuevo_apellido);
    } else {
        printf("El apellido es demasiado largo.\n");
    }
}

void modificar_dni_paciente(Paciente *p, long int nuevo_dni) {
    if(nuevo_dni > 99999999 || nuevo_dni < 10000000) {
        printf("El DNI debe tener 8 digitos.\n");
    } else {
        set_dni_paciente(p, nuevo_dni);
    }
}

void modificar_o_social_paciente(Paciente *p, int nuevo_oSocial) {
    if(nuevo_oSocial >= 0 && nuevo_oSocial <= 1) {
        set_o_social_paciente(p, nuevo_oSocial);
    } else {
        printf("Opcion de obra social invalida.\n");
    }
}

void ver_o_social_paciente(Paciente p) {
    int oSocial = get_o_social_paciente(p);
    switch(oSocial) {
        case 0:{
            printf("No tiene obra social.\n"); break;
        }
        case 1:{
            printf("Si tiene obra social.\n"); break;
        }
    }
}

void mostrar_datos_paciente(Paciente p) {
    printf("Nombre: %s\n", get_nombre_paciente(p));
    printf("Apellido: %s\n", get_apellido_paciente(p));
    printf("DNI: %ld\n", get_dni_paciente(p));
    ver_o_social_paciente(p);
    printf("\n");
}

void mostrar_pacientes(ListaEpaciente *lista) {
    if(EmptylistaE_paciente(*lista)) {
        printf("No hay pacientes registrados.\n");
        return;
    }
    resetListaEpaciente(lista);
    Paciente p;
    while(!isoosListaEpaciente(*lista)) {
        p = copyListaEpaciente(*lista);
        mostrar_datos_paciente(p);
        forwardListaEpaciente(lista);
    }
}

// ====================== FUNCIONES TURNOS ======================
Turno nuevo_turno(ListaVo *listaT, Fecha agenda[], ListaEpaciente *listaP) {
    Turno t = {0};
    int dia, mes, hora, minuto, estado;
    long int dni;
    Fecha fecha;
    int mutual;
    float monto;
    int id;
    int indice_fecha_nueva = -1;
    printf("Ingrese la fecha del turno (dia mes hora minuto): ");
    int fecha_correcta;
    do {
        scanf("%d %d %d %d", &dia, &mes, &hora, &minuto);
        getchar();
        init_fecha(&fecha);
        set_fecha(&fecha, dia, mes, hora, minuto, 1);

        fecha_correcta = fecha_valida(fecha);
        if(!fecha_correcta) {
            if(get_anio(fecha) != 2026) {
                printf("El anio debe ser 2026.\n");
            }
            if(get_mes(fecha) < 11 || get_mes(fecha) > 12) {
                printf("El mes debe ser noviembre (11) o diciembre (12).\n");
            } else if((get_mes(fecha) == 11 && (get_dia(fecha) < 1 || get_dia(fecha) > 30)) ||
                    (get_mes(fecha) == 12 && (get_dia(fecha) < 1 || get_dia(fecha) > 31))) {
                printf("El dia no es valido para el mes indicado.\n");
            }
            if(get_hora(fecha) < 8 || get_hora(fecha) > 12) {
                printf("La hora debe estar entre las 8 y las 12.\n");
            }
            if(get_minuto(fecha) != 0 && get_minuto(fecha) != 30) {
                printf("Los minutos permitidos son 0 o 30.\n");
            }
        } else if(fecha_disponible(fecha, MAX_AGENDA, agenda) == 0) {
            printf("La fecha no esta disponible o no corresponde a un dia habil.\n");
            fecha_correcta = 0;
        }
        if(!fecha_correcta) {
            printf("Ingrese nuevamente la fecha (dia mes hora minuto): \n");
        }
    } while(!fecha_correcta);


    printf("Ingrese el DNI del paciente: (0 para salir y registrar al paciente)");
    do
    {
        scanf("%ld", &dni);
        getchar();
        if(dni == 0) {
            return t; // ID 0 indica que se canceló la carga.
        }
        if(dni > 99999999 || dni < 10000000) {
            printf("El DNI debe tener 8 digitos. Ingrese nuevamente: \n");
        }
        if(buscarPacientePorDni(*listaP, dni) == 0) {
            printf("El paciente ingresado no existe. Registrelo en el sistema o verifique el DNI (0 para salir): \n");
        }
    } while (dni > 99999999 || dni < 10000000 || buscarPacientePorDni(*listaP, dni) == 0);


    printf("Ingrese la mutual del paciente ([0]: Ninguna, [1]: Mutual 1, [2]: Mutual 2, [3]: Mutual 3, [4]: Mutual 4): ");
    do
    {
        scanf("%d", &mutual);
        getchar();
        if(mutual < 0 || mutual > 4) {
            printf("Opcion de mutual invalida. Ingrese nuevamente: \n");
        }
    } while (mutual < 0 || mutual > 4);
    if(mutual == 0) {
        printf("Ingrese el monto a pagar: ");
        do
        {
            scanf("%f", &monto);
            getchar();
            if(monto < 0) {
                printf("El monto debe ser positivo. Ingrese nuevamente: \n");
            }
        } while (monto < 0);
    } else {
        monto = 0; // No se paga si tiene mutual
    }
    indice_fecha_nueva = indice_fecha(fecha, MAX_AGENDA, agenda);

    set_turno_fecha(&t, fecha);
    set_turno_dni_paciente(&t, dni);
    set_turno_mutual(&t, mutual);
    set_turno_monto(&t, monto);
    set_turno_estado(&t, 0); // Pendiente
    set_turno_id(&t, indice_fecha_nueva + 1); // ID del turno segun el índice de la fecha en la agenda
    agenda[indice_fecha_nueva].disponible = 0; // Marcar la fecha como ocupada
    return t;
}

void carga_n_turnos(ListaVo *listaT, Fecha agenda[], ListaEpaciente *listaP, int n) {
    int i;
    for(i = 0; i < n; i++) {
        if(isFull(*listaT)) {
            printf("No se pudo cargar el turno. La lista de turnos esta llena.\n");
            break;
        }
        Turno t = nuevo_turno(listaT, agenda, listaP);
        if(t.id_turno == 0) { // ID 0 indica que se canceló la carga.
            printf("Carga de turno cancelada por el usuario.\n");
            break;
        }
        agregarTurno(listaT, t);
        printf("Turno cargado exitosamente.\n");

        //Después de cargar un turno, preguntar si desea cargar otro
        if(i < n - 1) {
            char continuar;
            do {
                printf("Desea cargar otro turno? (s/n): ");
                scanf(" %c", &continuar);
                getchar();
                if(continuar != 's' && continuar != 'S' && continuar != 'n' && continuar != 'N') {
                    printf("Opcion invalida. Ingrese s o n.\n");
                }
            } while(continuar != 's' && continuar != 'S' && continuar != 'n' && continuar != 'N');

            if(continuar == 'n' || continuar == 'N') {
                break;
            }
        }
    }
}

void modificar_id_turno(ListaVo listaT, Turno *t, int nuevo_id) {
    if(t->estado != 3) { // Si el turno no está cancelado
        if(buscarIdTurno(listaT, nuevo_id) == 0) { // Si el ID no existe
            set_turno_id(t, nuevo_id);
        } else {
            printf("El ID de turno ya existe.\n");
        }
    } else {
        printf("No se puede modificar el ID de un turno cancelado.\n");
    }
}

void modificar_dni_turno(Turno *t, ListaEpaciente listaP, long int nuevo_dni) {
    Paciente p;
    if(t->estado != 3) { // Si el turno no está cancelado
        if(nuevo_dni > 99999999 || nuevo_dni < 10000000) {
            printf("El DNI debe tener 8 digitos.\n");
        } else if(buscarPacientePorDni(listaP, nuevo_dni) == 1) {
            set_turno_dni_paciente(t, nuevo_dni);
        } else {
            printf("El DNI del paciente no existe.\n");
        }
    } else {
        printf("No se puede modificar el DNI de un turno cancelado.\n");
    }
}

void modificar_monto_turno(Turno *t, float nuevo_monto) {
    if(t->estado != 3) { // Si el turno no está cancelado
        if(nuevo_monto >= 0 && t->mutual == 0) { // Solo se puede modificar el monto si no tiene mutual
            set_turno_monto(t, nuevo_monto);
        } else {
            printf("El monto debe ser positivo y el turno no debe tener mutual.\n");
        }
    } else {
        printf("No se puede modificar el monto de un turno cancelado.\n");
    }
}

void modificar_mutual_turno(Turno *t, int nuevo_mutual) {
    if(t->estado != 3) { // Si el turno no está cancelado
        if(nuevo_mutual >= 0 && nuevo_mutual <= 4) {
            set_turno_mutual(t, nuevo_mutual);
        } else {
            printf("Opcion de mutual invalida.\n");
        }
    } else {
        printf("No se puede modificar la mutual de un turno cancelado.\n");
    }
}

void modificar_fecha_turno(Turno *t, Fecha nueva_fecha, Fecha agenda[], int tamanio_agenda) {
    int i;
    if(t->estado != 3) { // Si el turno no está cancelado
        int indice_fecha_nueva = -1;
        int indice_fecha_actual = -1;
        if(comparar_fechas(nueva_fecha, t->fecha) == 0) {
            printf("La fecha seleccionada es la misma que la actual.\n");
            return;
        }
        for(i = 0; i < tamanio_agenda; i++) {
            if(agenda[i].dia == nueva_fecha.dia &&
                agenda[i].mes == nueva_fecha.mes &&
                agenda[i].hora == nueva_fecha.hora &&
                agenda[i].minuto == nueva_fecha.minuto) {
                indice_fecha_nueva = i;
            }
            if(agenda[i].dia == t->fecha.dia &&
                agenda[i].mes == t->fecha.mes &&
                agenda[i].hora == t->fecha.hora &&
                agenda[i].minuto == t->fecha.minuto) {
                indice_fecha_actual = i;
            }
        }

        if(indice_fecha_nueva < 0 || fecha_disponible(nueva_fecha, tamanio_agenda, agenda) == 0) {
            printf("La fecha seleccionada no esta disponible en la agenda.\n");
            return;
        }

        if(indice_fecha_actual >= 0) {
            // Marcar la fecha actual como disponible
            agenda[indice_fecha_actual].disponible = 1;
        }
        // Marcar la nueva fecha como ocupada
        agenda[indice_fecha_nueva].disponible = 0;
        set_turno_fecha(t, nueva_fecha);
    } else {
        printf("No se puede modificar la fecha de un turno cancelado.\n");
    }
}

void modificar_estado_turno(Turno *t, int nuevo_estado) {
    // Solo se puede modificar el estado si el turno no está cancelado
    if(t->estado != 3) {
        if(nuevo_estado >= 0 && nuevo_estado <= 3) {
            set_turno_estado(t, nuevo_estado);
        } else {
            printf("Estado de turno invalido.\n");
        }
    } else {
        printf("No se puede modificar el estado de un turno cancelado.\n");
    }
}

void ver_o_social_turno(Turno t) {
    int mutual = get_turno_mutual(t);
    switch(mutual) {
        case 0:{
            printf("Mutual: Ninguna\n"); break;
        }
        case 1:{
            printf("Mutual: Mutual 1\n"); break;
        }
        case 2:{
            printf("Mutual: Mutual 2\n"); break;
        }
        case 3:{
            printf("Mutual: Mutual 3\n"); break;
        }
        case 4:{
            printf("Mutual: Mutual 4\n"); break;
        }
        default:{
            printf("Mutual desconocida.\n");
        }
    }
}

void mostrar_turnos_reservados(ListaVo lista, int* pagina) {
    if(isEmpty(lista)) {
        printf("No hay turnos reservados.\n");
        return;
    }
    // Contar turnos para calcular la cantidad de paginas
    int total = 0;
    reset(&lista);
    while(!isOos(lista)) {
        total++;
        forward(&lista);
    }
    int total_paginas = (total + 9) / 10;

    // Por si se borraron turnos y la pagina guardada ya no existe
    if(*pagina > total_paginas) *pagina = total_paginas;
    if(*pagina < 1) *pagina = 1;

    int i;
    char opcion;
    do {
        // Posicionar el cursor al inicio de la pagina
        reset(&lista);
        for(i = 0; i < (*pagina - 1) * 10; i++) {
            forward(&lista);
        }

        // Mostrar hasta 10 turnos
        printf("=== Turnos Reservados ===\n");
        for(i = 0; i < 10 && !isOos(lista); i++) {
            ver_datos_turno(copyTurno(lista));
            forward(&lista);
        }
        printf("=== Pagina %d / %d ===\n", *pagina, total_paginas);
        int valida;
        do {
            printf("[S]iguiente  [A]nterior  [N] Salir: ");
            scanf(" %c", &opcion);
            getchar();
            if( valida = (opcion == 'n' || opcion == 'N') ||
                     ((opcion == 's' || opcion == 'S') && *pagina < total_paginas) ||
                     ((opcion == 'a' || opcion == 'A') && *pagina > 1)); {
            }
            if(!valida) {
                if((opcion == 'a' || opcion == 'A') && *pagina <= 1){
                    printf("No puede retroceder mas paginas.\n");
                }else if((opcion == 's' || opcion == 'S') &&  *pagina >= total_paginas){
                    printf("No puede avanzar mas paginas.\n");
                }else{
                    printf("Opcion invalida.\n");
                }
            }
        } while(!valida);

        if(opcion == 's' || opcion == 'S') (*pagina)++;
        else if(opcion == 'a' || opcion == 'A') (*pagina)--;
    } while(opcion != 'n' && opcion != 'N');
}

void ver_estado_turno(Turno t) {
    int estado = get_turno_estado(t);
    switch(estado) {
        case 0:{
            printf("Estado del turno: Solicitado\n"); break;
        }
        case 1:{
            printf("Estado del turno: Confirmado\n"); break;
        }
        case 2:{
            printf("Estado del turno: Atendido\n"); break;
        }
        case 3:{
            printf("Estado del turno: Cancelado\n"); break;
        }
        default:{
            printf("Estado de turno desconocido.\n");
        }
    }
}

void ver_datos_turno(Turno t) {
    printf("ID Turno: %d\n", get_turno_id(t));
    printf("DNI Paciente: %ld\n", get_turno_dni_paciente(t));
    printf("Monto: %.2f\n", get_turno_monto(t));
    ver_o_social_turno(t);
    ver_fecha(get_turno_fecha(t));
    ver_estado_turno(t);
    printf("\n");
}

void ver_turnos_asignados_mes(ListaVo listaTurnos, int mes){
    Turno t;
    int estado;
    if(!isOos(listaTurnos)){
        t = copyTurno(listaTurnos);
        estado = get_turno_estado(t);
        if(get_mes(get_turno_fecha(t))==mes && (estado == 0 || estado == 1)){
                ver_datos_turno(t);
        }
        forward(&listaTurnos);
        ver_turnos_asignados_mes(listaTurnos, mes);
    }else{
        return;
    }
}

int contar_turnos_atendidos_mes_dia(ListaVo listaTurnos, int dia, int mes){
    Turno t;
    if(!isOos(listaTurnos)){
        t = copyTurno(listaTurnos);
        forward(&listaTurnos);
        if(get_mes(get_turno_fecha(t))== mes && get_dia(get_turno_fecha(t)) == dia && get_turno_estado(t)==2){
            return 1 + contar_turnos_atendidos_mes_dia(listaTurnos, dia, mes);
        }
        return contar_turnos_atendidos_mes_dia(listaTurnos, dia, mes);
    }else{
        return 0;
    }
}

float ver_monto_ingresado_mes(ListaVo listaTurnos, int mes){
    float monto_ingresado = 0;
    Turno t;
    reset(&listaTurnos);
    while(!isOos(listaTurnos)){
        t = copyTurno(listaTurnos);
        int estado_turno = get_turno_estado(t); // Solo tendran un monto los turnos confirmados y atendidos (1 y 2).
        if(get_mes(get_turno_fecha(t))==mes && (estado_turno == 1 || estado_turno == 2)){
            monto_ingresado += get_turno_monto(t);
        }
        forward(&listaTurnos);
    }
    return monto_ingresado;
}

// ====================== FUNCIONES FECHA ======================
void ver_fecha(Fecha f) {
    printf("Fecha: %02d/%02d/%04d %02d:%02d\n", get_dia(f), get_mes(f), get_anio(f), get_hora(f), get_minuto(f));
}

// ====================== MENÚ ======================
void mostrar_menu() {
    printf("\n========== MENU ==========\n");
    printf("1. Pacientes\n");
    printf("2. Turnos\n");
    printf("3. Fechas\n");
    printf("4. Limpiar pantalla\n");
    printf("0. Salir\n");
    printf("==========================\n");
}

void mostrar_menu_pacientes() {
    printf("\n=== Menu Pacientes ===\n");
    printf("1. Nuevo paciente\n");
    printf("2. Modificar paciente\n");
    printf("3. Mostrar pacientes\n");
    printf("4. Limpiar pantalla\n");
    printf("0. Volver al menu principal\n");
    printf("======================\n");
}

void operaciones_menu_pacientes(int subopcion_pacientes, ListaEpaciente* listaPacientes){
    switch (subopcion_pacientes) {
                    case 1:{
                        nuevo_paciente(listaPacientes);
                        pausa();
                        break;
                    }
                    case 2:{
                        // Implementar modificar paciente
                        break;
                    }
                    case 3:{
                        mostrar_pacientes(listaPacientes);
                        break;
                    }
                    case 4:{
                        system("cls");
                        break;
                    }
                    case 0:{
                        printf("\nVolviendo...\n");
                        break;
                    }
                    default:{
                        printf("Opcion invalida.\n");
                        break;
                    }
                }
}

void mostrar_menu_turnos() {
    printf("\n=== Menu Turnos ===\n");
    printf("1. Nuevo turno\n");
    printf("2. Carga n turnos\n");
    printf("3. Mostrar turnos reservados\n");
    printf("4. Mostrar turnos reservados (en un mes)\n");
    printf("5. Mostrar turnos atendidos (en un dia/mes)\n");
    printf("6. Mostrar dinero ingresado (en un mes)\n");
    printf("7. Modificar turno\n");
    printf("8. Limpiar pantalla\n");
    printf("0. Volver al menu principal\n");
    printf("====================\n");
}

void operaciones_menu_turnos(int subopcion_turnos, ListaVo *listaTurnos, Fecha agenda[], ListaEpaciente *listaPacientes, int* pagina_turnos){
    switch (subopcion_turnos) {
                        case 1:{
                            if(isFull(*listaTurnos)) {
                                printf("\nLa lista de turnos esta llena.\n");
                            } else {
                                Turno t = nuevo_turno(listaTurnos, agenda, listaPacientes);
                                if(t.id_turno != 0) {
                                    agregarTurno(listaTurnos, t);
                                    printf("\nTurno cargado exitosamente.\n");
                                } else {
                                    printf("\nCarga de turno cancelada.\n");
                                }
                            }
                            break;
                        }
                        case 2:{
                            int n;
                            printf("Ingrese la cantidad de turnos a cargar: ");
                            //Controlar que se ingrese una cantidad valida.
                            do {
                                scanf("%d", &n);
                                getchar();
                                if(n <= 0) {
                                    printf("La cantidad debe ser mayor a 0. Ingrese nuevamente: ");
                                } else if(n > MAX_TURNOS) {
                                    printf("La cantidad no puede superar %d. Ingrese nuevamente: ", MAX_TURNOS);
                                }
                            } while(n <= 0 || n > MAX_TURNOS);
                            carga_n_turnos(listaTurnos, agenda, listaPacientes, n);
                            break;
                        }
                        case 3:{
                            reset(listaTurnos);
                            mostrar_turnos_reservados(*listaTurnos, pagina_turnos);
                            pausa();
                            break;
                        }
                        case 4:{
                            int mes;
                            printf("Ingrese el mes del cual quiere ver los turnos? [1]Noviembre [2]Diciembre\n");
                            do
                            {
                                scanf("%d", &mes);
                                getchar();
                                if(mes != 1 && mes != 2){
                                    printf("Ingrese una opcion valida.\n");
                                }
                            } while (mes != 1 && mes != 2);
                            reset(listaTurnos);
                            ver_turnos_asignados_mes(*listaTurnos, mes + 10);
                            break;
                        }
                        case 5:{
                            int dia, mes, atendidos;
                            int dia_valido = 1;
                            printf("Ingrese el mes del cual quiere ver los turnos atendidos [1]Noviembre [2]Diciembre\n");
                            do
                            {
                                scanf("%d", &mes);
                                getchar();
                                if(mes != 1 && mes != 2){
                                    printf("Ingrese una opcion valida.\n");
                                }
                            } while (mes != 1 && mes != 2);
                            printf("Ingrese el dia del cual quiere ver los turnos atendidos (1-31)\n");
                            do
                            {
                                scanf("%d", &dia);
                                getchar();
                                if(dia < 0 || dia > 31){
                                    printf("Ingrese un dia valido (1-31)\n");
                                    dia_valido = 0;
                                }
                                if(mes == 11 && dia > 30){
                                    printf("Noviembre solo tiene 30 dias\n");
                                    dia_valido = 0;
                                }
                            } while (!dia_valido);
                            reset(listaTurnos);
                            atendidos = contar_turnos_atendidos_mes_dia(*listaTurnos, dia, mes+10);
                            printf("\n=== Turnos atendidos el %02d/%02d/2026 ===\n", dia, mes+10);
                            printf("Se atendieron %d turnos\n", atendidos);
                            break;
                        }
                        case 6:{
                            int mes;
                            float monto_ingresado;
                            printf("Ingrese el mes del cual quiere ver las ganancias? [1]Noviembre [2]Diciembre\n");
                            do
                            {
                                scanf("%d", &mes);
                                getchar();
                                if(mes != 1 && mes != 2){
                                    printf("Ingrese una opcion valida.\n");
                                }
                            } while (mes != 1 && mes != 2);
                            reset(listaTurnos);
                            monto_ingresado = ver_monto_ingresado_mes(*listaTurnos, mes+10);
                            printf("En el mes %d se obtuvo una ganancia de %.2f", mes+10, monto_ingresado);
                            break;
                        }
                        case 8:{
                            system("cls");
                            break;
                        }
                        case 0:{
                            printf("\nVolviendo...\n");
                            break;
                        }
                        default:{
                            printf("Opcion invalida.\n");
                            break;
                        }
                        }
}

void mostrar_menu_fechas() {
    printf("\n=== Menu Fechas ===\n");
    printf("1. Buscar fechas libres\n");
    printf("2. Mostrar fechas disponibles\n");
    printf("3. Limpiar pantalla\n");
    printf("0. Volver al menu principal\n");
    printf("====================\n");
}
void operaciones_menu_fechas(int subopcion_fechas, Fecha agenda[]){
    switch(subopcion_fechas){
        case 1:{
            break;
        }
        case 2:{
            printf("\n--- PRIMERAS FECHAS LIBRES EN AGENDA ---\n");
                            int mostrados = 0;
                            int i;
                            for ( i = 0; i < MAX_AGENDA && mostrados < 15; i++) {
                                if (agenda[i].disponible == 1) {
                                    printf("ID Turno: %3d | Fecha: %02d/%02d/%d - %02d:%02d\n",
                                        i, agenda[i].dia, agenda[i].mes, agenda[i].anio, agenda[i].hora, agenda[i].minuto);
                                    mostrados++;
                                }
                            }
                            printf("(Mostrando las primeras 15 fechas disponibles)\n");
            break;
        }
        case 3:{
            system("cls");
            break;
        }
        case 0:{
                printf("\nVolviendo...\n");
                break;
                }
        default:{
            printf("Opcion invalida.\n");
        }
    }
}

void pausa() {
    printf("\nEnter para continuar...");
    getchar();
}

// ====================== PERSISTENCIA DE DATOS ======================
void guardar_pacientes(ListaEpaciente pacientes[]){
    int i;
    FILE* archivo;
    Paciente p;

    archivo = fopen("pacientes.txt", "w");
    if(archivo == NULL) return;

    resetListaEpaciente(pacientes);

    while(!isoosListaEpaciente(*pacientes)){
        p = copyListaEpaciente(*pacientes);
        fprintf(archivo, "%s\t%s\t%ld\t%d\n",
            get_nombre_paciente(p),
            get_apellido_paciente(p),
            get_dni_paciente(p),
            get_o_social_paciente(p)
        );
        forwardListaEpaciente(pacientes);
    }
    fclose(archivo);
}

int cargar_pacientes(ListaEpaciente pacientes[]) {
    FILE* archivo;
    int cargados = 0;
    char nombre[100];
    char apellido[100];
    long dni;
    int o_social;
    Paciente p;

    archivo = fopen("pacientes.txt", "r");
    if (archivo == NULL) return 0;

    while (fscanf(archivo, "%99[^\t]\t%99[^\t]\t%ld\t%d\n",
                nombre, apellido, &dni, &o_social) == 4) {

        // Construir el paciente con los datos del archivo
        modificar_nombre_paciente(&p, nombre);
        modificar_apellido_paciente(&p, apellido);
        modificar_dni_paciente(&p, dni);
        modificar_o_social_paciente(&p, o_social);

        // Insertar paciente
        insertListaEpaciente(pacientes, p);
        cargados++;
    }

    fclose(archivo);
    return cargados;
}


void guardar_turnos(ListaVo turnos[]){
    int i;
    Turno t;
    Fecha f;
    FILE* archivo;

    archivo = fopen("turno.txt", "w");
    if(archivo == NULL) return;

    reset(turnos);
    while(!isOos(*turnos)){
        t = copyTurno(*turnos);
        f = get_turno_fecha(t);
        fprintf(archivo, "%d\t%ld\t%f\t%d\t%d/%d/%d %d:%d\t%d\t%d\n",
            get_turno_id(t),
            get_turno_dni_paciente(t),
            get_turno_monto(t),
            get_turno_mutual(t),
            get_dia(f),
            get_mes(f),
            get_anio(f),
            get_hora(f),
            get_minuto(f),
            get_disponible(f),
            get_turno_estado(t)
        );
        forward(turnos);
    }
    fclose(archivo);
}

int cargar_turnos(ListaVo turnos[]) {
    FILE* archivo;
    int i=0;
    int id, mutual, dia, mes, anio;
    int hora, minuto, disponible, estado;
    long dni;
    float monto;
    Turno t;
    Fecha f;

    archivo = fopen("turno.txt", "r");
    if (archivo == NULL) return 0;
    init_fecha(&f);
    init_turno(&t);
    while (fscanf(archivo,
                "%d\t%ld\t%f\t%d\t%d/%d/%d %d:%d\t%d\t%d",
                &id, &dni, &monto, &mutual,
                &dia, &mes, &anio, &hora, &minuto,
                &disponible, &estado) == 11) {

        set_fecha(&f, dia, mes, hora, minuto, disponible);
        set_turno_id(&t, id);
        set_turno_dni_paciente(&t, dni);
        set_turno_monto(&t, monto);
        set_turno_mutual(&t, mutual);
        set_turno_fecha(&t, f);
        set_turno_estado(&t, estado);
        agregarTurno(turnos, t);
        i++;
    }

    fclose(archivo);
    return i;
}


void guardar_agenda(Fecha agenda[]){
    int i;
    FILE* archivo = fopen("agenda.txt", "w");
    if(archivo == NULL) return;

    for(i=0; i < MAX_AGENDA; i++){
        fprintf(archivo, "%d/%d/%d %d:%d\t%d\n",
            get_dia(agenda[i]),
            get_mes(agenda[i]),
            get_anio(agenda[i]),
            get_hora(agenda[i]),
            get_minuto(agenda[i]),
            get_disponible(agenda[i])
        );
    }
    fclose(archivo);
}

int cargar_agenda(Fecha agenda[]) {
    int i = 0;
    int dia, mes, anio, hora, minuto, disponible;

    FILE* archivo = fopen("agenda.txt", "r");
    if (archivo == NULL) return 0;

    while (i < MAX_AGENDA &&
        fscanf(archivo, "%d/%d/%d %d:%d\t%d",
                &dia, &mes, &anio,
                &hora, &minuto, &disponible) == 6) {

        set_dia(&agenda[i], dia);
        set_mes(&agenda[i], mes);
        agenda[i].anio = anio; //2026
        set_hora(&agenda[i], hora);
        set_minutos(&agenda[i], minuto);
        set_disponible(&agenda[i], disponible);
        i++;
    }

    fclose(archivo);
    return i;
}



int main()
{
    ListaEpaciente listaPacientes = {0};
    ListaVo listaTurnos = {0};
    int i;

    Fecha agenda[MAX_AGENDA];
    inicializarAgenda(agenda);
    cargar_pacientes(&listaPacientes);
    cargar_agenda(agenda);
    cargar_turnos(&listaTurnos);
    int opcion;
    int subopcion_pacientes;
    int subopcion_turnos;
    int subopcion_fechas;
    int pagina_turnos = 1;
    printf("=== Sistema de Turnos Medicos ===\n");

    do {
        mostrar_menu();
        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar();
        switch (opcion) {
            case 1:
            do
            {
                mostrar_menu_pacientes();
                printf("Opcion: ");
                scanf("%d", &subopcion_pacientes);
                getchar();
                operaciones_menu_pacientes(subopcion_pacientes, &listaPacientes);
                guardar_pacientes(&listaPacientes);
                } while (subopcion_pacientes != 0);
                break;
            case 2: {
                do
                {
                    mostrar_menu_turnos();
                    printf("Opcion: ");
                    scanf("%d", &subopcion_turnos);
                    getchar();
                    operaciones_menu_turnos(subopcion_turnos, &listaTurnos, agenda, &listaPacientes, &pagina_turnos);
                    guardar_turnos(&listaTurnos);
                    guardar_agenda(agenda);
                    } while (subopcion_turnos != 0);
                }
                break;
            case 3:{
                do
                {
                    mostrar_menu_fechas();
                    printf("Opcion: \n");
                    scanf("%d", &subopcion_fechas);
                    getchar();
                    operaciones_menu_fechas(subopcion_fechas, agenda);
                    guardar_agenda(agenda);
                    }while (subopcion_fechas != 0);
                }
                break;
            case 4:{
                system("cls");
                break;
            }
            case 0:{
                printf("\nSaliendo...\n");
                }break;
            default:{
                }printf("\nOpcion invalida.\n");
        }
    } while (opcion != 0);

    //Guardado de seguridad antes de salir.
    guardar_pacientes(&listaPacientes);
    guardar_turnos(&listaTurnos);
    guardar_agenda(agenda);
    return 0;
}





