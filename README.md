# BitQuest: Explorador de Matrices

Videojuego de consola desarrollado con programación híbrida en **C y NASM de 64 bits**.
El jugador recorre un laberinto representado por una matriz, recolecta monedas, encuentra
una llave, abre una puerta y llega a la salida para avanzar de nivel.

> **Importante:** Este proyecto fue desarrollado y probado en **Linux**.
> Las rutinas en ensamblador usan la convención de llamadas **System V AMD64** (Linux),
> por lo que la compilación está pensada para ese entorno.

---

## Requisitos

- Sistema operativo Linux
- **NASM** (ensamblador de 64 bits)
- **GCC** (compilador de C)

Para instalarlos en Ubuntu:

```bash
sudo apt update
sudo apt install nasm gcc
```

---

## Estructura del proyecto

```
BitQuest/
├── main.c        Función principal, menú y control del juego
├── juego.c       Lógica del juego: ventana visible, movimiento, resúmenes
├── juego.h       Prototipos y constantes generales
├── rutinas.asm   Las 5 funciones obligatorias en NASM
├── nivel1.txt    Mapa del nivel 1
├── nivel2.txt    Mapa del nivel 2
├── nivel3.txt    Mapa del nivel 3
└── README.md     Este archivo
```

> Los mapas se cargan desde archivos `.txt` externos, por lo que deben estar
> en la misma carpeta donde se ejecuta el juego.

---

## Compilación

Desde la carpeta del proyecto, ejecuta los siguientes comandos en la terminal:

```bash
nasm -f elf64 rutinas.asm -o rutinas.o
gcc main.c juego.c rutinas.o -o BitQuest -no-pie
```

- El primer comando ensambla `rutinas.asm` al formato objeto de Linux (`elf64`).
- El segundo compila los archivos en C, los enlaza con el objeto de NASM y genera
  el ejecutable `BitQuest`. La opción `-no-pie` es necesaria para enlazar
  correctamente con el código en ensamblador.

---

## Ejecución

Una vez compilado, ejecuta el juego con:

```bash
./BitQuest
```

---

## Controles

| Tecla | Acción              |
|-------|---------------------|
| W     | Mover hacia arriba  |
| A     | Mover a la izquierda|
| S     | Mover hacia abajo   |
| D     | Mover a la derecha  |
| Q     | Salir del juego     |

---

## Elementos del mapa

| Símbolo | Significado                              |
|---------|------------------------------------------|
| `#`     | Pared (no se puede atravesar)            |
| `.`     | Camino libre                             |
| `P`     | Jugador                                  |
| `M`     | Moneda (se puede recolectar)             |
| `K`     | Llave (abre la puerta)                   |
| `T`     | Puerta (solo se cruza con la llave)      |
| `E`     | Salida del nivel                         |