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

1. Compile the program with: `gcc allocator.c -o allocator`
    
2. Run the program with the total memory size: `./allocator 1048576` (for 1MB)
    
3. Use the following commands at the `allocator>` prompt:
    
    - `RQ P0 40000 W` - Request 40000 bytes for process P0 using worst-fit
    - `RL P0` - Release memory allocated to process P0
    - `C` - Compact memory
    - `STAT` - Display memory allocation status
    - `X` - Exit the program

--------
### Example for testing code
Here's a test scenario plan showing how external fragmentation occurs and how compaction solves it:

1) Add 3 processes in sequence, each with specific memory sizes
2) Remove the middle process to create fragmentation
3) Try to add a new process that fails due to external fragmentation
4) Perform compaction
5) Successfully add the same process after compaction


**First**, initialize the memory allocator with a specific size, let's say 1000 bytes:
```bash
./allocator 1000
```

#### 1)Add 3 processes in sequence:

```bash
allocator> RQ P1 300 F
allocator> RQ P2 200 F
allocator> RQ P3 400 F
```


#### 2)Check current memory status

```bash
allocator> STAT
```
**Output:**

```bash
Addresses [0:299] Process P1
Addresses [300:499] Process P2
Addresses [500:899] Process P3
Addresses [900:999] Unused
```

#### 3)Remove the middle process (P2)
```bash
allocator> RL P2

```

#### 4)Check status to confirm fragmentation
```bash
allocator> STAT
```
**Output:**
```bash
Addresses [0:299] Process P1
Addresses [300:499] Unused
Addresses [500:899] Process P3
Addresses [900:999] Unused
```
#### 5)Try adding a new process that won't fit in any individual free block
 
```bash
allocator> RQ P4 350 F
```
**output:**
```bash
Failed to allocate memory for process P4
```

#### 6)Perform memory compaction
```bash
allocator> C
```

#### 7)Check status after compaction
```bash
allocator> STAT

```
**output:**
```bash
Addresses [0:299] Process P1
Addresses [300:699] Process P3
Addresses [700:999] Unused
```
#### 8)Try adding P4 again
```bash
allocator> RQ P4 300 F

```
output:
```bash
Successfully allocated 300 to process P4
```
#### 9)Check final status
allocator> STAT
**output:**
```bash
Addresses [0:299] Process P1
Addresses [300:699] Process P3
Addresses [700:999] Process P4
```

#### 10)Exit the program
```bash
allocator> X
```