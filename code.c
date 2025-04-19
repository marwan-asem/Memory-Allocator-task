#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PROCESS_NAME 10
#define MAX_COMMAND_SIZE 50

typedef struct Block {
    int start;
    int end;
    char process[MAX_PROCESS_NAME];
    struct Block* next;
} Block;

Block* memory = NULL;
int max_memory_size = 0;

// Function to initialize memory
void initialize_memory(int size) {
    Block* new_block = (Block*)malloc(sizeof(Block));
    new_block->start = 0;
    new_block->end = size - 1;
    strcpy(new_block->process, "Unused");
    new_block->next = NULL;
    memory = new_block;
    max_memory_size = size;
}

// Function to print memory status
void print_memory_status() {
    Block* current = memory;
    
    while (current != NULL) {
        printf("Addresses [%d:%d] ", current->start, current->end);
        if (strcmp(current->process, "Unused") == 0) {
            printf("Unused\n");
        } else {
            printf("Process %s\n", current->process);
        }
        current = current->next;
    }
}

// Function to merge adjacent unused blocks
void merge_unused_blocks() {
    Block* current = memory;
    
    while (current != NULL && current->next != NULL) {
        if (strcmp(current->process, "Unused") == 0 && strcmp(current->next->process, "Unused") == 0) {
            // Merge the two blocks
            Block* to_delete = current->next;
            current->end = to_delete->end;
            current->next = to_delete->next;
            free(to_delete);
        } else {
            current = current->next;
        }
    }
}

// Function to request memory using first fit
bool request_first_fit(char* process_name, int size) {
    Block* current = memory;
    
    while (current != NULL) {
        if (strcmp(current->process, "Unused") == 0) {
            int block_size = current->end - current->start + 1;
            
            if (block_size >= size) {
                // Found a block that fits
                if (block_size == size) {
                    // Perfect fit
                    strcpy(current->process, process_name);
                } else {
                    // Need to split the block
                    Block* new_block = (Block*)malloc(sizeof(Block));
                    new_block->start = current->start + size;
                    new_block->end = current->end;
                    strcpy(new_block->process, "Unused");
                    new_block->next = current->next;
                    
                    current->end = current->start + size - 1;
                    strcpy(current->process, process_name);
                    current->next = new_block;
                }
                return true;
            }
        }
        current = current->next;
    }
    
    return false; // No suitable block found
}

// Function to request memory using best fit
bool request_best_fit(char* process_name, int size) {
    Block* current = memory;
    Block* best_block = NULL;
    int best_size = max_memory_size + 1; // Initialize with a value larger than possible
    
    // Find the best fit block
    while (current != NULL) {
        if (strcmp(current->process, "Unused") == 0) {
            int block_size = current->end - current->start + 1;
            
            if (block_size >= size && block_size < best_size) {
                best_block = current;
                best_size = block_size;
            }
        }
        current = current->next;
    }
    
    // If a suitable block is found, allocate memory
    if (best_block != NULL) {
        if (best_size == size) {
            // Perfect fit
            strcpy(best_block->process, process_name);
        } else {
            // Need to split the block
            Block* new_block = (Block*)malloc(sizeof(Block));
            new_block->start = best_block->start + size;
            new_block->end = best_block->end;
            strcpy(new_block->process, "Unused");
            new_block->next = best_block->next;
            
            best_block->end = best_block->start + size - 1;
            strcpy(best_block->process, process_name);
            best_block->next = new_block;
        }
        return true;
    }
    
    return false; // No suitable block found
}

// Function to request memory using worst fit
bool request_worst_fit(char* process_name, int size) {
    Block* current = memory;
    Block* worst_block = NULL;
    int worst_size = -1; // Initialize with a small value
    
    // Find the worst fit block
    while (current != NULL) {
        if (strcmp(current->process, "Unused") == 0) {
            int block_size = current->end - current->start + 1;
            
            if (block_size >= size && block_size > worst_size) {
                worst_block = current;
                worst_size = block_size;
            }
        }
        current = current->next;
    }
    
    // If a suitable block is found, allocate memory
    if (worst_block != NULL) {
        if (worst_size == size) {
            // Perfect fit
            strcpy(worst_block->process, process_name);
        } else {
            // Need to split the block
            Block* new_block = (Block*)malloc(sizeof(Block));
            new_block->start = worst_block->start + size;
            new_block->end = worst_block->end;
            strcpy(new_block->process, "Unused");
            new_block->next = worst_block->next;
            
            worst_block->end = worst_block->start + size - 1;
            strcpy(worst_block->process, process_name);
            worst_block->next = new_block;
        }
        return true;
    }
    
    return false; // No suitable block found
}

