bits 16


push di
    call do_e820
pop di

mov si , bad_space_message
call afficher



    ;verify if the first space is more than 512*30 bytes before load the kernel
    mov ecx , [memory_useable_list+8]
    cmp ecx , 512*30
    jb bad_space
    ;Load kernel at the first useable space
    cli
    mov dh , 30 ;nombre de secteur à lire
    mov cl , 0x2    ;commencer la lecture au cl secteur
    mov bx , [memory_useable_list]
    call load_sectors_memory
    sti

    jmp end


    %include "BOOT/detect_mem.inc"
    %include "BOOT/gdt_test.inc"

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

    bad_space:
    mov si , bad_space_message
    call afficher
    hlt

    bad_space_message db "No more space",13,10,0


end: jmp end


times 512-($-$$) db 144

