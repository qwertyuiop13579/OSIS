model tiny
.stack 100h
.data 
dir1 db "c:\Tasm\Tasks",0
dir2 db "c:\Tasm",0
n1 db 10
max1 db 255
len1 db ? 
str1 db 255 dup (?),0
max2 db 255
len2 db ? 
str2 db 255 dup (?)
inp1 db "Enter name of the file: $" 
inp2 db "Enter result name: $" 
buffer db 256 dup (?)
handler1 dw ?
handler2 dw ?
.code 
org 100h
start: 

lea dx,dir1       ;change directory
mov ah,3bh
int 21h


LEA DX,inp1  ;input beginng of the file
MOV AH,09h 
INT 21H 
LEA DX,max1 
MOV AH, 0Ah 
INT 21h
MOV Bl,len1
mov str1[BX],'*'
mov str1[BX+1],'.'
mov str1[BX+2],'*'
CALL NEWSTR


LEA DX,inp2    ;input result file
MOV AH,09h 
INT 21H 
LEA DX,max2 
MOV AH, 0Ah 
INT 21h
MOV Bl,len2
mov str2[BX],'.' 
mov str2[BX+1],'t'
mov str2[BX+2],'x'
mov str2[BX+3],'t'
mov str2[BX+4],'0'
CALL NEWSTR


lea dx,str2      ;create new file
mov ah,3ch
xor cx,cx
int 21h
mov  handler2, ax


mov ah,4eh      ;find first file
xor cx,cx
mov dx,offset str1
file_open:
int 21h
;call output
jc no_more_files
mov ah,3Dh    ;only for read
xor al,al
mov dx,80h+1eh
int 21h
jc find_next         
mov bx,ax
mov handler1,ax
mov cx,256
mov dx,offset buffer


read_next:
     mov        ah,3Fh            ; чтение файла
     int     21h
     jc      find_next          ; если ошибка - перейти к следующему если CF установлен
	 sub ax,256
	 cmp ax,0
	 jne not_full
      mov       ah,40h            ; записать в файл
	  mov cx, 256
      mov dx,offset buffer         ; из буфера в DS:DX
	  mov  bx, handler2
      int 21h
	  mov bx, handler1
      jmp      short read_next ; считать следующий байт

	  not_full:
	  add ax,256
	  mov cx,ax
	  mov       ah,40h            ; записать в файл
      mov dx,offset buffer ; из буфера в DS:DX
	  mov  bx, handler2
      int 21h
	  mov bx, handler1


find_next:
      mov      ah,3Eh            ; закрыть предыдущий файл
      int    21h
	  mov       ah,40h            ; записать в файл новую строку
      lea dx,n1    
	  mov  bx, handler2
	  mov cx,1
      int 21h
      mov      ah,4Fh           ; найти следующий файл	 
      mov      dx,80h           ; смещение DTA от начала PSP
      jmp     short file_open
no_more_files:


mov  ah, 3eh             ;close result file
mov  bx, handler2
int  21h


lea dx,dir2       ;change directory
mov ah,3bh
int 21h

MOV AH,4ch 
INT 20H 

OUTPUT PROC
PUSHF
PUSH AX 
PUSH BX 
PUSH CX 
PUSH DX
XOR CX,CX 
MOV BX,10 
label1: 
XOR DX,DX 
DIV BX 
PUSH DX 
INC CX 
XOR DX,DX 
CMP AX,DX 
JA label1 
label2: 
POP AX 
MOV BL,1 
DIV BL 
MOV DL,AL 
ADD DL,'0' 
MOV AH,02h 
INT 21H 
LOOP label2 
CALL NEWSTR 
POP DX 
POP CX 
POP BX 
POP AX 
POPF
RET 
OUTPUT ENDP 


NEWSTR PROC 
PUSH AX 
PUSH BX 
PUSH CX 
PUSH DX 
MOV DL,10 
MOV AH,02h 
INT 21H 
MOV DL,13 
INT 21H 
POP DX 
POP CX 
POP BX 
POP AX 
RET 
NEWSTR ENDP 

DELETESYMB PROC 
PUSH AX 
PUSH BX 
PUSH CX 
PUSH DX 
MOV DL,8 
MOV AH,02h 
INT 21H 
MOV DL,32 
INT 21H 
MOV DL,8 
INT 21H 
POP DX 
POP CX 
POP BX 
POP AX 
RET 
DELETESYMB ENDP 

end start
