<h1 align="center">mOS32 (MyOS32) a better version of MyOS</h1>
<h3>Info</h3>
Written by JM-Pilot and Licensed under MIT License

A kernel in x86_32 BIOS Using GRUB as its bootloader, its goal is to make old PCs
work again


<h3>Compiling and Running</h3>
Before you run make sure you have these tools installed

```
grub
xorriso
qemu (optional or other vms)
i686-elf tools (compiled from source)
binutils (not i686-elf-binutils)
```

After getting these tools you should be ready to compile

```
make clean && make
```

or run after compiling

```
make clean && make run
```

<h3>Contributing</h3>
> [Guide is here](/docs/CONTRIBUTING.md)