// Function to release memory
bool release_memory(char* process_name) {
    Block* current = memory;
    bool found = false;
    
    while (current != NULL) {
        if (strcmp(current->process, process_name) == 0) {
            // Found the process, mark as unused
            strcpy(current->process, "Unused");
            found = true;
        }
        current = current->next;
    }
    
    if (found) {
        merge_unused_blocks();
        return true;
    }
    
    return false;
}

// Function to compact memory
void compact_memory() {
    if (memory == NULL) return;
    
    int total_unused = 0;
    Block* current = memory;
    Block* new_memory = NULL;
    Block* last = NULL;
    
    // First, measure all unused memory
    while (current != NULL) {
        if (strcmp(current->process, "Unused") == 0) {
            total_unused += (current->end - current->start + 1);
        }
        current = current->next;
    }
    
    // Now create a new list, with all used blocks first
    current = memory;
    int next_start = 0;
    
    while (current != NULL) {
        if (strcmp(current->process, "Unused") != 0) {
            Block* new_block = (Block*)malloc(sizeof(Block));
            int block_size = current->end - current->start + 1;
            
            new_block->start = next_start;
            new_block->end = next_start + block_size - 1;
            strcpy(new_block->process, current->process);
            new_block->next = NULL;
            
            if (new_memory == NULL) {
                new_memory = new_block;
                last = new_block;
            } else {
                last->next = new_block;
                last = new_block;
            }
            
            next_start += block_size;
        }
        current = current->next;
    }
    
    // Add the unused block at the end if there's any unused memory
    if (total_unused > 0) {
        Block* unused_block = (Block*)malloc(sizeof(Block));
        unused_block->start = next_start;
        unused_block->end = next_start + total_unused - 1;
        strcpy(unused_block->process, "Unused");
        unused_block->next = NULL;
        
        if (new_memory == NULL) {
            new_memory = unused_block;
        } else {
            last->next = unused_block;
        }
    }
    
    // Free the old memory list
    current = memory;
    while (current != NULL) {
        Block* to_delete = current;
        current = current->next;
        free(to_delete);
    }
    
    memory = new_memory;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <memory_size>\n", argv[0]);
        return 1;
    }
    
    int memory_size = atoi(argv[1]);
    initialize_memory(memory_size);
    
    char command[MAX_COMMAND_SIZE];
    char process_name[MAX_PROCESS_NAME];
    int size;
    char strategy;
    bool success;
    
    while (1) {
        printf("allocator> ");
        fgets(command, MAX_COMMAND_SIZE, stdin);
        
        // Remove newline character
        command[strcspn(command, "\n")] = 0;
        
        if (strncmp(command, "RQ", 2) == 0) {
            // Request memory
            sscanf(command, "RQ %s %d %c", process_name, &size, &strategy);
            
            switch (strategy) {
                case 'F':
                    success = request_first_fit(process_name, size);
                    break;
                case 'B':
                    success = request_best_fit(process_name, size);
                    break;
                case 'W':
                    success = request_worst_fit(process_name, size);
                    break;
                default:
                    printf("Invalid allocation strategy. Use F, B, or W.\n");
                    continue;
            }
            
            if (success) {
                printf("Successfully allocated %d to process %s\n", size, process_name);
            } else {
                printf("Failed to allocate memory for process %s\n", process_name);
            }
        } else if (strncmp(command, "RL", 2) == 0) {
            // Release memory
            sscanf(command, "RL %s", process_name);
            
            if (release_memory(process_name)) {
                printf("Successfully released memory for process %s\n", process_name);
            } else {
                printf("Failed to release memory. Process %s not found.\n", process_name);
            }
        } else if (strcmp(command, "C") == 0) {
            // Compact memory
            compact_memory();
            printf("Memory compaction completed\n");
        } else if (strcmp(command, "STAT") == 0) {
            // Report memory status
            print_memory_status();
        } else if (strcmp(command, "X") == 0) {
            // Exit program
            break;
        } else {
            printf("Invalid command\n");
        }
    }
    
    // Clean up memory
    Block* current = memory;
    while (current != NULL) {
        Block* to_delete = current;
        current = current->next;
        free(to_delete);
    }
    
    return 0;
}