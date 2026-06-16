#include <stdio.h>
#include <stdlib.h>
#include <termios.h>    //para leer tecla sin enter en linuxx
#include <unistd.h>
#include "juego.h"



int cargar_mapa(const char *ruta, char mapa[FILAS][COLUMNAS]){
    //fopen abre el archivo en modo lectura "r"
    FILE *archivo=fopen(ruta, "r");

    if(archivo== NULL){
        printf("Error no se puede abrir %s\n", ruta); 
        return 0; //se avisa que fallo y se sale de la funcion
    }
    //buffer temporal grande para leer la linea compleata
    char buffer[128];
    //se recorre las 60 filas del mapa una por una
    for(int f=0; f<FILAS; f++){
        //se lee una linea completa en el buffer temporal
        if(fgets(buffer, sizeof(buffer), archivo)==NULL){
            printf("Error el archvio %s tiene menos de %d filas\n", ruta, FILAS);
            fclose(archivo);
            return 0;
        }

        int len=0;
        while(buffer[len] != '\0') len++;//se avanza hasta el fin del string

        //si el ultimo caracter antes del '\0' es '\n' se remplaza con '\0'
        if(len>0 && mapa[f][len-1] == '\n'){
            buffer[len-1] == '\0';
            len--;
        }

        //se copia el contenido del buffer al mapa, sin pasarse del ancho
        for(int c=0; c<len && c< COLUMNAS-1; c++){
            mapa[f][c] = buffer[c];
        }
        mapa[f][len]='\0';
    }
    //se cierra el archivo al terminar para liberar el recurso
    fclose(archivo);
    return 1;//todo bien
}
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
            if(detectar_objeto(&mapa[0][0], COLUMNAS, nf, nc, SALIDA)){
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

    int puntaje_parcial= obtener_puntaje(*monedas_acum, *pasos_acum,n->numero);
    resumen_nivel(&j, n, puntaje_parcial);
    printf("  Presiona cualquier tecla para continuar...\n");
    leer_tecla();

    return j.monedas;
}


//main
int main(){
    char mapa1[FILAS][COLUMNAS];
    char mapa2[FILAS][COLUMNAS];
    char mapa3[FILAS][COLUMNAS];

    if(!cargar_mapa("nivel1.txt", mapa1)){return 1;}
    if(!cargar_mapa("nivel2.txt", mapa2)){return 1;}
    if(!cargar_mapa("nivel3.txt", mapa3)){return 1;}
    system("clear");


    //pantalla de titulo
    printf("=========================================\n");
    printf("   BITQUEST: Explorador de Matrices\n");
    printf("=========================================\n");
    printf("  W/A/S/D  Mover jugador\n");
    printf("  Q        Salir\n");
    printf("  M        Moneda\n");
    printf("  K        Llave\n");
    printf("  T        Puerta (necesitas llave)\n");
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
    jugar_nivel(mapa1, &n1, &pasos_total, &monedas_total);
    monedas_posibles += n1.total_monedas;

    //nivel 2
    Nivel n2={2,0,0};
    jugar_nivel(mapa2, &n2, &pasos_total, &monedas_total);
    monedas_posibles += n2.total_monedas;

    //nivel 3
    Nivel n3= {3,0,0};
    jugar_nivel(mapa3, &n3, &pasos_total, &monedas_total);
    monedas_posibles += n3.total_monedas;


    //funcion 3 nasm puntaje total
    int puntaje = obtener_puntaje(monedas_total, pasos_total, 3);

    //resumen final
    resumen_final(monedas_total, monedas_posibles, pasos_total, puntaje);

    return 0;
}
