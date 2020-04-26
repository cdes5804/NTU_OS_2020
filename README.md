# OS Spring 2020, Project1

## environment
- Ubuntu 16.04 server
    - [Download](http://releases.ubuntu.com/16.04/)
- Kernel version: 4.14.25 x86_64
    - [Download](https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.14.25.tar.xz)

## Kernel compilation

We need to add some system calls define in this project.

The necessay files are placed in the ``kernel_files`` directory. Use these files to replace the corresponding original kernel files.

For adding system calls and kernel compilation, please follow the instructions in the following websites:

- [How to build and install the latest Linux kernel from source](https://medium.com/free-code-camp/building-and-installing-the-latest-linux-kernel-from-source-6d8df5345980)
- [Implementing a system call in Linux Kernel 4.7.1](https://medium.com/@ssreehari/implementing-a-system-call-in-linux-kernel-4-7-1-6f98250a8c38)
- [In class slides](https://www.dropbox.com/s/304idyd3txc8lhy/os2020_homework_1.pptx?dl=0#)

## Description

This project is about implementing scheduling algorithms that runs in user space. The related files are placed in the ``src`` directory.

The ``make`` command should generate an executable file ``scheduler``.

The inputs files are placed in the ``OS_PJ1_Test`` directory, and the outputs are placed in the ``output`` directory.

Running the ``test.sh`` script will run against all input files and generate the corresponding outputs in the ``output`` directory.