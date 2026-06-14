#include <stdio.h>
#include <stdlib.h>
#include <termios.h>    //para leer tecla sin enter en linuxx
#include <unistd.h>
#include "juego.h"
#include "mapas.h"


char leer_tecla(){
    struct termios viejo, nuevo;
    tcgetattr(STDIN_FILENO, &viejo); //se guarda cconfiguracion actual
    nuevo=viejo;
    nuevo.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &nuevo);   //se desactiva el buffer y el eco
    char c=getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &viejo);   //se restaura config
    return c;
}

int jugar_nivel(char mapa[FILAS][COLUMNAS], Nivel *n, int *pasos_acum, int *monedas_acum){
    Jugador j;

    //se inicializa al jugador buscando la 'p' en el mapa
    j.monedas=0;
    j.pasos=0;
    j.tiene_llave=0;

    //se busca la posicion inicial del jugador en el mapa
    j.fila=1;
    j.col=1;
    for(int f=0; f<FILAS; f++){
        for(int c=0; c<COLUMNAS; c++){
            if(mapa[f][c] == JUGADOR){
                j.fila=f;
                j.col=c;
                mapa[f][c]= CAMINO; //se borra la P del mapa 
            }
        }
    }

    //funcion 1 nasm: calcula total de monedas del nivel recorriendo el mapa
    n->total_monedas= contar_caracteres(&mapa[0][0], FILAS*COLUMNAS, MONEDA);

    //funcion 5 nasm: cuneta celdas libres al inicial el nivel
    n->celdas_libres= contar_celdas(&mapa[0][0], FILAS *COLUMNAS);

    printf("\n  Nivel %d iniciado!\n", n->numero);
    printf("  Celdas libres en el mapa: %d\n", n->celdas_libres);
    printf("  Monedas en el mapa: %d\n", n->total_monedas);
    printf("  Presiona WASD para moverte, Q para salir\n\n");


    //bucle principal del nivel
    while(1){
        system("clear");
        imprimir_ventana(mapa, &j);
        mostrar_hud(&j, n);

        char tecla = leer_tecla();

        if(tecla == 'q' || tecla == 'Q'){
            printf("\nSalistye del juego!\n");
            exit(0);
        }


    }
}
