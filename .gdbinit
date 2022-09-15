set auto-load safe-path .
set disassembly-flavor intel
add-symbol-file ./Build/KernelFull.o 0x100000
break Kernel.cpp:KMain
target remote | qemu-system-i386 -hda ./Build/Colonel.bin -S -gdb stdio
