## Hummingbird OS

A little 32-bit x86 operating system written in C. Currently it can print stuff to the screen with a simple stdio (actually, a stdo) library and a vga library to control text put in the screen. Everything done so far was made to ease debugging.

#### Why are you doing this?

I have always been fascinated with low-level programming and want to learn more about operating systems development and understand a little bit more about the internals of a computer.

#### What can I do with this?

So far, print stuff and make your computer sleep (with nop instructions). It may not do much, but at least it boots **really fast**.

I know it is really simple but I'm proud of it.

#### How do I run this?

You can:
- Install it in your own computer, if you have GRUB2 you can put the ISO into /boot, edit your `/boot/grub/grub.cfg` file adding a menuentry to this OS;
- Use QEMU and emulate a machine (or creating a virtual machine).

You can generate an ISO running
```
mkdir out
make all
```
to install it in a virtual machine, or run
```
mkdir out
make allr
```
to compile it, generate an ISO and run it using QEMU directly.

To compile it and generate an ISO, you'll need:
- GCC toolchain (gcc, as, ld);
- GRUB2;
- xorriso.
