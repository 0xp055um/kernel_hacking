cmd_/kernel/src/hello_dev_char.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000  --build-id  -T ./scripts/module-common.lds -o /kernel/src/hello_dev_char.ko /kernel/src/hello_dev_char.o /kernel/src/hello_dev_char.mod.o;  true
