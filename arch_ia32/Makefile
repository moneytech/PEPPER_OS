CC=gcc
CFLAGS := -m32 -g -ffreestanding -fno-PIC -fno-stack-protector -Werror
LDFLAGS := ld -m elf_i386 --oformat=binary -Tlinker.ld


LINKER := kernel/initialisation/init.a



all :
	make mixt
	make disk
	make launch

debug_file:kernel.o
	ld -m elf_i386 --oformat=elf32-i386 -Tlinker.ld $^ $(LINKER) -o kernel.elf


mixt : kernel.o
	clear
	$(LDFLAGS) $^ $(LINKER) -o kernel.bin

disk :
	dd if=/dev/zero of=disk.img bs=512 count=2880
	dd if=BOOT/boot.bin of=disk.img bs=512 conv=notrunc
	dd if=kernel.bin of=disk.img bs=512 seek=1 conv=notrunc

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