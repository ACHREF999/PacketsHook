cmd_/home/ubuntu/ldd/ldd.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000 -z noexecstack   --build-id  -T ./scripts/module-common.lds -o /home/ubuntu/ldd/ldd.ko /home/ubuntu/ldd/ldd.o /home/ubuntu/ldd/ldd.mod.o;  true
