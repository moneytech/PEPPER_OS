
;La meilleur façon de détecté la mémoire est d'utiliser la commande INT 0x15 EAX = 0xEB820
;C'est la seule fonction BIOS capable de détecter les zones mémoires supérieures à 4G

;Cette fonction renvoie une liste non triée qui peut contenir qui peut contenir des entrées inutilisées
;Chaque entrée est stockée en mémoir dans ES:DI

entries equ 0
buffer equ 1

do_E820:
    pushf
    pusha
    mov edi , buffer ;destination buffer 

.begin:
    xor ebx , ebx
    mov edx , 0x534D4150
    mov eax , 0xE820
    mov ecx , 24
    int 15

    jc .failed
    mov edx ,  0x534D4150
    cmp eax,edx ;voir si eax est different de  0x534D4150
    jne .failed
    test ebx , ebx  ;EBX will be set to some non-zero value
    je .failed
    test ecx , ecx
    jne .failed
    jmp .success

.success:
    push eax
        mov eax , dword[entries]
        inc eax
        mov dword[entries] , eax
    pop eax
    add edi , ecx
    
    stc
    jmp .begin
    

.failed:
    popa
    popf
    ret