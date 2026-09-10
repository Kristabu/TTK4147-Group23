# Exercise 3

## Task A
### What is the difference between **Memory** and **Swap**
**Memory** is the physical RAM
**Swap** is storage that can be used as an extention of RAM, usually SD card, SSD or other. Much slower than RAM

`memset` - you are forcing the system to back the memory with physical pages

If 4GB is the max then the RPi might not have enough space since terminal, program etc are taking up some space. A solution to this could be to use swap to store the pages that do not fit in the RAM

### Change the shape of the matrix by increasing `x_dim`, what happens? Hint: does `malloc()`always allocate memory right away?
*When using `malloc` you get the address spaces reserved for your program, but you do not acctually use the RAM until you access the memory.*

Increasing `x_dim` changes the shape of the matrix while keeping the total amount of matrix data approximatly the same. Because each row is allocated separately and `matrix[y]` is a pointer to the begining of each row, fewer rows also means fewer pointers and fewer separate `malloc()`allocations. Therefore, the memory usage can decrease somewhat because there is less pointer and allocation overhead.

However, the `malloc()` calls do not necessarily cause all of the requested memory to be backed by physical RAM immediately. The program receives virtual memory, but the operating system can wait until the memory is actually accessed before assigning physical memory pages to it. This is why changing the shape of the matrix can affect the observed memory usage even though the total amount of matrix data remains approximately the same.

## Task B
### Create an array with some small capacity, then `insertBack` more elements than there is capacity for, and run the program. What happens?
When we insert more elements than the array's capacity allows, the program writes beyond the allocated memory bounds. This results in a buffer overflow, meaning the program accesses memory that does not belong to the array. The behavior is undefined: the program might appear to work normally, produce incorrect results, corrupt other data, or crash.

### Compile with the falg `-g -fsanitize=address`, what happens now?
When compiling with the -g -fsanitize=address flags, the program detects the array out-of-bounds access. Instead of silently performing an invalid memory access, AddressSanitizer reports the error and shows where it occurred in the program. This makes it easier to identify and debug the array error.

## Task C
*Just fix the code from the task above*

## Task D
### How does increasing a dynamic array's capacity by 2× or 1.5× affect available memory and the reuse of freed memory?
When a dynamic array grows, a new contiguous block of memory must be allocated, the existing elements are copied to the new block, and the old block is freed. With a growth factor of 2, the capacities grow as 2, 4, 8, 16, 32, etc. With a growth factor of 1.5, they grow approximately as 2, 3, 4, 6, 9, 13, etc. The old memory becomes available after relocation, but it cannot necessarily be reused immediately because the next array requires one contiguous block of memory. Freed memory can be reused when there is a sufficiently large contiguous free region available. Therefore, even if there is enough total free memory, it may not be possible to use it for the new array if the free memory is fragmented into smaller regions.

### In what cases will you be able to reuse memory that you have freed up?
You can reuse freed memory if the freed region is large enough and appropriately located for the new allocation.