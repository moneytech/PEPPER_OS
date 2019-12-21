
global load_idt 

section .text

load_idt:
	cli
		mov edx, [esp + 4]
		lidt [edx]
		
	ret
