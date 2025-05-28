#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define HEAP_SIZE 1024

typedef struct mem_block
{
    uint16_t size;
    bool free;
}block;


unsigned char heap[HEAP_SIZE];
block *freelist=NULL;

void intialiseHeap()
{
    freelist = (block*)heap;    //pointing to the 1st cell of heap array
    freelist->size = HEAP_SIZE-sizeof(block);
    freelist->free =1;
}

void * allocate_mem(size_t size)
{
    
    block* current = freelist;  //iterating through array

    size= (uint16_t)(size+3) & ~3; //making size required to the nearest multiple of 4

    while (current)
    {
        if (current->free && current->size >= size)  //checking if block is large enough
        {
            if (current->size >= size + sizeof(block)+ 4) //block is larger than required
            {
                block* newblock = (block*)((unsigned char*)current + sizeof(block) + size);
                newblock->size=current->size - size - sizeof(block);
                newblock->free=1;

                current->size=size;
            }
            current->free =0;
            return (void *)((unsigned char*)current + sizeof(block));  //return pointer to mem after header
        }
        current = (block*)((unsigned char*)current + sizeof(block)+ current->size);  //move to next block
    }
    return NULL;    // no suitable block found
}


void free_mem(void* ptr)
{
    if (!ptr) 
    {
        printf("null passed\n");
        return;
    }

    block* blocktofree = (block*)((unsigned char*)ptr - sizeof(block));
    blocktofree->free=1; //marking block as free

    //merging adjacent free block by iterating from start
    block* current =freelist;
    while ((unsigned char *)current < heap+HEAP_SIZE)
    {
        if (current->free)
        {   
            block* nextblock = (block*)((unsigned char*)current + sizeof(block) + current->size);
            
            if ((unsigned char*)nextblock < heap + HEAP_SIZE && nextblock->free)    //check if the next block is free
            {
                current->size += sizeof(block) + nextblock->size;
            }
            else
            {
                current = nextblock;
            }
        }
        else
        {
            current =(block*)((unsigned char*)current + sizeof(block) + current->size);   //going to next free block
        }
    }
}

void printheap()
{
    block* current = freelist;  //ptr to start of array
    while ((unsigned char*)current < heap + HEAP_SIZE)
    {
        printf("Block at %p: Size = %zu, Free = %d\n", current, current->size, current->free);
        current = (block*)((unsigned char*)current + sizeof(block) + current->size);
    }
}

// Function to display the status of the heap
void displayHeap() {
    block* current = freelist;
    printf("Heap Status:\n");
    while ((unsigned char*)current < heap + HEAP_SIZE) {
        printf("Block at %p: Size = %zu, Free = %d\n", (void*)current, current->size, current->free);
        if (!current->free) {
            // Print the contents of the allocated block
            printf("Contents: ");
            for (size_t i = 0; i < current->size; i++) {
                printf("%c", *((unsigned char*)current + sizeof(block) + i));
            }
            printf("\n");
        }
        current = (block*)((unsigned char*)current + sizeof(block) + current->size);
    }
}

// Function to print the entire heap array
void printHeapArray() {
    printf("Heap Array Contents:\n");
    for (size_t i = 0; i < HEAP_SIZE; i++) {
        if (i % 16 == 0) {
            printf("\n%04zu: ", i); // Print the address in hex format
        }
        printf("%02X ", heap[i]); // Print each byte in hex format //02X
    }
    printf("\n");
}

void main()
{
    intialiseHeap();

    char *a;
    a=(char*)allocate_mem(10*sizeof(char));
    if(a!=NULL)
    {
        strcpy(a, "hello");
        printf("%s\n",a);
    }

    printheap();

    free_mem(a);
    printheap();

    displayHeap();
    //printHeapArray();

}
