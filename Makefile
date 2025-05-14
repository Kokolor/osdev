IMAGE := osdev.iso

.PHONY: all
all: $(IMAGE)

.PHONY: kernel
kernel:
	make -C kernel

.PHONY: ramdisk
ramdisk:
	mkdir -p ramdisk_root
	cp apps/hello.elf ramdisk_root
	tar -cf ramdisk.tar -C ramdisk_root .

.PHONY: apps
apps:
	make -C apps

$(IMAGE): kernel apps ramdisk
	rm -rf iso_root
	mkdir -p iso_root/boot/limine/
	cp ramdisk.tar iso_root/
	cp -v kernel/kernel.elf iso_root/boot/
	cp -v limine.conf iso_root/boot/limine/
	mkdir -p iso_root/EFI/BOOT/
	cp -v limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin iso_root/boot/limine/
	cp -v limine/BOOTX64.EFI iso_root/EFI/BOOT/
	cp -v limine/BOOTIA32.EFI iso_root/EFI/BOOT/
	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
		-apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso_root -o $(IMAGE)
	./limine/limine bios-install $(IMAGE)

.PHONY: run
run: $(IMAGE)
	qemu-system-x86_64 -m 512M -enable-kvm -debugcon stdio -smp 2 -cdrom $<

.PHONY: clean
clean:
	rm -rf iso_root $(IMAGE) ramdisk*
	make clean -C kernel
	make clean -C apps