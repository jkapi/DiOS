DiOS
====

A simple OS I'll be working on for the foreseable future when I'm bored.

Progress
--------

- GDT setup
- IDT setup
- ISRS/IRQs setup
- Timer setup
- Basic keyboard setup
- Physical Memory Manager setup
- Virtual Memory Manager setup
- Higher Half Kernel setup
- Testing framework setup
- Kernel heap setup

Under Construction
------------------

- Basic shell to allow manual test runs, print OS info, etc
- Exploring user mode
- Organizing the file structure that's already a mess somehow (ongoing effort)

Planned
---------

- Basic data structures: array_list, linked_list
- Better Kernel heap: consolidate free blocks into larger ones
- Improve testing framework: print errors, use heap, etc...

Resources
---------

[OSDev][0] was the starting point of this project, thanks to its very useful 
beginner tutorials. Particularly, OSDev's [Meaty Skeleton][1] was used
as the skeleton in this project.

Then, [Bran's Kernel Development][2] was a great source for setting up the GDT,
IDT and the timer/keyboard. Definitely check it out if you're lost after the 
setting up your kernel.

[Brokenthorn's OS Development Series][3] was a valuable source to get started
with Physical and Virtual Memory management. But the hacky way they deal with
Higher Half Kernels and the messy approach to VMM drove me away from it a bit.
Also the code is pretty messy.

[JamesM's kernel development tutorials][4] was an extremely useful source
for setting up an initial ramdisk and VFS.

I'll try to use [The little book about OS development][5] as a general guide
from now on. It is cleaner, more updated and well-writen than the other sources
I've used up to this point. It also leave things more open for the programmer.

[0]:http://wiki.osdev.org/
[1]:http://wiki.osdev.org/User:Sortie/Meaty_Skeleton
[2]:http://www.osdever.net/bkerndev/Docs/gettingstarted.htm
[3]:http://www.brokenthorn.com/Resources/OSDevIndex.html
[4]:http://www.jamesmolloy.co.uk/tutorial_html/
[5]:http://littleosbook.github.io/

Installing
----------

I'll write this soon. One day, I'm sure.