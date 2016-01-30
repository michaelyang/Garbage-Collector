# Garbage-Collector

This garbage-collecting memory manager for C, replacing the functions malloc and free.

Function myallocint initializes the memory manager. It calls malloc to get memory and initializes that memeory to correspond to 2 blocks: the large free bloack, and the sentinel allocated block (of size 0) at the end of the memory. The arugment to the function specifies the byte the memory manager must be able to allocate in all.

Function myalloc uses first-fit (as opposed to best-fit), splitting the block as much as possible. The code for free simply marks the freed block as unallocated.

Function coaslesce cpalesces all adjoining free blocks in the block list. After myalloc calls coalesce, it searches the block list again.

Function printallocation prints the current size and allocation of each block. This will be useful for debugging purposes.

Function mygc performs garbage collection. It uses the mark-and-sweep algorithm. Using the fact that the size of a block is divisible by 8, which means the rightmost 3 bits will always be 0's, the program uses the rightmost bit to encode the allocation. The 2nd bit from the right is used to encode the marking (That is, a block is marked if the value of (size % 4) is either 2 or 3. The isPtr function looks to see if there is an allocated block that the specified pointer is pointing into. The block is canned lienarly (while a binary tree could've been used), and looks to see whether the specified pointer points into any allocated block. To find roots, every value on the AR stack is scanned. Assuming that each 4-byte value is a pointer, the isPtr function is called on each value. Since we can't look from the bottom of the stack in the current Linux system as it would create problems, we look from the top of the stack and thus the first variable you allocate in mygc should be of type (int *). This address will be the lowest address you look at.

To use the test functions, call:
gcc -o AnyName testx.c myalloc.c
Then run the compiled code.

Test1: Simple test for the basic functions such as myallocinit, myalloc, printallocation, and myfree.
Test2: This test reads up to 5 doubles from stdin, terminated by a negative number.  It then inserts them into a list, prints the list, and frees the list nodes. Note that more than 5 doubles gives a segmentation fault.
Test3: This test reads up to 5 doubles from stdin, terminated by a negative number.  It then inserts them into a list, and prints the list. Unlike test2, it doesn't free the list nodes.  Instead, it calls the garbage collector. Again, note that more than 5 doubles gives a segmentation fault.
Test4: A simple test for the gc collector after calling a sequence of myalloc and myfree
Test5: This tests whether garbage collection works after a function call exits. Note that after the AR is deallocated, a pointer to the heap may still exist, and an unreachable block will not get garbage collected.
