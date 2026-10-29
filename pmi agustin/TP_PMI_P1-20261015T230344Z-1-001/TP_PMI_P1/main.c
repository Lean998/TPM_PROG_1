#include <stdio.h>
#include <stdlib.h>
#include "paciente.h"
#include "listaE_paciente.h"
#include "listaVO.h"
#include "turno.h"
#define MAX_AGENDA 390
#define MAX_PACIENTES 100
#define MAX_TURNOS 390

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
            printf("El DNI debe tener 8 dígitos. Ingrese nuevamente: ");
        }else if(buscarPacientePorDni(*lista, dni) == 1) { // Si el paciente ya existe
            printf("El paciente con DNI %d ya existe. Ingrese otro DNI o salga (0) ", dni);
            dni = -1; // Forzar la repetición
        }
    } while(dni < 10000000 || dni > 99999999);

    printf("Ingrese la opcion de obra social (0-4): ");
    do {
        scanf("%d", &oSocial);
        getchar();
        if(oSocial < 0 || oSocial > 4) {
            printf("Opcion de obra social invalida. Ingrese nuevamente (0-4): ");
        }
    } while(oSocial < 0 || oSocial > 4);
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
    if(nuevo_oSocial >= 0 && nuevo_oSocial <= 4) {
        set_o_social_paciente(p, nuevo_oSocial);
    } else {
        printf("Opcion de obra social invalida.\n");
    }
}

