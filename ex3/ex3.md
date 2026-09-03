# Exercise 3

## Task A
### What is the difference between **Memory** and **Swap**
**Memory** is the physical RAM
**Swap** is storage that can be used as an extention of RAM, usually SD card, SSD or other. Much slower than RAM

### Change the shape of the matrix by increasing `x_dim`, what happens? Hint: does `malloc()`always allocate memory right away?
When using `malloc` you get the address spaces reserved for your program, but you do not acctually use the RAM until you access the memory. 

Chanigng does not  appear `x_dim` to do anything because you are just reserving the space, but not accessing it yet.

`memset` - you are forcing the system to back the memory with physical pages

If 4GB is the max then the RPi might not have enough space since terminal, program etc are taking up some space. A solution to this could be to use swap to store the pages that do not fit in the RAM

## Task B

## Task C

## Task D