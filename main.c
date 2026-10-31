#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "paciente.h"
#include "listaE_paciente.h"
#include "listaVO.h"
#include "turno.h"
#define MAX_PACIENTES 100
#define MAX_TURNOS 390

//===================== PROTOTIPOS AGUSTIN ======================
void eliminar_paciente_sin_turnos(ListaEpaciente *listaP, ListaVo listaTurnos, long dniBuscado);
void mostrar_turnos_por_dni(ListaVo listaTurnos, long dniBuscado);
void inicializarAgenda(Fecha agenda[MAX_AGENDA]);
void leer_texto(char *mensaje, char *destino, int tam);
void cargar_paciente_teclado(ListaEpaciente *lista);
void modificar_nombre_paciente(Paciente *p, char *nuevo_nombre);
void modificar_apellido_paciente(Paciente *p, char *nuevo_apellido);
void modificar_dni_paciente(Paciente *p, long int nuevo_dni);
void modificar_o_social_paciente(Paciente *p, int nuevo_oSocial);
void ver_o_social_paciente(Paciente p);
void mostrar_datos_paciente(Paciente p);
void mostrar_todos_los_pacientes(ListaEpaciente *lista);
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
void ver_turnos_asignados_mes(ListaVo listaTurnos, int mes);
int contar_turnos_atendidos_mes_dia(ListaVo listaTurnos, int dia, int mes);
float ver_monto_ingresado_mes(ListaVo listaTurnos, int mes);
void guardar_pacientes(ListaEpaciente pacientes[]);
int cargar_pacientes(ListaEpaciente pacientes[]);
void guardar_turnos(ListaVo turnos[]);
int cargar_turnos(ListaVo turnos[]);
void guardar_agenda(Fecha agenda[]);
int cargar_agenda(Fecha agenda[]);
int buscarPacientePorDni(ListaEpaciente e, long dniBuscado);
int turno_existe(ListaVo turnos, int id_turno);

//===================== PROTOTIPOS EZE ======================
Turno nuevo_turno(ListaVo *listaT, Fecha agenda[], ListaEpaciente *listaP);
void mostrar_fechas_libres_dia_mes(Fecha agenda[], int dia, int mes);
void mostrar_fechas_ocupadas_mes(Fecha agenda[], int mes);
void cancelar_y_liberar(ListaVo *lista, Fecha agenda[], int id_turno);
void atender_proximo_paciente(ListaVo *lista, Fecha agenda[]);
void eliminar_turnos_cancelados(ListaVo *lista);
Turno obtenerTurnoPorId(ListaVo lista, int id_buscar);
void actualizarTurnoActual(ListaVo *lista, Turno t_modificado);

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

int buscarPacientePorDni(ListaEpaciente e, long dniBuscado){
    int i = 0;
    while (i < e.cant){
        if(e.v[i].dni == dniBuscado){
            return 1;
        }
        i++;
    }
    return 0;
}

int turno_existe(ListaVo turnos, int id_turno){
    int i = 0;
    reset(&turnos);
    Turno t;
    while(!isOos(turnos)){
        t = copyTurno(turnos);
        if(get_turno_id(t) == id_turno){
            return 1;
        }
        forward(&turnos);
    }
    return 0;
}

//Eze: "Devuelve una copia del turno buscado por su ID. Si no existe, devuelve un turno con ID 0."
Turno obtenerTurnoPorId(ListaVo lista, int id_buscar) {
    Turno tError;
    set_turno_id(&tError, 0); // Usamos ID 0 como bandera de que no se encontro
    reset(&lista);
    while (!isOos(lista)) {
        Turno turno = copyTurno(lista);
        if (get_turno_id(turno) == id_buscar) {
            return turno; // Encontrado
        }
        forward(&lista);
    }
    return tError; // No encontrado
}

//Eze: "Sobrescribe los datos del nodo al que apunta el cursor actual con un turno modificado."
void actualizarTurnoActual(ListaVo *lista, Turno t_modificado) {
    if(!isOos(*lista)){
        lista->cur->turno = t_modificado;
    }
}

