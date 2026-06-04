
# mOS32 (MyOS32) a better version of MyOS

## Info

Written by JM-Pilot and Licensed under MIT License

A kernel in x86_32 BIOS Using GRUB as its bootloader, its goal is to make old PCs
work again

### Compiling and Running

Before you run make sure you have these tools installed

```sh
grub
xorriso
qemu (optional or other vms)
i686-elf tools (compiled from source)
binutils (not i686-elf-binutils)
```

After getting these tools you should be ready to compile. Be warned though we
ask for sudo permission, just type your password... nothing will not happen to
your system

```sh
make clean && make
```

or run after compiling

```sh
make clean && make run
```

### Contributing

The guide to contributing
[is here](/docs/CONTRIBUTING.md)
