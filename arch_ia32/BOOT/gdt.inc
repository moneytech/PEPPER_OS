gdt_start:
gdt_null:
  dq 0
 
  gdt_code:
  dw 0FFFFh
  dw 0
 
  db 0
  db 10011010b
  db 11001111b
  db 0
 
  gdt_data:
  dw 0FFFFh
  dw 0
 
  db 0
  db 10010010b
  db 11001111b
  db 0
end_of_gdt:

gdtr:
    dw end_of_gdt - gdt_start - 1   ; limit (Size of GDT)
    dd gdt_start        ; base of GDT

    CODE_SEG equ gdt_code - gdt_start
    DATA_SEG equ gdt_data - gdt_start