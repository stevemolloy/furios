# furios
An experiment to see if an idiot (and his buddies) can build an operating system.

## The goal
Based on the [skeleton](https://gitlab.com/sortie/meaty-skeleton) provided by the good folks at 
[OSDEV.org](https://wiki.osdev.org/Meaty_Skeleton), the plan is to build an operating system capable of building itself.

That is, an operating system that could be installed on a real machine (not just a VM), and could then be used to build
itself from source.

Although this doesn't require networking (the source code could be bundled with the OS image), a graphical front-end, or 
drivers for all the different devices that could be on the real machine, this is still a considerable piece of work.  It will 
need all the apparatus that allows installation and usage of a compiler -- memory management, filesystem access, a C library, 
a linker, etc.  And not only will filesystem access be needed, but we will need to make an actual filesystem.

All in all, a terrifyingly substantial piece of work.

## TODO's
At this stage it's a little laughable to have a list of TODO's, since it's best summed up by: TODO = Everything
Nevertheless, here's a list you can add to.

* Improved build system.  A build that is held together by multiple (interdependent) shell scripts, and Makefiles located in various directories, doesn't sound maintainable.  This should be a single Makefile at the top-level folder.
* Improved VGA driver.  At the moment, the VGA driver has enough functionality to print a couple of words to the screen.
There is not yet any ability for text to wrap off the right-hand edge, or for the screen to scroll when the bottom is reached.
* ...
