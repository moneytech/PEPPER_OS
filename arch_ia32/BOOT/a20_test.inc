;Test if A20 is already enabled - if it is you don't need to do anything at all
;Try the BIOS function. Ignore the returned status.
;Test if A20 is enabled (to see if the BIOS function actually worked or not)
;Try the keyboard controller method.
;Test if A20 is enabled in a loop with a time-out (as the keyboard controller method may work slowly)
;Try the Fast A20 method last
;Test if A20 is enabled in a loop with a time-out (as the fast A20 method may work slowly)
;If none of the above worked, give up 


enabling_A20:
;Test if A20 is already enabled - if it is you don't need to do anything at all
call  _check_a20_
cmp ax , 1
je  _enabled_

;Try the BIOS function. Ignore the returned status.
call _a20_BIOS_
cmp ax , 0
jne  _enabled_

;Test if A20 is enabled (to see if the BIOS function actually worked or not)


ret


_enabled_:
ret

_check_a20_:
pushf
push ds
push es
push di
push si

cli

xor ax , ax
mov es,ax
not ax
mov ds , ax

mov di , 0x7DFE     ;es:di->0000:7c00
mov si , 0x7E0E     ;ds:si->FFFF:7E0E

mov ecx , [es:di]
cmp ecx , [ds:si]

mov ax, 0

je  check_a20__exit  ;if it is different ; then A20 is already enable

mov ax , 1

check_a20__exit:
pop si
pop di
pop es
pop ds
popf

ret

_a20_BIOS_:
mov ax , 2403h  ;--- A20-Gate Support ---
int 15h
jb  _a20_not_supported_  ;INT 15h is not supported
cmp ah , 0
jnz  _a20_not_supported_ ;INT 15h is not supported

mov cx , 3
mov ax , 2402h  ;--- A20-Gate Status ---
int 15h
jb  _a20_failed_ ;couldn't get status
cmp ah , 0
jnz  _a20_failed_    ;couldn't get status

cmp al , 1
jz  _a20_activated_  ;A20 is already activated

mov ax , 2401h  ;--- A20-Gate Activate ---
int 15h
jb  _a20_failed_ ;couldn't activate the gate
cmp ah , 0
jnz  _a20_failed_    ;couldn't activate the gate

_a20_activated_:
mov ax , 1
ret

_a20_failed_:
dec cx
cmp cx , 0
jne  _a20_BIOS_
mov ax , 0
ret

_a20_not_supported_:
mov ax , 0
ret