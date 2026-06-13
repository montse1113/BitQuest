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
;Validar movimiento
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


;FUNCION 3
;Calcular puntaje
; Parametros:
;   rdi = monedas recolectadas
;   rsi = pasos realizados
;   rdx = niveles completados
;   en rax guardamos resultado

obtener_puntaje:
;formula puntaje =(monedas * 100) - (pasos*2) + (niveles * 500)
    mov rax, rdi
    imul rax, 100

    mov r9, rsi
    imul r9, 2
    sub rax, r9

    mov r9, rdx
    imul r9, 500
    add rax, r9
    ret

;FUNCION 4
;Detectar objeto de una celda
; Parametros:
;   rdi = direccion inicial del mapa
;   rsi = numero de columnas del mapa
;   rdx = fila que se desea revisar
;   rcx = columna que se desea revisar
;   r8 = caracter del objeto que se desea buscar

detectar_objeto:
    imul rdx, rsi
    add rdx, rcx

    movzx rax, byte[rdi + rdx]

    cmp rax, r8
    je .encontrado

    xor rax, rax
    ret

.encontrado:
    mov rax, 1
    ret

;FUNCION 5
;Contar celdas libres
; Parametros:
;   rdi = la direccion inicial del mapa
;   rdi = numeor total de celdas

contar_celdas:
    xor rax,rax
    xor rcx, rcx    ;posicion donde se encuentra

.loop_cl:
    cmp rcx, rsi
    jge .fin_cl

    movzx r9, byte [rdi + rcx]
    cmp r9, '.'         ;verifica si es celda libre 
    jne .siguiente_cl

    inc rax     ;incrementa el contador

.siguiente_cl:
    inc rcx
    jmp .loop_cl

.fin_cl:
    ret



