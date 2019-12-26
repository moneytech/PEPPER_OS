#define __GDT__
#include "gdt.h"
#include "../../stdlib/lib.h"
#include "../../stdlib/video.h"
#include "../../stdlib/i386types.h"

/*
 * init_desc initialise un descripteur de segment situe en gdt ou en ldt.
 * desc est l'adresse lineaire du descripteur a initialiser.
 */

void init_gdt_desc(u32 base, u32 limite, u8 acces, u8 other,
		   struct gdtdesc *desc)
{
	desc->lim0_15 = (limite & 0xffff);
	desc->base0_15 = (base & 0xffff);
	desc->base16_23 = (base & 0xff0000) >> 16;
	desc->acces = acces;
	desc->lim16_19 = (limite & 0xf0000) >> 16;
	desc->other = (other & 0xf);
	desc->base24_31 = (base & 0xff000000) >> 24;
	EncodeGDTEntry(desc ,(u8*)desc) ;
}

/**
 * \param target A pointer to the 8-byte GDT entry
 * \param source An arbitrary structure describing the GDT entry
 */

void EncodeGDTEntry(struct gdtdesc *source , u8* target)
{
	//Verifier la limite pour pouvoir l'encoder

	unsigned int limit , base ;

	limit = (*source).lim16_19 ;
	limit <<= 16 ;
	limit |= (*source).lim0_15 ;

	base = (*source).base24_31 ;
	base <<= 8 ;
	base |= (*source).base16_23 ;
	base <<= 16 ;
	base |= (*source).base0_15 ;

	unsigned char type ;

	type = (*source).acces ;

	// Check the limit to make sure that it can be encoded
    if ((limit > 65536) && ((limit & 0xFFF) != 0xFFF))
        write_string(ERROR_COLOR , "Reconfiguration des tailles des segments\n");
    

	if (limit > 65536) {
        // Adjust granularity if required
		write_string(ADVICE_COLOR , "Ajustement des segments memoire\n");
        limit = limit >> 12;
        target[6] = 0xC0;
    }
	else
        target[6] = 0x40;
    

	// Encode the limit
    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0xF;

	// Encode the base 
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

	// And... Type
    target[5] = type;
}
/*
 * Cette fonction initialise la GDT apres que le kernel soit charge 
 * en memoire. Une GDT est deja operationnelle, mais c'est celle qui
 * a ete initialisee par le secteur de boot et qui ne correspond
 * pas forcement a celle que l'on souhaite.
 */
void init_gdt(void){
	print_address(LOADING_COLOR , GDTBASE) ;
	write_string(READY_COLOR , " KERNEL : Configuration des segments\n") ;
	
	/* initialisation des descripteurs de segment */
	init_gdt_desc(0x0, 0x0, 0x0, 0x0, &kgdt[0]);
	//Configuration code, noyau, pile système
	init_gdt_desc(0x0, 0xFFFFF, SEG_CODE_E_R_A | SEG_DESCTYPE (1) | SEG_PRIV (0) |	\
						SEG_PRES (1), 0x0D, &kgdt[1]);	/* code */

	init_gdt_desc(0x0, 0xFFFFF, SEG_DATA_R_W_A |	\
		SEG_DESCTYPE (1) | SEG_PRIV (0) | SEG_PRES (1), 0x0D, &kgdt[2]);	/* data */


	init_gdt_desc(0x0, 0x0,SEG_DATA_R_W_EX_A |	\
		SEG_DESCTYPE(1) | SEG_PRIV(0) | SEG_PRES(1), 0x0D, &kgdt[3]);

	EncodeGDTEntry( (kgdt+4) , (u8*)(kgdt+4) ) ;
	EncodeGDTEntry( (kgdt+8) , (u8*)(kgdt+8) ) ;

	memcpy((char*)GDTBASE , (char*)kgdt , sizeof(struct gdtdesc)*GDTSIZE);

	/* initialisation de la structure pour GDTR */
	unsigned long gdt_adress = (unsigned long)GDTBASE ;
	gdt_ptr[0] = (sizeof(struct gdtdesc)*GDTSIZE) + ((gdt_adress & 0xFFFF) << 16);
	gdt_ptr[1] = gdt_adress >> 16 ;

	/* chargement du registre GDTR */
	load_gdt(gdt_ptr) ;


	/* initialisation des segments */
	asm("   movw $0x10, %ax	\n \
            movw %ax, %ds	\n \
            movw %ax, %es	\n \
            movw %ax, %fs	\n \
            movw %ax, %gs	\n \
            ljmp $0x08, $next	\n \
            next:		\n");

}