#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "smalloc.h"



void *mem;
struct block *freelist;
struct block *allocated_list;
static struct block *pointers[2];



void *smalloc(unsigned int nbytes) {
    // Instantiated a pointer to search through freelist with.
    struct block *freeblock;
    freeblock = freelist;

    // Search for a block with larger than the required size.
    while (freeblock->size < nbytes && freeblock->next != NULL) {
        freeblock = freeblock->next;
    }
    
    if (freeblock->size >= nbytes) {
	printf("Address of block with adequate size: %p\n", freeblock->addr);
        struct block *next = allocated_list;
        struct block *allocate = malloc(nbytes);
	printf("				allocate: %p\n", &allocate);
        
        if (allocated_list == NULL) {
            allocated_list = malloc(nbytes);
            allocated_list->addr = freeblock->addr;
            allocated_list->size = nbytes;
            allocated_list->next = NULL;
        }
        
        else {
        allocate->addr = freeblock->addr;
        allocate->size = nbytes;
        allocate->next = next;
        *allocated_list = allocate;
	printf("				allocated_list: %p\n", &allocated_list);
	printf("allocated_list->addr: %p\n", allocated_list->addr);
        freeblock->addr = freeblock->addr + nbytes;
        freeblock->size = freeblock->size - nbytes;
	printf("freelist->addr: %p\n\n", freelist->addr);
        return allocated_list->addr;
        }
    }

    return NULL;
}


static struct block *find_parent_and_child(void *addr, struct block *block) {
    struct block *parent, *child;
    child = block;

    if (block == allocated_list) {
        while (child->addr != addr && child->next != NULL) {
            parent = child;
            child = child->next;
        }
    }

    else {
        while (child->addr < addr && child->next != NULL) {
            parent = child;
            child = child->next;
        }
    }

    pointers[0] = parent;
    pointers[1] = child;

    return child;
}


int sfree(void *addr) {
    struct block *parentblock, *childblock;
    struct block *block_to_remove = find_parent_and_child(addr, allocated_list);
    parentblock = pointers[0];
    childblock = pointers[1];

    if (childblock->addr != addr) {
        return -1;
    }

    if (childblock == allocated_list) {
        allocated_list = childblock->next;
    }

    else {
        parentblock->next = childblock->next;
    }

    find_parent_and_child(addr, freelist);
    parentblock = pointers[0];
    childblock = pointers[1];

    if (childblock->addr != addr) {
        return -1;
    }

    if (childblock == freelist) {
        block_to_remove->next = freelist;
        freelist = block_to_remove;
    }

    else {
        parentblock->next = block_to_remove;
        block_to_remove->next = childblock;
    }
}


/* Initialize the memory space used by smalloc,
 * freelist, and allocated_list
 * Note:  mmap is a system call that has a wide variety of uses.  In our
 * case we are using it to allocate a large region of memory. 
 * - mmap returns a pointer to the allocated memory
 * Arguments:
 * - NULL: a suggestion for where to place the memory. We will let the 
 *         system decide where to place the memory.
 * - PROT_READ | PROT_WRITE: we will use the memory for both reading
 *         and writing.
 * - MAP_PRIVATE | MAP_ANON: the memory is just for this process, and 
 *         is not associated with a file.
 * - -1: because this memory is not associated with a file, the file 
 *         descriptor argument is set to -1
 * - 0: only used if the address space is associated with a file.
 */
void mem_init(int size) {
    mem = mmap(NULL, size,  PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if(mem == MAP_FAILED) {
         perror("mmap");
         exit(1);
    }

    /* NOTE: this function is incomplete */

    freelist = malloc(size);
    freelist->addr = mem;
    freelist->size = size;
    freelist->next = NULL;    
}


void mem_clean(){
    struct block *lists[2];
    lists[0] = allocated_list;
    lists[1] = freelist;

    int j;
    for (j = 0; j < 2; j++) {
        struct block *list = lists[j];
        struct block *block;
        struct block *next_block;
        for (block = list; free != NULL; ) {
            next_block = block->next;
            free(&block);
            block = next_block;
        }
    }
}


int main(int argc, char ** argv) {
	printf("\n\n");
	mem_init(1000000);
	void *ptrs[3];
        printf("ptrs[0] = smalloc(1000001)\n");
        ptrs[0] = smalloc(1000001);
	printf("ptrs[0] = smalloc(16)\n");
	ptrs[0] = smalloc(16);
	printf("ptrs[1] = smalloc(24)\n");
	ptrs[1] = smalloc(24);
	printf("ptrs[2] = smalloc(32)\n");
	ptrs[2] = smalloc(32);
	printf("sfree(ptrs[0])\n");
        sfree(ptrs[0]);
	printf("mem_clean()\n");
//        mem_clean();
	free(&allocated_list->next);
//	free(&allocated_list->addr);
//	free(&allocated_list->size); 
	allocated_list = NULL;
	printf("	allocated_list->addr: %p\n", &allocated_list->addr);
	printf("	freelist->addr: %p\n", freelist->addr);
	return 0;
}

