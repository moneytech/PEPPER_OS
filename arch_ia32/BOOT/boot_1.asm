%define BASE 0X07c0
%define KSIZE 1

bits 16

jmp start

afficher:
    push ax
    push bx
.debut:
    lodsb
    ; ds:si -> al
    cmp al, 0
    ; fin chaîne ?
    jz .fin
    mov ah, 0x0E ; appel au service 0x0e, int 0x10 du bios
    mov bx, 0x07 ; bx -> attribut, al -> caractère ASCII
    int 0x10
    jmp .debut
.fin:
    pop bx
    pop ax
    ret




start:
    cli
    mov ax , 0x07c0
    mov ds , ax
    mov es , ax
    mov ss , ax
    mov sp , 0xf000

    mov [bootdrv] , dl

    mov si , msg
    call afficher

    xor ax , ax
    int 13h

    push es
    mov ax , [BASE]
    mov es , ax
    mov bx , 512
    mov ah , 0x2
    mov al , [KSIZE]
    mov ch , 0
    mov cl , 2
    mov dh , 0
    mov dl , [bootdrv]
    int 13h
    jc erreur
    pop es
    cmp dh , al
    jne erreur

    jmp dword BASE:512

erreur:
    mov si , msg_erreur
    call afficher
    hlt

msg: db "chargement",13,10,0
msg_erreur: db "erreur de disque",13,10,0
bootdrv: db 0

times 510-($-$$) db 144
dw 0xAA55