// ====================== FUNCIONES PACIENTE ======================
void cargar_paciente_teclado(ListaEpaciente *listaP) {
    if (FullListaEpaciente(*listaP)) {
        printf("La lista de pacientes esta llena (Maximo 100).\n");
        return;
    }

    int dni, oSocial;
    char nombre[100], apellido[100];
    Paciente p;
    getchar();

    printf("Ingrese el nombre del paciente: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    printf("Ingrese el apellido del paciente: ");
    fgets(apellido, sizeof(apellido), stdin);
    apellido[strcspn(apellido, "\n")] = '\0';

    do {
        printf("Ingrese el DNI del paciente (8 digitos, 0 para cancelar): ");
        scanf("%d", &dni);
        getchar();

        if (dni == 0) {
            printf("Carga de paciente cancelada.\n");
            return;
        }
        if (dni < 10000000 || dni > 99999999) {
            printf("Error: El DNI debe tener exactamente 8 digitos.\n");
        } else if (buscarPacientePorDni(*listaP, dni) == 1) {
            printf("Error: Ya existe un paciente registrado con el DNI %d.\n", dni);
            dni = -1; // Forzar repetición
        }
    } while (dni < 10000000 || dni > 99999999);

    // Eze: "Preguntamos SOLAMENTE si tiene Obra Social (1 = Si, 0 = No)"
    do {
        printf("\n El paciente posee Obra Social? [1] Si  [0] No: ");
        scanf("%d", &oSocial);
        getchar();
        if (oSocial != 1 && oSocial != 0) printf("Error: Opcion invalida. Ingrese 1 (Si) o 0 (No).\n");
    } while (oSocial != 1 && oSocial != 0);

    set_nombre_paciente(&p, nombre);
    set_apellido_paciente(&p, apellido);
    set_dni_paciente(&p, dni);
    set_o_social_paciente(&p, oSocial);

    insertListaEpaciente(listaP, p);
    printf(" Paciente cargado exitosamente al sistema!\n");
}

void ver_o_social_paciente(Paciente p) {
    int oSocial = get_o_social_paciente(p);
    if(oSocial == 1) {
        printf("Obra social: Si\n");
    } else {
        printf("Obra social: No\n");
    }
}

void mostrar_paciente_por_dni(ListaEpaciente listaP, long dniBuscado) { //AGUSTIN
    if (EmptylistaE_paciente(listaP)) {
        printf("No hay pacientes registrados.\n");
        return;
    }
    resetListaEpaciente(&listaP);
    int encontrado = 0;
    while (!isoosListaEpaciente(listaP)) {
        Paciente p = copyListaEpaciente(listaP);
        if (get_dni_paciente(p) == dniBuscado) {
            printf("\n=== DATOS DEL PACIENTE ===\n");
            printf("Nombre: %s\n", get_nombre_paciente(p));
            printf("Apellido: %s\n", get_apellido_paciente(p));
            printf("DNI: %ld\n", get_dni_paciente(p));
            ver_o_social_paciente(p);
            printf("--------------------------\n");
            encontrado = 1;
            break;
        }
        forwardListaEpaciente(&listaP);
    }
    if (!encontrado) {
        printf("No se encontro ningun paciente registrado con el DNI %ld.\n", dniBuscado);
    }
}

void eliminar_paciente_sin_turnos(ListaEpaciente *listaP, ListaVo listaTurnos, long dniBuscado) { // AGUSTIN
    if (EmptylistaE_paciente(*listaP)) {
        printf("No hay pacientes registrados en el sistema.\n");
        return;
    }
    if (buscarPacientePorDni(*listaP, dniBuscado) == 0) {
        printf("El paciente con DNI %ld no se encuentra registrado.\n", dniBuscado);
        return;
    }
    int tieneTurnos = 0;
    reset(&listaTurnos);
    while (!isOos(listaTurnos)) {
        Turno t = copyTurno(listaTurnos);
        if (get_turno_dni_paciente(t) == dniBuscado) {
            tieneTurnos = 1;
            break;
        }
        forward(&listaTurnos);
    }
    if (tieneTurnos) {
        printf("No se puede eliminar al paciente con DNI %ld porque tiene turnos asignados.\n", dniBuscado);
        return;
    }
    resetListaEpaciente(listaP);
    int encontradoPos = 0;
    while (!isoosListaEpaciente(*listaP)) {
        Paciente p = copyListaEpaciente(*listaP);
        if (get_dni_paciente(p) == dniBuscado) {
            encontradoPos = 1;
            break;
        }
        forwardListaEpaciente(listaP);
    }
    if (encontradoPos) {
        if (suppressListaEpaciente(listaP)) {
            printf("¡Paciente con DNI %ld eliminado exitosamente (no poseia turnos)!\n", dniBuscado);
        } else {
            printf("Error al intentar eliminar al paciente.\n");
        }
    }
}

void mostrar_todos_los_pacientes(ListaEpaciente *listaP) { // AGUSTIN
    if (EmptylistaE_paciente(*listaP)) {
        printf("No hay pacientes registrados en el sistema.\n");
        return;
    }
    resetListaEpaciente(listaP);
    printf("\n=== LISTA COMPLETA DE PACIENTES ===\n");
    while (!isoosListaEpaciente(*listaP)) {
        Paciente p = copyListaEpaciente(*listaP);
        printf("Nombre: %s | Apellido: %s | DNI: %ld | ",
            get_nombre_paciente(p),
            get_apellido_paciente(p),
            get_dni_paciente(p));
            ver_o_social_paciente(p);
        forwardListaEpaciente(listaP);
    }
    printf("===================================\n");
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
    if(nuevo_oSocial >= 0 && nuevo_oSocial <= 4) { // Eze: Cambiado el limite a 4
        set_o_social_paciente(p, nuevo_oSocial);
    } else {
        printf("Opcion de obra social invalida.\n");
    }
}



// Eze: "Guardamos los punteros que devuelven los getters para poder hacerles free() despues"
void mostrar_datos_paciente(Paciente p) {
    char *n = get_nombre_paciente(p);
    char *a = get_apellido_paciente(p);

    printf("Nombre: %s\n", n);
    printf("Apellido: %s\n", a);
    printf("DNI: %ld\n", get_dni_paciente(p));
    ver_o_social_paciente(p);
    printf("\n");

    free(n);
    free(a);
}


// ====================== FUNCIONES TURNOS ======================

// Eze Nuevo Turno
Turno nuevo_turno(ListaVo *listaT, Fecha agenda[], ListaEpaciente *listaP) {
    Turno t = {0};
    int dia, mes_input, mes, hora, minuto;
    long int dni;
    int i;
    int indice_fecha_nueva = -1;

    //Eze: "1. Primero pedimos el DNI y si no existe, ofrecemos registrarlo."
    printf("Ingrese el DNI del paciente (0 para salir): ");
    do {
        scanf("%ld", &dni);
        getchar();
        if(dni == 0) return t;

        if(dni > 99999999 || dni < 10000000) {
            printf("El DNI debe tener 8 digitos. Ingrese nuevamente: ");
        } else if(buscarPacientePorDni(*listaP, dni) == 0) {
            char registrar;
            printf("El paciente no existe. Desea registrarlo ahora? (s/n): ");
            scanf(" %c", &registrar);
            getchar();
            if(registrar == 's' || registrar == 'S') {
                cargar_paciente_teclado(listaP);
            } else {
                printf("Ingrese otro DNI o 0 para salir: ");
                dni = -1;
            }
        }
    } while (dni > 99999999 || dni < 10000000 || buscarPacientePorDni(*listaP, dni) == 0);

    //Eze: "2. Seleccionamos el mes."
    printf("\nSeleccione el mes: [1] Noviembre  [2] Diciembre\nOpcion: ");
    do {
        scanf("%d", &mes_input);
        getchar();
        if(mes_input != 1 && mes_input != 2) {
            printf("Opcion invalida. Ingrese 1 para Noviembre o 2 para Diciembre: ");
        }
    } while(mes_input != 1 && mes_input != 2);
    mes = (mes_input == 1) ? 11 : 12;

    //Eze: "3. Dias disponibles en vertical."
    printf("\n--- Dias disponibles en el mes %d ---\n", mes);
    int dias_impresos[32] = {0};
    int hay_dias = 0;
    const char* nombres_dias[] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};

    for(i = 0; i < MAX_AGENDA; i++) {
        if(agenda[i].mes == mes && agenda[i].disponible == 1) {
            int d = agenda[i].dia;
            if(es_feriado(d, mes) == 0 && dias_impresos[d] == 0) {
                int idx_dia = (mes == 11) ? (d - 1) % 7 : (d + 29) % 7;
                printf("- %02d %s\n", d, nombres_dias[idx_dia]);
                dias_impresos[d] = 1;
                hay_dias = 1;
            }
        }
    }

    if(!hay_dias) {
        printf("\nNo hay dias disponibles para este mes.\n");
        set_turno_id(&t, 0);
        return t;
    }

    printf("\nIngrese el dia deseado de los mostrados arriba: ");
    int dia_valido = 0;
    do {
        scanf("%d", &dia);
        getchar();
        if(dia < 1 || dia > 31 || dias_impresos[dia] == 0) {
            printf("Dia invalido o sin turnos libres. Ingrese un dia de la lista: ");
        } else {
            dia_valido = 1;
        }
    } while(!dia_valido);

    //Eze: "4. Horarios disponibles."
    printf("\n--- Horarios disponibles para el %02d/%02d ---\n", dia, mes);
    for(i = 0; i < MAX_AGENDA; i++) {
        if(agenda[i].mes == mes && agenda[i].dia == dia && agenda[i].disponible == 1 && es_feriado(dia, mes) == 0) {
            printf("- %02d:%02d\n", agenda[i].hora, agenda[i].minuto);
        }
    }

    printf("\nIngrese la hora y los minutos deseados (ejemplo: 8 30): ");
    int horario_valido = 0;
    Fecha fecha_elegida;
    do {
        scanf("%d %d", &hora, &minuto);
        getchar();

        init_fecha(&fecha_elegida);
        set_fecha(&fecha_elegida, dia, mes, hora, minuto, 1);

        indice_fecha_nueva = indice_fecha(fecha_elegida, MAX_AGENDA, agenda);
        if(indice_fecha_nueva != -1 && agenda[indice_fecha_nueva].disponible == 1) {
            horario_valido = 1;
        } else {
            printf("Horario invalido o no disponible. Ingrese nuevamente (hora minuto): ");
        }
    } while(!horario_valido);

    //Eze: "5. Asignamos la fecha y dejamos monto/mutual en 0. Se definen al Confirmar."
    set_turno_fecha(&t, fecha_elegida);
    set_turno_dni_paciente(&t, dni);
    set_turno_mutual(&t, 0);
    set_turno_monto(&t, 0.0);
    set_turno_estado(&t, 0); // 0 = Solicitado
    set_turno_id(&t, indice_fecha_nueva + 1);
    agenda[indice_fecha_nueva].disponible = 0;
    return t;
}

