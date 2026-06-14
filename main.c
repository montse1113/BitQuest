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
    nuevo.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL);
    nuevo.c_cc[VMIN] = 1;
    nuevo.c_cc[VTIME] = 0;
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
        system("clear");    //se limpia pantalla
        imprimir_ventana(mapa, &j);
        mostrar_hud(&j, n);

        char tecla = leer_tecla();

        if(tecla == 'q' || tecla == 'Q'){
            printf("\nSaliste del juego!\n");
            exit(0);
        }

        //funcion 4 nasm se detecta si el jugador llego a la salida
        int nf= j.fila, nc = j.col;
        if(tecla == 'w' || tecla == 'W') nf--;
        if(tecla == 's' || tecla == 'S') nf++;
        if(tecla == 'a' || tecla == 'A') nc--;
        if(tecla == 'd' || tecla == 'D') nc++;

        if(nf>0 && nf<FILAS && nc >= 0 && nc<COLUMNAS){
            if(detectar_objeto(&mapa[0][0], COLS_MAPA, nf, nc, SALIDA)){
                j.fila = nf;
                j.col = nc;
                break; //nivel completado
            }
        }
        mover_jugador(mapa, &j, tecla, n);
    }
   
    //se acumulan pasos y monedas para el resumen final 
    *pasos_acum += j.pasos;
    *monedas_acum += j.monedas;

    resumen_nivel(&j, n);
    printf("  Presiona cualquier tecla para continuar...\n");
    leer_tecla();

    return j.monedas;
}


//main
int main(){
    system("clear");


    //pantalla de titulo
    printf("=========================================\n");
    printf("   BITQUEST: Explorador de Matrices\n");
    printf("=========================================\n");
    printf("  W/A/S/D  Mover jugador\n");
    printf("  Q        Salir\n");
    printf("  M        Moneda\n");
    printf("  K        Llave\n");
    printf("  D        Puerta (necesitas llave)\n");
    printf("  E        Salida del nivel\n");
    printf("=========================================\n");
    printf("  Presiona cualquier tecla para iniciar...\n");
    leer_tecla();

    //variables para el resumen final
    int pasos_total=0;
    int monedas_total=0;
    int monedas_posibles=0;

    //nivel 1
    Nivel n1={1,0,0};
    jugar_nivel(mapa_nivel1, &n1, &pasos_total, &monedas_total);
    monedas_posibles += n1.total_monedas;

    //nivel 2
    Nivel n2={2,0,0};
    jugar_nivel(mapa_nivel2, &n2, &pasos_total, &monedas_total);
    monedas_posibles += n2.total_monedas;

    //nivel 3
    Nivel n3= {3,0,0};
    jugar_nivel(mapa_nivel3, &n3, &pasos_total, &monedas_total);
    monedas_posibles += n3.total_monedas;


    //funcion 3 nasm puntaje total
    int puntaje = obtener_puntaje(monedas_total, pasos_total, 3);

    //resumen final
    resumen_final(monedas_total, monedas_posibles, pasos_total, puntaje);

    return 0;
}
