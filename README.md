# Garbage-Collector

This garbage-collecting memory manager for C, replacing the functions malloc and free.

Function myallocint initializes the memory manager. It calls malloc to get memory and initializes that memeory to correspond to 2 blocks: the large free bloack, and the sentinel allocated block (of size 0) at the end of the memory. The arugment to the function specifies the byte the memory manager must be able to allocate in all.

myalloc uses first-fit (as opposed to best-fit), splitting the block as much as possible. The code for free simply marks the freed blco
