@echo off
nasm -f win64 rutinas.asm -o rutinas.obj
gcc main.c juego.c rutinas.obj -o BitQuest.exe
echo Compilacion terminada. Ejecuta BitQuest.exe