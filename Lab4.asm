.model small
.stack 100h
.data
	old1 dd ?
.code
.486		
new_handle proc    			
	push ds si es di dx cx bx ax           ; в стек регистры       
	xor ax, ax 
	in  al, 60h              ;Получаем скан-код символа	
	cmp al, 39h       ; ' '
	jne old_handler
	xor ax, ax
	mov al, 20h
	out 20h, al 
	pop ax bx cx dx di es si ds;      восстановление регистров перед выходом из нашего обработчика прерываний
	iret
old_handler: 
	pop ax bx cx dx di es si ds                       
	jmp dword ptr cs:old1              ;вызов стандартного обработчика прерывания               
new_handle endp
 
start:
	mov ax, @data
	mov ds, ax
        
	cli           ;сброс флага IF
	pushf 
	push 0        ;перебрасываем значение 0 в DS
	pop ds
	mov eax, ds:[09h*4] 
	mov cs:[old1], eax ;сохранение системного обработчика

	mov ax, cs
	shl eax, 16
	mov ax, offset new_handle
	mov ds:[09h*4], eax      ;запись нового обработчика
	sti                      ;Установка флага IF
    
	xor ax, ax
	mov ah, 31h
	MOV DX, (start - @code + 10FH) / 16 ;вычисление размера резидентной части в параграфах(16 байт)
	INT 21H 

	end start
