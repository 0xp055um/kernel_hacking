cmd_/kernel/src/make_root.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000  --build-id  -T ./scripts/module-common.lds -o /kernel/src/make_root.ko /kernel/src/make_root.o /kernel/src/make_root.mod.o;  true
