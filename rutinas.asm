bits 64
section .text

global contar_caracteres
global validar_movimiento
global obtener_puntaje
global detectar_objeto
global contar_celdas


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
    cmp rcx, rsi    ; si i >= total celdas ya terminamos
    jge .fin_cc
    
    movzx r9, byte[rdi + rcx]  ;r9 = mapa[i], leemos un byte del mapa
    cmp r9, rdx                ; verificamos si es el caracter que buscamos
    jne .siguiente_cc          ;no coincide, nos saltamos el inc

    inc rax;        ; si coincide, sumamos 1 al contador

.siguiente_cc:
    inc rcx;        ; i++, siguiente celda
    jmp .loop_cc

.fin_cc:
    ret             ; rax tiene el total


;FUNCION 2
;Validar movimiento
; Parametros:
;   rdi = direccion inicial del mapa
;   rsi = numero de columnas del mapa
;   rdx = nueva fila propuesta para el jugador
;   rcx = nueva columna propuesta para el jugador

validar_movimiento:
    imul rdx, rsi   ; fila * columnas, para encontrar en que fila estamos en memoria
    add rdx, rcx    ; + col, indice final = mapa[fila][col]

    movzx rax, byte[rdi + rdx]  ;tomamos el caracter del mapa que esta en la posicion rdx

    cmp rax, '#'    ; coincide con el caracter que representa la pared
    je .bloqueado   ; si es pared no puede pasar

    mov rax, 1      ; no es pared, movimiento valido
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
    imul rax, 100   ; monedas * 100

    mov r9, rsi
    imul r9, 2      ; pasos * 2
    sub rax, r9     ; le restamos los pasos al puntaje, mas pasos = menos puntos

    mov r9, rdx
    imul r9, 500    ; niveles * 500
    add rax, r9     ; completar niveles da bastantes puntos
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
    imul rdx, rsi   ; misma formula que validar_movimiento
    add rdx, rcx    ; indice = fila * columnas + col

    movzx rax, byte[rdi + rdx]  ; leemos la celda

    cmp rax, r8     ; es el objeto que buscamos
    je .encontrado  ; si coincide retornamos 1

    xor rax, rax    ; no esta el objeto, retornamos 0
    ret

.encontrado:
    mov rax, 1      ; si esta el objeto
    ret


;FUNCION 5
;Contar celdas libres
; Parametros:
;   rdi = la direccion inicial del mapa
;   rsi = numero total de celdas

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