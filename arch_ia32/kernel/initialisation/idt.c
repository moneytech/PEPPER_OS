#include "idt.h"
#include "../../stdlib/lib.h"
#include "../../stdlib/video.h"
#include "pic.h"
#include "pit.h"

void set_idt(unsigned short selector, unsigned char type, unsigned long offset, unsigned short int vector) {
    IDT[vector].selector = selector;  //Kernelcode segment offset
    IDT[vector].type_attr = type;     //Interrupt gate
    IDT[vector].zero = 0;             //Only zero
    IDT[vector].offset_lowerbits = (offset & 0xFFFF);
    IDT[vector].offset_higherbits = (offset & 0xFFFF0000) >> 16;
}

void init_idt() {
    Init_PIT(PIT_0, 0xDAAD);

    //On itiialise les intérruptions qu'on va utiliser
    PIC_remap(0x20, 0x28);

    set_idt(0x08, INTGATE, (unsigned long)PIT_handler, 0x20);  //IRQ_0
    set_idt(0x08, INTGATE, (unsigned long)irq1, 0x21);
    set_idt(0x08, INTGATE, (unsigned long)irq2, 0x22);
    set_idt(0x08, INTGATE, (unsigned long)irq3, 0x23);
    set_idt(0x08, INTGATE, (unsigned long)irq4, 0x24);
    set_idt(0x08, INTGATE, (unsigned long)irq5, 0x25);
    set_idt(0x08, INTGATE, (unsigned long)irq6, 0x26);
    set_idt(0x08, INTGATE, (unsigned long)irq7, 0x27);
    set_idt(0x08, INTGATE, (unsigned long)irq8, 0x28);
    set_idt(0x08, INTGATE, (unsigned long)irq9, 0x29);
    set_idt(0x08, INTGATE, (unsigned long)irq10, 0x2A);
    set_idt(0x08, INTGATE, (unsigned long)irq11, 0x2B);
    set_idt(0x08, INTGATE, (unsigned long)irq12, 0x2C);
    set_idt(0x08, INTGATE, (unsigned long)irq13, 0x2D);
    set_idt(0x08, INTGATE, (unsigned long)irq14, 0x2E);
    set_idt(0x08, INTGATE, (unsigned long)irq15, 0x2F);

    // set_idt(0x08 , INTGATE , (unsigned long)_general_protection_handler_ , 0xD) ;

    print_address(LOADING_COLOR, (unsigned int)IDTBASE);
    write_string(READY_COLOR, " KERNEL : Chargement de la Table d'interruption ...\n");

    memcpy((char *)IDTBASE, (char *)IDT, sizeof(struct IDT_entry) * IDTSIZE);

    idt_address = (unsigned long)IDTBASE;
    idt_ptr[0] = (sizeof(struct IDT_entry) * IDTSIZE) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16;

    load_idt(idt_ptr);

    write_string(READY_COLOR, "KERNEL  : PIC,PIT initialises\n");
    print_address(ADVICE_COLOR, (IRQ0_frequency));
    write_string(ADVICE_COLOR, " KERNEL : frequence  du PIT ...\n");
}