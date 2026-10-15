#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "paciente.h"
#include "fecha.h"
#include "turno.h"
#include "listaE_paciente.h"
#include "listaVO_turno.h"

#define MAX_AGENDA 390

//CREAMOS LA AGENDA CON UNA CONSTANTE YA DEFINIDA

void inicializarAgenda(fecha agenda[MAX_AGENDA]) {
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
            int minutos = 0;
            int t;

            // aca generamos los 10 turnos que va a contener ese dia. desde las 8:00am hasta las 12:30pm
            for (t = 0; t < 10 && index < MAX_AGENDA; t++) {
                agenda[index].dia = dia;
                agenda[index].mes = mes;
                agenda[index].anio = anio;
                agenda[index].hora = hora;
                agenda[index].minutos = minutos;
                agenda[index].disponible = 1; // anteriormente tenemos que 1 significaba libre

                index++; // aca paso al siguiente casillero de la agenda

                // como los turnos son cada 30 minutos, le incremento 30 para el siguiente turno
                minutos += 30;
                if (minutos >= 60) { // comparo los minutos y en caso de ser mayor o igual a60
                    minutos = 0;     //restaurmanos minutos a 0 y le sumamos +1 a horas
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
//------------------------------------------------------------


int main() {
    // 1. Inicializar la agenda auxiliar de 390 turnos automáticamente
    fecha agenda[MAX_AGENDA];
    inicializarAgenda(agenda);

    // 2. Inicializar las estructuras de listas
    listaEpaciente listaPacs;
    initListaE_paciente(&listaPacs);

    ListaVOTurno listaTurnos;
    initListaVOTurno(&listaTurnos);

    int opcion;
    long dniBuscado;

    do {
        printf("\n========================================\n");
        printf("   SISTEMA DE GESTION DE TURNOS - PMI   \n");
        printf("========================================\n");
        printf("1. Registrar nuevo paciente\n");
        printf("2. Mostrar todos los pacientes\n");
        printf("3. Ver turnos libres en la agenda\n");
        printf("4. Solicitar un turno\n");
        printf("5. Mostrar turnos asignados (ordenados)\n");
        printf("6. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        fflush(stdin); // Limpiar buffer

        switch (opcion) {
            case 1: {
                if (FulllistaEpaciente(listaPacs)) {
                    printf("\nLa lista estatica de pacientes esta llena.\n");
                    break;
                }
                char nom[30], ape[30];
                long d;
                int oso;

                printf("\n--- REGISTRAR PACIENTE ---\n");
                printf("Ingrese nombre: ");
                scanf("%s", nom);
                printf("Ingrese apellido: ");
                scanf("%s", ape);
                printf("Ingrese DNI: ");
                scanf("%ld", &d);
                printf("Tiene obra social? (1 = Si, 0 = No): ");
                scanf("%d", &oso);

                paciente nuevoP;
                initpaciente(&nuevoP);
                modificarpaciente(&nuevoP, nom, ape, d, oso);

                if (insertlistaEpaciente(&listaPacs, nuevoP)) {
                    printf("\n¡Paciente registrado con exito!\n");
                }
                break;
            }
            case 2: {
                printf("\n--- LISTA DE PACIENTES REGISTRADOS ---\n");
                if (EmptylistaE_paciente(listaPacs)) {
                    printf("No hay pacientes cargados.\n");
                } else {
                    ResetlistaEpaciente(&listaPacs);
                    while (!isooslistaEpaciente(listaPacs)) {
                        paciente pActual;
                        copylistaEpaciente(listaPacs, &pActual);
                        mostrarpaciente(pActual);
                        printf("\n");
                        forwardlistaEpaciente(&listaPacs);
                    }
                }
                break;
            }
            case 3: {
                printf("\n--- PRIMEROS TURNOS LIBRES EN AGENDA ---\n");
                int mostrados = 0;
                int i;
                for ( i = 0; i < MAX_AGENDA && mostrados < 15; i++) {
                    if (agenda[i].disponible == 1) {
                        printf("ID Turno (Indice): %3d | Fecha: %02d/%02d/%d - %02d:%02d\n",
                               i, agenda[i].dia, agenda[i].mes, agenda[i].anio, agenda[i].hora, agenda[i].minutos);
                        mostrados++;
                    }
                }
                printf("(Mostrando los primeros 15 turnos libres disponibles)\n");
                break;
            }
            case 4: {
                printf("\n--- SOLICITAR TURNO ---\n");
                printf("Ingrese el DNI del paciente: ");
                scanf("%ld", &dniBuscado);

                paciente pacienteEncontrado;
                int encontrado = 0;

                // Buscar si el paciente ya existe en la lista estática
                int i = 0;
                while (i < listaPacs.cant) {
                    if (listaPacs.v[i].dni == dniBuscado) {
                        pacienteEncontrado = listaPacs.v[i];
                        encontrado = 1;
                        break;
                    }
                    i++;
                }

                // Si no está registrado, se lo registra en el momento
                if (!encontrado) {
                    if (FulllistaEpaciente(listaPacs)) {
                        printf("El paciente no esta registrado y la lista de pacientes esta llena.\n");
                        break;
                    }
                    printf("\nEl paciente con DNI %ld no existe. Debe registrarlo:\n", dniBuscado);
                    char nom[30], ape[30];
                    int oso;
                    printf("Ingrese nombre: ");
                    scanf("%s", nom);
                    printf("Ingrese apellido: ");
                    scanf("%s", ape);
                    printf("Tiene obra social? (1 = Si, 0 = No): ");
                    scanf("%d", &oso);

                    initpaciente(&pacienteEncontrado);
                    modificarpaciente(&pacienteEncontrado, nom, ape, dniBuscado, oso);
                    insertlistaEpaciente(&listaPacs, pacienteEncontrado);
                    printf("¡Paciente registrado automaticamente!\n");
                }

                // Pedir el ID de la agenda (índice)
                int idTurnoElegido;
                printf("Ingrese el ID de turno deseado (indice de la agenda): ");
                scanf("%d", &idTurnoElegido);

                if (idTurnoElegido < 0 || idTurnoElegido >= MAX_AGENDA) {
                    printf("ID de turno fuera de rango (0 a 389).\n");
                    break;
                }

                if (agenda[idTurnoElegido].disponible == 1) {
                    agenda[idTurnoElegido].disponible = 0; // Marcar como ocupado

                    float monto = 0.0;
                    char mutual[30] = "";

                    if (pacienteEncontrado.osocial == 1) {
                        printf("Ingrese la mutual (OSDE, Swiss Medical, Galeno, PAMI): ");
                        scanf("%s", mutual);
                    } else {
                        printf("Ingrese el monto pagado por la consulta: ");
                        scanf("%f", &monto);
                    }

                    turno nuevoT;
                    initTurno(&nuevoT);
                    cargaTurno(&nuevoT, &agenda[idTurnoElegido], idTurnoElegido, dniBuscado, monto, mutual, "solicitado");

                    insertListaVOTurno(&listaTurnos, nuevoT);
                    printf("\n¡Turno solicitado y asignado correctamente con ID %d!\n", idTurnoElegido);
                } else {
                    printf("\nEl turno seleccionado ya se encuentra ocupado. Elija otro.\n");
                }
                break;
            }
            case 5: {
                printf("\n--- LISTA DE TURNOS ASIGNADOS (Ordenados cronologicamente) ---\n");
                if (EmptyListaVOTurno(listaTurnos)) {
                    printf("No hay turnos asignados en la lista.\n");
                } else {
                    ResetListaVOTurno(&listaTurnos);
                    while (!isoosListaVOTurno(listaTurnos)) {
                        turno tActual;
                        copyListaVOTurno(listaTurnos, &tActual);
                        mostrarTurno(tActual);
                        forwardListaVOTurno(&listaTurnos);
                    }
                }
                break;
            }
            case 6:
                printf("\nSaliendo del sistema...\n");
                break;
            default:
                printf("\nOpcion no valida. Intente de nuevo.\n");
        }

    } while (opcion != 6);

    return 0;
}