// Eze: busca un turno en la lista por ID, le pone estado 3 (Cancelado) y cambia disponible=1 en la agenda.
void cancelar_y_liberar(ListaVo *lista, Fecha agenda[], int id_turno) {
    reset(lista);
    while(!isOos(*lista)) {
        Turno t = copyTurno(*lista);
        if(get_turno_id(t) == id_turno) {
            if(get_turno_estado(t) == 3) {
                printf("El turno ya estaba cancelado.\n");
                return;
            }
            modificar_estado_turno(&t, 3);
            actualizarTurnoActual(lista, t); // Pisamos el nodo usando nuestro nuevo actualizador

            int ind = indice_fecha(get_turno_fecha(t), MAX_AGENDA, agenda);
            if(ind != -1) agenda[ind].disponible = 1; // Liberar el bloque de la agenda
            printf("Turno cancelado y fecha liberada con exito.\n");
            return;
        }
        forward(lista);
    }
    printf("Turno no encontrado.\n");
}

// Eze: como la lista esta ordenada, busca el primer turno con estado 0 o 1 y te da opciones para atenderlo o cancelarlo
void atender_proximo_paciente(ListaVo *lista, Fecha agenda[]) {
    reset(lista);
    while(!isOos(*lista)) {
        Turno t = copyTurno(*lista);
        if(get_turno_estado(t) == 0 || get_turno_estado(t) == 1) {
            printf("\n--- Proximo Paciente a Atender ---\n");
            ver_datos_turno(t);
            int opcion;
            printf("Que desea hacer? [1] Atender [2] Cancelar (No asistio) [0] Salir: ");
            scanf("%d", &opcion);
            getchar();

            if(opcion == 1) {
                modificar_estado_turno(&t, 2); // Atendido
                actualizarTurnoActual(lista, t);
                printf("Turno marcado como Atendido.\n");
            } else if(opcion == 2) {
                cancelar_y_liberar(lista, agenda, get_turno_id(t));
            }
            return;
        }
        forward(lista);
    }
    printf("No hay pacientes proximos para atender.\n");
}

