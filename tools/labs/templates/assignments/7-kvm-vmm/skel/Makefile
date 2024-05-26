CFLAGS = -g -Wall -Wextra -O0 -Iinclude/

.PHONY: run
run: kvm-hello-world
	./kvm-hello-world

kvm-hello-world: vmm.o payload.o
	$(CC) $^ -o $@

payload.o: payload.ld guest_16_bits/guest16.o guest64.img.o
	$(LD) -r -T $< -o $@

guest64.o: guest_32_bits/guest_code.c
	$(CC) $(CFLAGS) -m64 -ffreestanding -fno-pic -c -o $@ $^

guest64.img: guest64.o
	$(LD) -T guest_32_bits/guest.ld $^ -o $@

%.img.o: %.img
	$(LD) -b binary -r $^ -o $@

.PHONY: clean
clean:
	$(RM) kvm-hello-world vmm.o payload.o guest_16_bits/guest16.o \
		guest64.o guest64.img guest64.img.o
