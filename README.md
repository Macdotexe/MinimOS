# MinimOS
An extremely basic operating system built for floppy disks.

### ⚠️ WARNING ⚠️: IT IS HEAVILY RECOMMENDED TO USE THIS OS IN AN EMULATOR AND NOT ON REAL HARDWARE

<br>

# Build

## 1. Install dependencies
### On Debian and Ubuntu:
Run:
```
sudo apt install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo nasm mtools qemu-system-x86
```

### On other Linux distros:
install the following packages with
your package manager:
```
build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo nasm mtools qemu-system-x86`
```

### On Mac OS:
You can find most of these packages on
[Homebrew](https://brew.sh)

### On Windows:
The easiest way to do the build proccess on Windows is by using WSL,
this project recommends using Ubuntu or Debian. to install Ubuntu WSL run:
```
wsl --install -d Ubuntu
```

If you prefer a diffrent distro, run this to list all installable distros:
```
wsl --list --online
```

Next just follow the instructions on the Linux sections.

## 2. Installing toolchain
To set up the cross-compiler, just run this:
```
make toolchain
```
#### Warning: This takes a long time and is very resource expensive.

## 3. Building floppy image
After installing the dependencies and the toolchain,
you shoulden't run them again. After making your
changes, all you need to do is to build the floppy image.
This is pretty simple as well, just run:
```
make
```
This will build the floppy image to `build/main_floppy.img`.
Next you can run it as specified below or (**NOT RECOMMENDED**)
flash it onto a real floppy and test it on real hardware.

# Running and testing

After building the OS using the commands specifed above, you
can run this to execute it with QEMU:
```
./run.sh
```
Or if you prefer a diffrent virtualization software, 
(VirtualBox, VMWare, etc...) run it with that.
When using diffrent sotware, make sure to set
`build/mail_floppy.img` as a 1.44k floppy disk.
<br>
<br>
Remember to build the OS before running or in this
case testing. After built you can debug the OS using Bochs
with the following command:
```
./debug.sh
```

