;FUNCION 1: CONTAR CARACTERES
; Parametros:
; //Se ejecuta en linux
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