// Eze: abre (o crea) un archivo 'cancelados.txt' (append), guarda ahi los turnos con estado 3 y los borra de la lista (suppressTurno)
void eliminar_turnos_cancelados(ListaVo *lista) {
    FILE *fp = fopen("cancelados.txt", "a");
    if(fp == NULL) {
        printf("Error al abrir el archivo cancelados.txt\n");
        return;
    }
    int eliminados = 0;
    reset(lista);
    while(!isOos(*lista)) {
        Turno t = copyTurno(*lista);
        if(get_turno_estado(t) == 3) {
            fprintf(fp, "ID Turno: %d, DNI: %ld, Fecha: %02d/%02d/%d %02d:%02d\n",
                    get_turno_id(t), get_turno_dni_paciente(t),
                    get_dia(get_turno_fecha(t)), get_mes(get_turno_fecha(t)), get_anio(get_turno_fecha(t)),
                    get_hora(get_turno_fecha(t)), get_minuto(get_turno_fecha(t)));

            suppressTurno(lista); // Borramos el nodo liberando con free()
            eliminados++;
        } else {
            forward(lista);
        }
    }
    fclose(fp);
    printf("Se han eliminado y guardado %d turnos cancelados.\n", eliminados);
}

// AGUSTIN
void mostrar_turnos_por_dni(ListaVo listaTurnos, long dniBuscado) {
    if (isEmpty(listaTurnos)) {
        printf("No hay turnos registrados en el sistema.\n");
        return;
    }
    int encontrados = 0;
    reset(&listaTurnos);
    printf("\n=== TURNOS DEL PACIENTE CON DNI: %ld ===\n", dniBuscado);
    while (!isOos(listaTurnos)) {
        Turno t = copyTurno(listaTurnos);
        if (get_turno_dni_paciente(t) == dniBuscado) {
            ver_datos_turno(t);
            encontrados++;
        }
        forward(&listaTurnos);
    }
    if (encontrados == 0) {
        printf("No se encontraron turnos asociados al DNI %ld.\n", dniBuscado);
    }
}

void insertarTurnoOrdenado(ListaVo *lista, Turno nuevoTurno) {
    Fecha fechaNueva = get_turno_fecha(nuevoTurno);
    reset(lista);

    // 2. Avanzar mientras la fecha del turno actual sea menor o igual a la nueva
    while (!isOos(*lista)) {
        Turno turnoActual = copyTurno(*lista);
        if (comparar_fechas(get_turno_fecha(turnoActual), fechaNueva) > 0) {
            // Se encontró un turno con fecha posterior: detener el cursor en esta posición
            break;
        }
        forward(lista);
    }

    agregarTurno(lista, nuevoTurno);
}

void carga_n_turnos(ListaVo *listaT, Fecha agenda[], ListaEpaciente *listaP, int n) {
    int i;
    for(i = 0; i < n; i++) {
        if(isFull(*listaT)) {
            printf("No se pudo cargar el turno. La lista de turnos esta llena.\n");
            break;
        }
        Turno t = nuevo_turno(listaT, agenda, listaP);
        if(t.id_turno == 0) {
            printf("Carga de turno cancelada por el usuario.\n");
            break;
        }
        insertarTurnoOrdenado(listaT, t);
        printf("Turno cargado exitosamente.\n");

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
            if(continuar == 'n' || continuar == 'N') break;
        }
    }
}

