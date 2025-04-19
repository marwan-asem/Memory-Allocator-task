## How the Memory Allocator Works

This program implements a contiguous memory allocator that supports the following commands:

1. **RQ (Request)**: Allocates memory for a process using first-fit, best-fit, or worst-fit strategy
2. **RL (Release)**: Releases memory allocated to a process
3. **C (Compact)**: Compacts unused memory holes into one contiguous block
4. **STAT (Status)**: Reports the current memory allocation status
5. **X (Exit)**: Exits the program

### Implementation Details

- **Memory Representation**: I've used a linked list of `Block` structures, where each block represents a contiguous region of memory that is either used by a process or unused.
    
- **Allocation Strategies**:
    
    - **First-fit**: Allocates the first block that's large enough
    - **Best-fit**: Allocates the smallest block that's large enough
    - **Worst-fit**: Allocates the largest block that's large enough
- **Memory Release**: When memory is released, the allocator marks the block as "Unused" and merges it with adjacent unused blocks if possible.
    
- **Compaction**: The compaction algorithm moves all allocated blocks to the beginning of memory, leaving a single unused block at the end.
    

### How to Use the Program

1. Compile the program with: `gcc -o allocator memory_allocator.c`
    
2. Run the program with the total memory size: `./allocator 1048576` (for 1MB)
    
3. Use the following commands at the `allocator>` prompt:
    
    - `RQ P0 40000 W` - Request 40000 bytes for process P0 using worst-fit
    - `RL P0` - Release memory allocated to process P0
    - `C` - Compact memory
    - `STAT` - Display memory allocation status
    - `X` - Exit the program
