bits 16
org 0x0

cli
mov ax , 0x07c0
mov ds , ax
mov es , ax
xor ax , ax
mov ss , ax
mov sp , 0x8000

mov byte [BOOT_DRIVE] , dl ;récupération de l'unité de boot

push di
    call do_e820
pop di

;load the second boot in memory
mov dh , 1  ;Nombre de secteur à lire
mov cl , 0x01   ;commencer la lecture au cl secteur
mov bx  , 520
mov si , message_boot
call load_sectors_memory


    ;verify if the first space is more than 512*30 bytes before load the kernel
    mov ecx , [memory_useable_list+8]
    cmp ecx , 512*30
    jb bad_space



    ;Load kernel at the first useable space
    mov dh , 30 ;nombre de secteur à lire
    mov cl , 0x2    ;commencer la lecture au cl secteur
    mov bx , [memory_useable_list]
    mov si , message_kernel
    call load_sectors_memory


jmp end

;ES:bx : Memory place
;dh : number of sectors

load_sectors_memory:
    push dx
    mov bp , 3
    load_sectors_memory_loop:
        xor ax , ax
        int 0x13

        mov ah , 0x02
        mov al,dh   ;Dh secteurs à lire.
        mov ch , 0x0    ;cylinder number
        mov dh , 0x00   ;head number
        int 0x13
        jc disk_error
        pop dx
        cmp dh , al ;si le nombre de secteurs 
                    ;lu est égales au nombre de secteur attendu
        jne disk_error


    .load_kernel_end:
        cmp dh , al
        jne disk_error

disk_error:
    call afficher
    dec bp
    cmp bp , 0
    jne load_sectors_memory_loop
    hlt

bad_space:
    mov si , bad_space_message
    call afficher
    hlt
    
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


end:
        jmp end

    %include "BOOT/detect_mem.inc"


BOOT_DRIVE db 0
message_boot db "Bad file format boot",13,10,0
message_kernel db "Bad file format kernel",13,10,0
bad_space_message db "No more space",13,10,0

BASE_KERNEL dd 0

times 510-($-$$) db 0

    db 0x55
    db 0xAA