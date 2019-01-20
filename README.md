## Hummingbird OS

A little x86 OS written mostly in C.

Is not nearly done as I'm only starting to learn OS development.

So far the kernel prints stuff, not optimized... and that's it.

#### Running
I have no idea why you would want to run this, but if you want to, this is what you should do:
```bash
mkdir out
make allr
```

Compiling it requires GNU toolchain (gcc, as, ld). Generating the iso requires Grub2. Running it requires QEMU (or your own computer, at your own risk).
