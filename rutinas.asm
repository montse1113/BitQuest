;FUNCION 1: CONTAR CARACTERES
; Parametros:
; Se ejecuta en linux
;   rdi = direccion inicial del mapa
;   rsi = numero totoal de celdas
;   rdx = caracter que se desea contar
;   rax = cantidad de veces que aparece el caracter


contar_caracteres:
    xor rax, rax    ; contador = 0
    xor rcx, rcx    ; i = 0

.loop_cc:
    cmp rcx, rsi
    jge .fin_cc
    
    movzx r9, byte[rdi + rcx]  ;r9 = mapa[i]
    cmp r9, rdx
    jne .siguiente_cc

    inc rax;

.siguiente_cc:
    inc rcx;
    jmp .loop_cc

.fin_cc:
    ret

;FUNCION 2
; Parametros:
;   rdi = direccion inicial del mapa
;   rsi = numero de columnas del mapa
;   rdx = nueva fila propuesta para el jugador
;   rcx = nueva columna propuesta para el jugador

validar_movimiento:
    imul rdx, rsi;
    add rdx, rcx

    movzx rax, byte[rdi +rdx]  ;tomamos el caracter del mapa que esta en la posicion rdx

    cmp rax, '#'
    je .bloqueado

    mov rax, 1
    ret

.bloqueado:
    xor rax, rax    ;retorna 0
    ret
