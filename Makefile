GCCPARAMS = -m32 -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore -fno-stack-protector -O2
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = out/loader.o out/kernel.o out/stdio.o out/vga.o

kernel: src/kernel.c
	gcc $(GCCPARAMS) -o out/$@.o -c $<

vga: src/vga/vga.c src/vga/vga.h
	gcc $(GCCPARAMS) -o out/$@.o -c $<

stdio: src/stdio/stdio.c src/stdio/stdio.h
	gcc $(GCCPARAMS) -o out/$@.o -c $<

loader: src/loader.s
	as $(ASPARAMS) -o out/$@.o $<

hummingbird: src/linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o out/$@.bin $(objects)

iso: out/hummingbird.bin
		mkdir iso
		mkdir iso/boot
		mkdir iso/boot/grub
		cp $< iso/boot
		echo 'set timeout=5' > iso/boot/grub/grub.cfg
		echo 'set default=0' >> iso/boot/grub/grub.cfg
		echo '' >> iso/boot/grub/grub.cfg
		echo 'menuentry "Hummingbird OS" {' >> iso/boot/grub/grub.cfg
		echo '    multiboot /boot/hummingbird.bin' >> iso/boot/grub/grub.cfg
		echo '    boot' >> iso/boot/grub/grub.cfg
		echo '}' >> iso/boot/grub/grub.cfg
		grub-mkrescue --output=out/hummingbird.iso iso
		rm -rf iso

all:
	make kernel
	make stdio
	make vga
	make loader
	make hummingbird
	make iso

allr:
	make all
	./run

clean:
	rm -rf out/*
