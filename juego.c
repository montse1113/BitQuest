#include <stdio.h>
#include <stdlib.h>
#include "juego.h"


void imprimir_ventana(char mapa[FILAS][COLUMNAS], Jugador *j){
    //se calcula desde donde empieza la pantalla visible
    //se intenta centrar al jugador en la ventana
    int inicio_fila=j->fila - VISTA_FILAS/2;
    int inicio_col=j->col - VISTA_COLS/2;

    //si la ventana se sale por arriba o izquierda se ajusta al borde
    if(inicio_fila<0) inicio_fila=0;
    if(inicio_col<0) inicio_col=0;

    //si la ventana se sale por abajo o derecha lo ajustamos al borde
    if(inicio_fila>FILAS-VISTA_FILAS) inicio_fila = FILAS - VISTA_FILAS;
    if(inicio_col > ANCHO_MAPA-VISTA_COLS) inicio_col= ANCHO_MAPA - VISTA_COLS;


    //se imprime las 20 filas y las 20 columnas visibles
    for(int f=0; f<VISTA_FILAS; f++){
        for(int c=0; c<VISTA_COLS; c++){
            int fila_real= inicio_fila + f;
            int col_real=inicio_col + c;

            // Determinamos qué carácter mostrar en esta celda
            char celda;
            if(fila_real == j->fila && col_real == j->col){
                celda = JUGADOR;
            } else {
                celda = mapa[fila_real][col_real];
            }

            // Imprimimos el carácter con su color correspondiente
            switch(celda){
                case JUGADOR: 
                    printf(COLOR_JUGADOR  "P" COLOR_RESET); 
                    break;
                case PARED:   
                    printf(COLOR_PARED    "#" COLOR_RESET); 
                    break;
                case MONEDA:  
                    printf(COLOR_MONEDA   "M" COLOR_RESET); 
                    break;
                case LLAVE:   
                    printf(COLOR_LLAVE    "K" COLOR_RESET); 
                    break;
                case PUERTA:  
                    printf(COLOR_PUERTA   "T" COLOR_RESET); 
                    break;
                case SALIDA:  
                    printf(COLOR_SALIDA   "E" COLOR_RESET);
                    break;
                case CAMINO:  
                    printf(COLOR_CAMINO   "." COLOR_RESET); 
                    break;
                default:      
                    printf("%c", celda);                    
                    break;
            }
        }
        putchar('\n');
    }
}


void mostrar_hud(Jugador *j, Nivel *n){
    printf("─────────────────────────────────────────\n");
    printf(" Nivel: %d  |  Monedas: %d/%-3d  |  Pasos: %d  |  Llave: %s\n",
        n->numero,
        j->monedas,
        n->total_monedas,
        j->pasos,
        j->tiene_llave ? "Si" : "No"
    );
    printf("─────────────────────────────────────────\n");
}


void mover_jugador(char mapa[FILAS][COLUMNAS], Jugador *j, char tecla, Nivel *n){
    
    int nueva_fila = j->fila;
    int nueva_col = j->col;
    //se calcula la nueva posicion segun la tecla presionada
    if(tecla=='w' || tecla =='W') nueva_fila--;
    else if (tecla == 's' || tecla == 'S') nueva_fila++;
    else if (tecla == 'a' || tecla == 'A') nueva_col--;
    else if (tecla == 'd' || tecla == 'D') nueva_col++;
    else return;

    //se verififca que no se salga del map
    if(nueva_fila < 0 || nueva_fila >= FILAS) return;
    if(nueva_col < 0 || nueva_col >= ANCHO_MAPA) return;

    //funcion 2 nasm: validar_movimiento devuelve 1 si no es pared
    if(!validar_movimiento(&mapa[0][0], COLUMNAS, nueva_fila, nueva_col)){
        return;
    }

    //funcion 4 nasm: detecta si hay puerta y el jugador no tiene lalve
    if(detectar_objeto(&mapa[0][0], COLUMNAS, nueva_fila, nueva_col, PUERTA)){
        if(!j->tiene_llave){
            printf("Necesitas la llave para abrir la puerta!\n");
            return;
        }
        //tiene la llave abre la puerta y la celda se convierte en camino
        mapa[nueva_fila][nueva_col]=CAMINO;
    }

    //funcion 4 nasm: detecta si es moneda
    if(detectar_objeto(&mapa[0][0], COLUMNAS, nueva_fila, nueva_col, MONEDA)){
        j->monedas++;
        mapa[nueva_fila][nueva_col]=CAMINO;//la moneda desaparece
    }

    //funcion 4 nasm detecta llave
    if(detectar_objeto(&mapa[0][0], COLUMNAS, nueva_fila, nueva_col, LLAVE)){
        j->tiene_llave = 1;
        mapa[nueva_fila][nueva_col]=CAMINO;//la llave desaparece
        printf("Recogiste la llave\n");
    }

    //movimiento valido se actualiza posicion y pasos
    j->fila = nueva_fila;
    j->col = nueva_col;
    j->pasos++;
}

void resumen_nivel(Jugador *j, Nivel *n, int puntaje_parcial){
    printf("\n=========================================\n");
    printf(" Nivel %d completado!\n", n->numero);
    printf(" Monedas recolectadas : %d / %d\n", j->monedas, n->total_monedas);
    printf(" Pasos realizados     : %d\n", j->pasos);
    printf(" Celdas libres en mapa: %d\n", n->celdas_libres);
    printf(" Puntaje acumulado    : %d\n", puntaje_parcial);
    printf("=========================================\n");
}

void resumen_final(int monedas_total, int monedas_posibles, int pasos_total, int puntaje){
    printf("\n=========================================\n");
    printf(" Juego completado!\n");
    printf(" Monedas totales      : %d / %d\n", monedas_total, monedas_posibles);
    printf(" Pasos totales        : %d\n", pasos_total);
    printf(" Niveles completados  : 3\n");
    printf(" Puntaje final        : %d\n", puntaje);
    printf("=========================================\n");
}