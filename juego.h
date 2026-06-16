#ifndef JUEGO_H
#define JUEGO_H

// ─── Tamaño del mapa y ventana visible ───────────────────────────────────────
#define FILAS        60
#define COLUMNAS     61 //ancho del array
#define COLS_MAPA    61//ancho logico del mapa para nasm
#define VISTA_FILAS  20
#define VISTA_COLS   20

// ─── Caracteres del mapa ─────────────────────────────────────────────────────
#define PARED        '#'
#define CAMINO       '.'
#define JUGADOR      'P'
#define MONEDA       'M'
#define LLAVE        'K'
#define PUERTA       'T'
#define SALIDA       'E'

// ─── Struct del jugador ───────────────────────────────────────────────────────
typedef struct {
    int fila;       // posición actual en el mapa
    int col;
    int monedas;    // monedas recolectadas en el nivel actual
    int pasos;      // pasos dados en el nivel actual
    int tiene_llave;// 0 = no tiene, 1 = tiene
} Jugador;

// ─── Struct del estado del nivel ─────────────────────────────────────────────
typedef struct {
    int numero;          // 1, 2 o 3
    int total_monedas;   // calculado por NASM al iniciar el nivel
    int celdas_libres;   // calculado por NASM al iniciar el nivel
} Nivel;

// ─── Prototipos de funciones en C (juego.c) ──────────────────────────────────
void imprimir_ventana(char mapa[FILAS][COLUMNAS], Jugador *j);
void mover_jugador(char mapa[FILAS][COLUMNAS], Jugador *j, char direccion, Nivel *n);
void mostrar_hud(Jugador *j, Nivel *n);
void resumen_nivel(Jugador *j, Nivel *n, int puntaje_parcial);
void resumen_final(int monedas_total, int monedas_posibles, int pasos_total, int puntaje);

// ─── Prototipos de funciones en NASM (rutinas.asm) ───────────────────────────
int  contar_caracteres(char *mapa, int total_celdas, char caracter);
int  validar_movimiento(char *mapa, int columnas, int nueva_fila, int nueva_col);
int obtener_puntaje(int monedas, int pasos, int niveles);
int  detectar_objeto(char *mapa, int columnas, int fila, int col, char objeto);
int  contar_celdas(char *mapa, int total_celdas);//cuenta celdas libres

#endif