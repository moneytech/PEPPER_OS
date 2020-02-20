CC=gcc
CFLAGS := -m32 -g -ffreestanding -fno-PIC -fno-stack-protector -Wstringop-overflow -Werror
LDFLAGS := ld -m elf_i386 --oformat=binary -Tlinker.ld


KERNEL_INIT = kernel/initialisation
KERNEL_MEM = kernel/memory_management

OBJ := $(KERNEL_INIT)/pit.o $(KERNEL_INIT)/pit_asm.o $(KERNEL_INIT)/pic.o $(KERNEL_INIT)/idt.o $(KERNEL_INIT)/gdt_idt_asm.o $(KERNEL_INIT)/gdt.o
OBJ += $(KERNEL_INIT)/exceptions/exception_asm.o $(KERNEL_INIT)/exceptions/exception.o
OBJ += stdlib/video.o stdlib/lib.o 
OBJ += $(KERNEL_INIT)/interruptions/interrupt.o $(KERNEL_INIT)/interruptions/interrupt_asm.o

OBJ += $(KERNEL_MEM)/paging/paging_asm.o $(KERNEL_MEM)/paging/paging.o

all :
	make mixt
	make boot
	make disk
	make launch

debug_file:kernel.o $(OBJ)
	ld -m elf_i386 --oformat=elf32-i386 -Tlinker.ld kernel.o $(OBJ) -o kernel.elf


mixt : kernel.o $(OBJ)
	clear
	$(LDFLAGS) kernel.o $(OBJ) -o kernel.bin
	
%.o : %.c
		@$(CC) -o $@ -c $< $(CFLAGS)

%.o:%.asm		
		nasm -felf32 $< -o $@

boot:
	nasm BOOT/bootloader.asm -f bin -o BOOT/boot.bin
	# nasm -g -f elf32 -F dwarf -o BOOT/boot.o BOOT/bootloader.asm
	# ld -melf_i386 -Ttext=0x7c00 -nostdlib --nmagic -o BOOT/boot.elf BOOT/boot.o
	# objcopy -O binary BOOT/boot.elf BOOT/boot.bin


disk :
	dd if=/dev/zero of=disk.img bs=512 count=2880
	dd if=BOOT/boot.bin of=disk.img bs=512 conv=notrunc
	# dd if=kernel.bin of=disk.img bs=512 seek=1 conv=notrunc

launch:
	qemu-system-i386 -fda disk.img -d cpu_reset -d int -serial file:serial.log

debug :
	make debug_file
	qemu-system-i386 -fda disk.img -S -s &
	gdb kernel.elf  \
        -ex 'target remote localhost:1234' \
        -ex 'layout src' \
        -ex 'layout reg' \
        -ex 'break main' \
-ex 'continue' 

%.o : %.c %.asm
	@$(CC) -o $@ -c $< $(CFLAGS)

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