CC=gcc
CFLAGS := -m32 -g -ffreestanding -fno-PIC -fno-stack-protector -Werror


OBJ := pit.o pit_asm.o pic.o idt.o idt_asm.o gdt.o
OBJ += interruptions/interrupt.o interruptions/interrupt_asm.o
OBJ += exceptions/exception_asm.o exceptions/exception.o
OBJ += ../../stdlib/video.o ../../stdlib/lib.o 

MIXT = mixt

all : $(MIXT)

mixt : $(OBJ)
	ar cr init.a $^

%.o : %.c %.asm
	@$(CC) -o $@ -c $< $(CFLAGS)
	nasm -felf32 $< -o $@ 

.PHONY : clean mrproper

clean : 
	rm -rf *.o

mrproper :
	rm -rf $(MIXT)

	# $@ Le nom de la cible
	# $< le nom de la première dépendance
	# $^ la liste des dépendances
	# $? La liste des dépendances plus récente que la cible
	# $* Le nom du fichier sans suffixe