void modificar_id_turno(ListaVo listaT, Turno *t, int nuevo_id) {
    if(t->estado != 3) {
        if(buscarIdTurno(listaT, nuevo_id) == 0) {
            set_turno_id(t, nuevo_id);
        } else {
            printf("El ID de turno ya existe.\n");
        }
    } else {
        printf("No se puede modificar el ID de un turno cancelado.\n");
    }
}

void modificar_dni_turno(Turno *t, ListaEpaciente listaP, long int nuevo_dni) {
    if(t->estado != 3) {
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
    if(t->estado != 3) {
        if(nuevo_monto >= 0 && t->mutual == 0) {
            set_turno_monto(t, nuevo_monto);
        } else {
            printf("El monto debe ser positivo y el turno no debe tener mutual.\n");
        }
    } else {
        printf("No se puede modificar el monto de un turno cancelado.\n");
    }
}

void modificar_mutual_turno(Turno *t, int nuevo_mutual) {
    if(t->estado != 3) {
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
    if(t->estado != 3) {
        int indice_fecha_nueva = -1;
        int indice_fecha_actual = -1;
        if(comparar_fechas(nueva_fecha, t->fecha) == 0) {
            printf("La fecha seleccionada es la misma que la actual.\n");
            return;
        }
        for(i = 0; i < tamanio_agenda; i++) {
            if(agenda[i].dia == nueva_fecha.dia && agenda[i].mes == nueva_fecha.mes &&
                agenda[i].hora == nueva_fecha.hora && agenda[i].minuto == nueva_fecha.minuto) {
                indice_fecha_nueva = i;
            }
            if(agenda[i].dia == t->fecha.dia && agenda[i].mes == t->fecha.mes &&
                agenda[i].hora == t->fecha.hora && agenda[i].minuto == t->fecha.minuto) {
                indice_fecha_actual = i;
            }
        }
        if(indice_fecha_nueva < 0 || fecha_disponible(nueva_fecha, tamanio_agenda, agenda) == 0) {
            printf("La fecha seleccionada no esta disponible en la agenda.\n");
            return;
        }
        if(indice_fecha_actual >= 0) {
            agenda[indice_fecha_actual].disponible = 1;
        }
        agenda[indice_fecha_nueva].disponible = 0;
        set_turno_fecha(t, nueva_fecha);
    } else {
        printf("No se puede modificar la fecha de un turno cancelado.\n");
    }
}

void modificar_estado_turno(Turno *t, int nuevo_estado) {
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
        case 0: printf("Mutual: Ninguna\n"); break;
        case 1: printf("Mutual: Mutual 1\n"); break;
        case 2: printf("Mutual: Mutual 2\n"); break;
        case 3: printf("Mutual: Mutual 3\n"); break;
        case 4: printf("Mutual: Mutual 4\n"); break;
        default: printf("Mutual desconocida.\n");
    }
}

void mostrar_turnos_reservados(ListaVo lista, int* pagina) {
    if(isEmpty(lista)) {
        printf("No hay turnos reservados.\n");
        return;
    }
    int total = 0;
    reset(&lista);
    while(!isOos(lista)) {
        total++;
        forward(&lista);
    }
    int total_paginas = (total + 9) / 10;
    if(*pagina > total_paginas) *pagina = total_paginas;
    if(*pagina < 1) *pagina = 1;
    int i;
    int encontrados = 0;
    char opcion;
    Turno turno;
    do {
        reset(&lista);
        for(i = 0; i < (*pagina - 1) * 10; i++) {
            forward(&lista);
        }
        printf("=== Turnos Reservados ===\n");
        for(i = 0; i < 10 && !isOos(lista); i++) {
            turno = copyTurno(lista);
            if(get_turno_estado(turno)==0){
                ver_datos_turno(turno);
                encontrados ++;
            }
            forward(&lista);
        }
        if(encontrados > 0){
            printf("=== Pagina %d / %d ===\n", *pagina, total_paginas);
            int valida;
            do {
                printf("[S]iguiente  [A]nterior  [N] Salir: ");
                scanf(" %c", &opcion);
                getchar();
                valida = (opcion == 'n' || opcion == 'N') ||
                         ((opcion == 's' || opcion == 'S') && *pagina < total_paginas) ||
                         ((opcion == 'a' || opcion == 'A') && *pagina > 1);
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
        }else{
            printf("No hay turnos reservados!\n");
            opcion = 'N';
        }

    } while(opcion != 'n' && opcion != 'N');
}

void ver_estado_turno(Turno t) {
    int estado = get_turno_estado(t);
    switch(estado) {
        case 0: printf("Estado del turno: Solicitado\n"); break;
        case 1: printf("Estado del turno: Confirmado\n"); break;
        case 2: printf("Estado del turno: Atendido\n"); break;
        case 3: printf("Estado del turno: Cancelado\n"); break;
        default: printf("Estado de turno desconocido.\n");
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
        int estado_turno = get_turno_estado(t);
        if(get_mes(get_turno_fecha(t))==mes && (estado_turno == 1 || estado_turno == 2)){
            monto_ingresado += get_turno_monto(t);
        }
        forward(&listaTurnos);
    }
    return monto_ingresado;
}


// ====================== FUNCIONES FECHA ======================

// Eze: pide el dia y mes, y recorre el arreglo agenda buscando e imprimiendo las fechas con disponible==1
void mostrar_fechas_libres_dia_mes(Fecha agenda[], int dia, int mes) {
    int i, encontradas = 0;
    printf("\n--- Fechas Libres para el %02d/%02d ---\n", dia, mes);
    for(i = 0; i < MAX_AGENDA; i++) {
        if(agenda[i].dia == dia && agenda[i].mes == mes && agenda[i].disponible == 1) {
            printf("ID Agenda: %3d | Hora: %02d:%02d\n", i, agenda[i].hora, agenda[i].minuto);
            encontradas++;
        }
    }
    if(encontradas == 0) printf("No se encontraron fechas libres.\n");
}

// Eze: recorre el arreglo agenda y muestra todas las fechas del mes que tienen disponible==0
void mostrar_fechas_ocupadas_mes(Fecha agenda[], int mes) {
    int i, encontradas = 0;
    printf("\n--- Fechas Ocupadas para el mes %02d ---\n", mes);
    for(i = 0; i < MAX_AGENDA; i++) {
        if(agenda[i].mes == mes && agenda[i].disponible == 0) {
            printf("ID Agenda: %3d | Dia: %02d | Hora: %02d:%02d\n", i, agenda[i].dia, agenda[i].hora, agenda[i].minuto);
            encontradas++;
        }
    }
    if(encontradas == 0) printf("No se encontraron fechas ocupadas.\n");
}

void ver_fecha(Fecha f) {
    printf("Fecha: %02d/%02d/%04d %02d:%02d\n", get_dia(f), get_mes(f), get_anio(f), get_hora(f), get_minuto(f));
}

// ====================== MENÚS ======================
void mostrar_menu() {
    printf("\n========== MENU ==========\n");
    printf("1. Pacientes\n");
    printf("2. Turnos\n");
    printf("3. Fechas\n");
    printf("4. Persistencia de datos\n");
    printf("5. Limpiar pantalla\n");
    printf("0. Salir\n");
    printf("==========================\n");
}

void mostrar_menu_pacientes() {
    printf("\n=== Menu Pacientes ===\n");
    printf("1. Nuevo paciente\n");
    printf("2. Mostrar todos los pacientes\n");
    printf("3. Buscar paciente por DNI\n");
    printf("4. Mostrar turnos de un paciente por DNI\n");
    printf("5. Eliminar paciente sin turnos\n");
    printf("6. Limpiar pantalla\n");
    printf("0. Volver al menu principal\n");
    printf("======================\n");
}

void operaciones_menu_pacientes(int subopcion_pacientes, ListaEpaciente* listaPacientes, ListaVo* listaTurnos) {
    long dni;
    switch (subopcion_pacientes) {
        case 1: {
            cargar_paciente_teclado(listaPacientes);
            pausa();
            break;
        }
        case 2: {
            mostrar_todos_los_pacientes(listaPacientes);
            pausa();
            break;
        }
        case 3: {
            printf("Ingrese el DNI del paciente: ");
            scanf("%ld", &dni);
            mostrar_paciente_por_dni(*listaPacientes, dni);
            pausa();
            break;
        }
        case 4: {
            printf("Ingrese el DNI del paciente: ");
            scanf("%ld", &dni);
            mostrar_turnos_por_dni(*listaTurnos, dni);
            pausa();
            break;
        }
        case 5: {
            printf("Ingrese el DNI del paciente a eliminar: ");
            scanf("%ld", &dni);
            eliminar_paciente_sin_turnos(listaPacientes, *listaTurnos, dni);
            pausa();
            break;
        }
        case 6: system("cls"); break;
        case 0: printf("\nVolviendo al menu principal...\n"); break;
        default: printf("Opcion invalida.\n"); break;
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
    printf("7. Confirmar turno\n");
    printf("8. Cancelar turno y liberar fecha\n");
    printf("9. Atender proximo paciente\n");
    printf("10. Guardar y purgar turnos cancelados\n");
    printf("11. Buscar y mostrar turno por ID\n");
    printf("12. Limpiar pantalla\n");
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
                    insertarTurnoOrdenado(listaTurnos, t);
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
            do {
                scanf("%d", &n);
                getchar();
                if(n <= 0) printf("La cantidad debe ser mayor a 0. Ingrese nuevamente: ");
                else if(n > MAX_TURNOS) printf("La cantidad no puede superar %d. Ingrese nuevamente: ", MAX_TURNOS);
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
            do {
                scanf("%d", &mes);
                getchar();
                if(mes != 1 && mes != 2) printf("Ingrese una opcion valida.\n");
            } while (mes != 1 && mes != 2);
            reset(listaTurnos);
            ver_turnos_asignados_mes(*listaTurnos, mes + 10);
            pausa();
            break;
        }
        case 5:{
            int dia, mes, atendidos;
            int dia_valido = 1;
            printf("Ingrese el mes del cual quiere ver los turnos atendidos [1]Noviembre [2]Diciembre\n");
            do {
                scanf("%d", &mes);
                getchar();
                if(mes != 1 && mes != 2) printf("Ingrese una opcion valida.\n");
            } while (mes != 1 && mes != 2);

            printf("Ingrese el dia del cual quiere ver los turnos atendidos (1-31)\n");
            do {
                dia_valido = 1;
                scanf("%d", &dia);
                getchar();
                if(dia < 1 || dia > 31){
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
            pausa();
            break;
        }
        case 6:{
            int mes;
            float monto_ingresado;
            printf("Ingrese el mes del cual quiere ver las ganancias? [1]Noviembre [2]Diciembre\n");
            do {
                scanf("%d", &mes);
                getchar();
                if(mes != 1 && mes != 2) printf("Ingrese una opcion valida.\n");
            } while (mes != 1 && mes != 2);
            reset(listaTurnos);
            monto_ingresado = ver_monto_ingresado_mes(*listaTurnos, mes+10);
            if(mes==1){
                printf("En el mes de Noviembre se obtuvo una ganancia de %.2f\n", monto_ingresado);
            }else{
                printf("En el mes de Diciembre se obtuvo una ganancia de %.2f\n", monto_ingresado);
            }

            pausa();
            break;
        }
        case 7: {

    int id;
    printf("Ingrese el ID del turno a confirmar: ");
    scanf("%d", &id);
    getchar();
    Turno t = obtenerTurnoPorId(*listaTurnos, id);

    if(get_turno_id(t) != 0) {
        if(get_turno_estado(t) != 0) {
            printf("Error: Solo se pueden confirmar turnos que esten en estado 'Solicitado' (Pendiente).\n");
        } else {
            // Buscamos si el paciente asociado a este turno tiene obra social (1 o 0)
            long dni_paciente = get_turno_dni_paciente(t);
            int tiene_os = 0;
            Paciente p_aux;

            resetListaEpaciente(listaPacientes);
            while(!isoosListaEpaciente(*listaPacientes)) {
                p_aux = copyListaEpaciente(*listaPacientes);
                if(get_dni_paciente(p_aux) == dni_paciente) {
                    tiene_os = get_o_social_paciente(p_aux);
                    break;
                }
                forwardListaEpaciente(listaPacientes);
            }

            int mutual = 0;
            float monto = 0.0;

            if (tiene_os == 1) {
                do {
                    printf("\nEl paciente TIENE Obra Social registrada.\n");
                    printf("--- SELECCIONE LA MUTUAL ---\n");
                    printf("[1] Mutual 1 (Cobertura 60%% - Paga 40%%)\n");
                    printf("[2] Mutual 2 (Cobertura 60%% - Paga 40%%)\n");
                    printf("[3] Mutual 3 (Cobertura 100%% - No paga nada)\n");
                    printf("[4] Mutual 4 (Cobertura 100%% - No paga nada)\n");
                    printf("Ingrese opcion (1-4): ");
                    scanf("%d", &mutual);
                    getchar();
                    if(mutual < 1 || mutual > 4) printf("Opcion invalida.\n");
                } while (mutual < 1 || mutual > 4);

                if (mutual == 1 || mutual == 2) {
                    float monto_base;
                    printf("Ingrese el valor total de la consulta: $");
                    do {
                        scanf("%f", &monto_base);
                        getchar();
                        if(monto_base < 0) printf("El monto debe ser positivo.\n");
                    } while (monto_base < 0);
                    monto = monto_base * 0.40;
                    printf("Monto a cobrar con descuento: $%.2f\n", monto);
                } else {
                    printf("El paciente tiene cobertura 100%%. No debe abonar nada.\n");
                    monto = 0.0;
                }
            } else {
                printf("\nEl paciente NO TIENE Obra Social.\n");
                printf("Ingrese el valor total de la consulta: $");
                do {
                    scanf("%f", &monto);
                    getchar();
                    if(monto < 0) printf("El monto debe ser positivo.\n");
                } while (monto < 0);
            }

            // Usamos setters directos para no tener problemas con validaciones cruzadas
            set_turno_mutual(&t, mutual);
            set_turno_monto(&t, monto);
            modificar_estado_turno(&t, 1); // Lo pasamos a 1 (Confirmado)

            actualizarTurnoActual(listaTurnos, t);
            printf("\n Turno confirmado exitosamente!\n");
        }
    } else {
        printf("Turno no encontrado.\n");
    }
    pausa();
    break;
}
        case 8: {
            int id;
            printf("Ingrese el ID del turno a cancelar: ");
            scanf("%d", &id);
            getchar();
            cancelar_y_liberar(listaTurnos, agenda, id);
            pausa();
            break;
        }
        case 9: {
            atender_proximo_paciente(listaTurnos, agenda);
            pausa();
            break;
        }
        case 10: {
            eliminar_turnos_cancelados(listaTurnos);
            pausa();
            break;
        }
        case 11: {
            int id;
            printf("Ingrese el ID del turno a buscar: ");
            scanf("%d", &id);
            getchar();
            Turno t = obtenerTurnoPorId(*listaTurnos, id);
            if(get_turno_id(t) != 0) {
                printf("\n--- Datos del Turno Buscado ---\n");
                ver_datos_turno(t);
            } else {
                printf("No existe ningun turno con el ID %d.\n", id);
            }
            pausa();
            break;
        }
        case 12:{
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
    printf("1. Buscar fechas libres por dia y mes\n");
    printf("2. Buscar fechas ocupadas por mes\n");
    printf("3. Mostrar primeras fechas disponibles\n");
    printf("4. Limpiar pantalla\n");
    printf("0. Volver al menu principal\n");
    printf("====================\n");
}

void operaciones_menu_fechas(int subopcion_fechas, Fecha agenda[]){
    switch(subopcion_fechas){
        case 1: {
            int dia, mes;
            printf("Ingrese el mes ([11] Noviembre o [12] Diciembre): ");
            scanf("%d", &mes);
            printf("Ingrese el dia: ");
            scanf("%d", &dia);
            getchar();
            mostrar_fechas_libres_dia_mes(agenda, dia, mes);
            pausa();
            break;
        }
        case 2: {
            int mes;
            printf("Ingrese el mes ([11] Noviembre o [12] Diciembre): ");
            scanf("%d", &mes);
            getchar();
            mostrar_fechas_ocupadas_mes(agenda, mes);
            pausa();
            break;
        }
        case 3:{
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
            pausa();
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
        }
    }
}

void pausa() {
    printf("\n");
    system("pause"); // Eze: para asegurarnos que espere siempre y no consuma basura del teclado
}

void mostrar_menu_persistencias(ListaVo* listaTurnos, ListaEpaciente* listaPacientes, Fecha agenda[]) {
    int opcion;
    do {
        printf("\n--- PERSISTENCIA DE DATOS ---\n");
        printf("1. Guardar datos del sistema\n");
        printf("2. Cargar datos del sistema\n");
        printf("3. Volver al menu principal\n");
        printf("Seleccione una opcion: ");

        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                guardar_turnos(listaTurnos);
                guardar_pacientes(listaPacientes);
                guardar_agenda(agenda);
                printf("Backup realizado correctamente.\n");
                break;
            case 2:
                cargar_pacientes(listaPacientes);
                cargar_agenda(agenda);
                cargar_turnos(listaTurnos);
                printf("Carga finalizada.\n");
                break;
            case 3:
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (opcion != 3);
}

// ====================== PERSISTENCIA DE DATOS ======================
void guardar_pacientes(ListaEpaciente pacientes[]){
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

        modificar_nombre_paciente(&p, nombre);
        modificar_apellido_paciente(&p, apellido);
        modificar_dni_paciente(&p, dni);
        modificar_o_social_paciente(&p, o_social);

        if (!buscarPacientePorDni(*pacientes, dni)) {
            insertListaEpaciente(pacientes, p);
            cargados++;
        }
    }

    fclose(archivo);
    return cargados;
}

void guardar_turnos(ListaVo turnos[]){
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
    int i = 0;
    int cargados = 0;
    int id, mutual, dia, mes, anio;
    int hora, minuto, disponible, estado;
    long dni;
    float monto;
    Turno t;
    Fecha f;

    archivo = fopen("turno.txt", "r");
    if (archivo == NULL) return 0;
    init_fecha(&f);

    // Aclaracion: asumo que init_turno(&t) esta en tu turno.h por la implementacion de Agustin
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
        if (!turno_existe(*turnos, id)) {
            insertarTurnoOrdenado(turnos, t);
            cargados++;
        }
        i++;
    }

    fclose(archivo);
    return cargados;
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

int main() {
    Fecha agenda[MAX_AGENDA];
    ListaEpaciente listaPacientes;
    ListaVo listaTurnos;

    initListaE_paciente(&listaPacientes);
    inicializarLista(&listaTurnos);
    inicializarAgenda(agenda);

    int opcion;
    int subopcion;
    int pagina_turnos = 1;

    do {
        mostrar_menu();
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: {
                do {
                    mostrar_menu_pacientes();
                    scanf("%d", &subopcion);
                    operaciones_menu_pacientes(subopcion, &listaPacientes, &listaTurnos);
                } while (subopcion != 0);
                break;
            }
            case 2: {
                do {
                    mostrar_menu_turnos();
                    scanf("%d", &subopcion);
                    operaciones_menu_turnos(subopcion, &listaTurnos, agenda, &listaPacientes, &pagina_turnos);
                } while (subopcion != 0);
                break;
            }
            case 3: {
                do {
                    mostrar_menu_fechas();
                    scanf("%d", &subopcion);
                    operaciones_menu_fechas(subopcion, agenda);
                } while (subopcion != 0);
                break;
            }
            case 4: {
                mostrar_menu_persistencias(&listaTurnos, &listaPacientes, agenda);
                break;
            }
            case 5: {
                system("cls");
                break;
            }
            case 0: {
                printf("Saliendo del sistema...\n");
                break;
            }
            default: {
                printf("Opcion invalida.\n");
                break;
            }
        }
    } while (opcion != 0);

    return 0;
}
