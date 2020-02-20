bits 16

cli
mov ax , 0x07c0
mov ds , ax
mov es , ax
xor ax , ax
mov ss , ax
mov sp , 0x8000
mov byte [BOOT_DRIVE] , dl ;récupération de l'unité de b00t
mov dh , 30 ;Nombre dof sectors to read

push di
    call do_e820
pop di


    ;verify if the first space is more than 512*30 bytes before load the kernel
    mov ecx , [memory_useable_list+8]
    cmp ecx , 512*30
    jb bad_space



    ;Load kernel at the firstuseable space
    mov bx , [memory_useable_list]
    call load_kernel


jmp end

mov bp , 3

load_kernel:
    push dx
        xor ax , ax
        int 0x13

        mov ah , 0x02
        mov al,dh   ;Dh secteurs à lire.
        mov ch , 0x0
        mov dh , 0x00
        mov cl , 0x02   ;commencer la lecture au second secteur
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
    mov si , message
    call afficher
    dec bp
    cmp bp , 0
    jne load_kernel
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
    %include "BOOT/a20.inc"


BOOT_DRIVE db 0
message db "Hello , no noooooo",13,10,0
bad_space_message db "Hello , no more space",13,10,0

BASE_KERNEL dd 0

times 510-($-$$) db 0

    db 0x55
    db 0xAA