void ver_o_social_paciente(Paciente p) {
    int oSocial = get_o_social_paciente(p);
    switch(oSocial) {
        case 0:{
            printf("Obra social: Ninguna\n"); break;
        }
        case 1:{
            printf("Obra social: Mutual 1\n"); break;
        }
        case 2:{
            printf("Obra social: Mutual 2\n"); break;
        }
        case 3:{
            printf("Obra social: Mutual 3\n"); break;
        }
        case 4:{
            printf("Obra social: Mutual 4\n"); break;
        }
        default:{
            printf("Obra social desconocida.\n");
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

// AGUSTIN

// 1. MOSTRAR LOS TURNOS DE UN PACIENTE SEGÚN SU DNI
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

// 2. CARGAR UN PACIENTE POR TECLADO (Con las 4 Obras Sociales y Cobertura)
void cargar_paciente_teclado(ListaEpaciente *listaP) {
    if (FullListaEpaciente(*listaP)) {
        printf("La lista de pacientes esta llena (Maximo 100).\n");
        return;
    }

    int dni, oSocial;
    char nombre[100], apellido[100];
    Paciente p;

    // Ingreso seguro de nombre y apellido
    printf("Ingrese el nombre del paciente: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    printf("Ingrese el apellido del paciente: ");
    fgets(apellido, sizeof(apellido), stdin);
    apellido[strcspn(apellido, "\n")] = '\0';

    // Validación de DNI (8 dígitos y no duplicado)
    do {
        printf("Ingrese el DNI del paciente (8 digitos, 0 para cancelar): ");
        scanf("%d", &dni);
        getchar(); // Limpiar buffer

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

    // Selección obligatoria de Obra Social mediante switch (op1 a op4)
    do {
        printf("\n--- SELECCIONE OBRA SOCIAL ---\n");
        printf("1. Mutual Op1 (Cubre 60%% del costo)\n");
        printf("2. Mutual Op2 (Cubre 60%% del costo)\n");
        printf("3. Mutual Op3 (Cubre 100%% del costo)\n");
        printf("4. Mutual Op4 (Cubre 100%% del costo)\n");
        printf("Ingrese opcion (1-4): ");
        scanf("%d", &oSocial);
        getchar(); // Limpiar buffer

        switch (oSocial) {
            case 1:
            case 2:
            case 3:
            case 4:
                // Opción válida
                break;
            default:
                printf("Opcion invalida. Debe elegir una obra social entre 1 y 4.\n");
                oSocial = -1; // Forzar repetición
        }
    } while (oSocial == -1);

    // Guardar datos usando los setters del TDA Paciente
    set_nombre_paciente(&p, nombre);
    set_apellido_paciente(&p, apellido);
    set_dni_paciente(&p, dni);
    set_o_social_paciente(&p, oSocial);

    insertListaEpaciente(listaP, p);
    printf("¡Paciente cargado exitosamente al sistema!\n");
}

// 3. ELIMINAR UN PACIENTE QUE NO TENGA TURNOS
void eliminar_paciente_sin_turnos(ListaEpaciente *listaP, ListaVo listaTurnos, long dniBuscado) {
    if (EmptylistaE_paciente(*listaP)) {
        printf("No hay pacientes registrados en el sistema.\n");
        return;
    }

    // Verificar si el paciente existe en la lista estática
    if (buscarPacientePorDni(*listaP, dniBuscado) == 0) {
        printf("El paciente con DNI %ld no se encuentra registrado.\n", dniBuscado);
        return;
    }

    // Verificar si el paciente tiene turnos reservados en la lista dinámica
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

    // Buscar la posición exacta del paciente en la lista estática para eliminarlo
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

// 4. MOSTRAR LOS DATOS DE UN PACIENTE POR DNI
void mostrar_paciente_por_dni(ListaEpaciente listaP, long dniBuscado) {
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

            int os = get_o_social_paciente(p);
            printf("Obra Social: Op%d ", os);
            if (os == 1 || os == 2) {
                printf("(Cobertura del 60%% - Paga 40%%)\n");
            } else if (os == 3 || os == 4) {
                printf("(Cobertura del 100%% - No paga nada)\n");
            }
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

// 5. MOSTRAR TODOS LOS PACIENTES
void mostrar_todos_los_pacientes(ListaEpaciente *listaP) {
    if (EmptylistaE_paciente(*listaP)) {
        printf("No hay pacientes registrados en el sistema.\n");
        return;
    }

    resetListaEpaciente(listaP);
    printf("\n=== LISTA COMPLETA DE PACIENTES ===\n");
    while (!isoosListaEpaciente(*listaP)) {
        Paciente p = copyListaEpaciente(*listaP);

        printf("Nombre: %s | Apellido: %s | DNI: %ld | Obra Social: Op%d\n",
               get_nombre_paciente(p),
               get_apellido_paciente(p),
               get_dni_paciente(p),
               get_o_social_paciente(p));

        forwardListaEpaciente(listaP);
    }
    printf("===================================\n");
}

// AGUSTIN


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
                printf("¿Desea cargar otro turno? (s/n): ");
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

void mostrar_turnos_reservados(ListaVo lista) {
    Turno t;
    if(isEmpty(lista)) {
        printf("No hay turnos reservados.\n");
        return;
    }
    reset(&lista);
    printf("=== Turnos Reservados ===\n");
    while(!isOos(lista)) {
        t = copyTurno(lista);
        ver_datos_turno(t);
        forward(&lista);
    }
}

void ver_estado_turno(Turno t) {
    int estado = get_turno_estado(t);
    switch(estado) {
        case 0:{
            printf("Estado del turno: Pendiente\n"); break;
        }
        case 1:{
            printf("Estado del turno: Confirmado\n"); break;
        }
        case 2:{
            printf("Estado del turno: Completado\n"); break;
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

// ====================== FUNCIONES FECHA ======================
void ver_fecha(Fecha f) {
    printf("Fecha: %02d/%02d/%04d %02d:%02d\n", get_dia(f), get_mes(f), get_anio(f), get_hora(f), get_minuto(f));
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

void ver_datos_turno(Turno t);
void ver_fecha(Fecha f);
void inicializarAgenda(Fecha agenda[MAX_AGENDA]);

int main()
{
Fecha agenda[MAX_AGENDA];
    ListaEpaciente listaPacientes;
    ListaVo listaTurnos;

    // Inicialización obligatoria de estructuras
    initListaE_paciente(&listaPacientes);
    inicializarLista(&listaTurnos);
    inicializarAgenda(agenda);

    int opcion;
    long dniIngresado;

    do {
        printf("\n==========================================\n");
        printf("     SISTEMA DE PRUEBA - MODULO PACIENTES\n");
        printf("==========================================\n");
        printf("1. Cargar un paciente por teclado (Op1-Op4)\n");
        printf("2. Mostrar todos los pacientes\n");
        printf("3. Mostrar datos de un paciente por DNI\n");
        printf("4. Mostrar turnos de un paciente por DNI\n");
        printf("5. Eliminar un paciente (si no tiene turnos)\n");
        printf("0. Salir\n");
        printf("==========================================\n");
        printf("Ingrese una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            printf("Error: Ingrese un numero valido.\n");
            while (getchar() != '\n'); // Limpiar buffer de entrada
            continue;
        }
        getchar(); // Limpiar el salto de línea pendiente

        switch (opcion) {
            case 1:
                printf("\n--- CARGAR PACIENTE ---\n");
                cargar_paciente_teclado(&listaPacientes);
                break;

            case 2:
                mostrar_todos_los_pacientes(&listaPacientes);
                break;

            case 3:
                printf("\n--- BUSCAR PACIENTE POR DNI ---\n");
                printf("Ingrese el DNI a buscar: ");
                scanf("%ld", &dniIngresado);
                getchar();
                mostrar_paciente_por_dni(listaPacientes, dniIngresado);
                break;

            case 4:
                printf("\n--- TURNOS DE UN PACIENTE ---\n");
                printf("Ingrese el DNI del paciente: ");
                scanf("%ld", &dniIngresado);
                getchar();
                mostrar_turnos_por_dni(listaTurnos, dniIngresado);
                break;

            case 5:
                printf("\n--- ELIMINAR PACIENTE ---\n");
                printf("Ingrese el DNI del paciente a eliminar: ");
                scanf("%ld", &dniIngresado);
                getchar();
                eliminar_paciente_sin_turnos(&listaPacientes, listaTurnos, dniIngresado);
                break;

            case 0:
                printf("\nSaliendo del programa de prueba...\n");
                break;

            default:
                printf("\nOpcion invalida. Por favor, elija entre 0 y 5.\n");
        }

        if (opcion != 0) {
            printf("\nPresione Enter para continuar...");
            getchar();
        }

    } while (opcion != 0);

    return 0;

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



