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
- Basic testing framework setup

Under Construction
------------------

- Tests for Paging
- Exploring user mode
- Organizing the file structure that's already a mess somehow (ongoing effort)

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
Higher Half Kernels drove me away from it a bit.

I'll try to use [The little book about OS development][4] as a general guide
from now on. It is cleaner, more updated and well-writen than the other sources
I've used up to this point.

[0]:http://wiki.osdev.org/
[1]:http://wiki.osdev.org/User:Sortie/Meaty_Skeleton
[2]:http://www.osdever.net/bkerndev/Docs/gettingstarted.htm
[3]:http://www.brokenthorn.com/Resources/OSDevIndex.html
[4]:http://littleosbook.github.io/

Installing
----------

I'll write this soon