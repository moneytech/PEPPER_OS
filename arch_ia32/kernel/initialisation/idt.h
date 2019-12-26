#ifndef IDT_H


#define IDTSIZE 0xFF /* nombre max. de descripteurs dans la table */

#define INTGATE 0x8E /* utilise pour gerer les interruptions */
#define IDTBASE 0x800

unsigned long idt_address;
unsigned long idt_ptr[2];

extern int load_idt(unsigned long int *idtr);

extern /* use IRQ 0 to accurately keep track of
    real time in milliseconds since the PIT was configured .
        cette interruption permettra d'eviter certaines incohérences
        pour la définitionsd e la fréquence
    */
    unsigned long PIT_handler() ,
    _general_protection_handler_();

//Descripteur de segment:
struct IDT_entry {
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
} __attribute__((packed));

void init_idt(void);

struct IDT_entry IDT[IDTSIZE];

void set_idt(unsigned short selector, unsigned char type, unsigned long offset, unsigned short int int_);

        extern int irq0();
        extern int irq1();
        extern int irq2();
        extern int irq3();
        extern int irq4();
        extern int irq5();
        extern int irq6();
        extern int irq7();
        extern int irq8();
        extern int irq9();
        extern int irq10();
        extern int irq11();
        extern int irq12();
        extern int irq13();
        extern int irq14();
        extern int irq15();


#endif  // !IDT_H