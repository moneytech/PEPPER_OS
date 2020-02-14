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
    mov byte [edi+20] , 1  ;Force a vaalid ACPI

.begin:
    xor ebx , ebx
    mov edx , 0x534D4150
    mov eax , 0xE820
    mov ecx , 24
    int 0x15

    jc .failed
    mov edx ,  0x534D4150
    cmp eax,edx ;voir si eax est different de  0x534D4150
    jne .failed
    test ebx , ebx  ;EBX will be set to some non-zero value
    je .failed
    jmp .verify

.do_E820_loop:
    mov mov eax, 0xe820		; eax, ecx get trashed on every int 0x15 call
	mov [es:di+20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24		; ask for 24 bytes again
	int 0x15
    jc .e820_failed

.verify:
    jcxz .skip_entries  ;Length of "region" (if this value is 0, ignore the entry) 
    cmp cl , 20
    jbe .extension
    cmp byte [es:di+20] , 0
    je .skip_entries

.extension:    ;go to the next buffer if cl equal 24
    push eax
        mov eax , dword [entries]
        inc eax
        mov dword [entries] , eax
    pop eax
    mov ecx , [es:di+8]
    jcxz .skip_entries
    add di ,24

.skip_entries:
    test ebx , ebx  ;if ebx resets to 0, list is complete
    jne .do_E820_loop

.e820_failed:
    clc
    popa
    popf
    ret

.failed:
    stc
    popa
    popf
    ret