#include "kheap.h"

#include <inc/memlayout.h>
#include <inc/dynamic_allocator.h>
#include "memory_manager.h"

//==================================================================//
//==================================================================//
//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)//
//==================================================================//
//==================================================================//

void initialize_dyn_block_system() {
	//TODO: [PROJECT MS2] [KERNEL HEAP] initialize_dyn_block_system
	// your code is here, remove the panic and write your code
	//kpanic_into_prompt("initialize_dyn_block_system() is not implemented yet...!!");

	//[1] Initialize two lists (AllocMemBlocksList & FreeMemBlocksList) [Hint: use LIST_INIT()]

	LIST_INIT(&AllocMemBlocksList);
	LIST_INIT(&FreeMemBlocksList);
	cprintf("test1 -- ");

#if STATIC_MEMBLOCK_ALLOC
	//DO NOTHING
#else
	uint32 SizeHeap = KERNEL_HEAP_MAX - KERNEL_HEAP_START;
	uint32 NumPages = (KERNEL_HEAP_MAX - KERNEL_HEAP_START) / PAGE_SIZE;
	MAX_MEM_BLOCK_CNT = NumPages;
	//-------check
	//uint32 KHS = ROUNDDOWN(KERNEL_HEAP_START, PAGE_SIZE);
	cprintf("test2 -- ");

	cprintf("test2.1 -- ");
	MemBlockNodes = (struct MemBlock *) KERNEL_HEAP_START;
	//cprintf("MBN sa: %x -- ", MemBlockNodes);
	cprintf("test2.2 -- ");

	uint32 Memsize = sizeof(struct MemBlock);
	uint32 size = Memsize * NumPages;
	cprintf("MBN sa: %x -- ", size);

	cprintf("test3 -- ");

	allocate_chunk(ptr_page_directory, KERNEL_HEAP_START, size, PERM_WRITEABLE);
	cprintf("test4 -- ");
#endif
//endif

	initialize_MemBlocksList(MAX_MEM_BLOCK_CNT);
	cprintf("test5 -- ");

	struct MemBlock* element = LIST_LAST(&AvailableMemBlocksList);
	LIST_REMOVE(&AvailableMemBlocksList, element);
	cprintf("test6 -- ");
	uint32 sizeRemain = SizeHeap - ROUNDUP(size, PAGE_SIZE);
	uint32 startFreeNode = KERNEL_HEAP_START + ROUNDUP(size, PAGE_SIZE);
	element->sva = startFreeNode;
	element->size = sizeRemain;
	cprintf("test7 -- ");
	LIST_INSERT_HEAD(&FreeMemBlocksList, element);
	cprintf("test9 -- ");
//#endif
}

void* kmalloc(unsigned int size) {
	//TODO: [PROJECT MS2] [KERNEL HEAP] kmalloc
	// your code is here, remove the panic and write your code
	//kpanic_into_prompt("kmalloc() is not implemented yet...!!");

	//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)
	//refer to the project presentation and documentation for details
	// use "isKHeapPlacementStrategyFIRSTFIT() ..." functions to check the current strategy

	struct MemBlock* element;
	uint32 newSize = ROUNDUP(size, PAGE_SIZE);

	if (isKHeapPlacementStrategyFIRSTFIT()) {
		element = alloc_block_FF(newSize);
	} else if (isKHeapPlacementStrategyBESTFIT()) {
		element = alloc_block_BF(newSize);
	}

	else if (isKHeapPlacementStrategyNEXTFIT()) {
		element = alloc_block_NF(newSize);
	}

	if (element != NULL) {
		//start address of chunk should match the start address of the block (not the block struct address)
		int returnValue = allocate_chunk(ptr_page_directory, element->sva, newSize, PERM_WRITEABLE);
		if (returnValue == 0) {
			insert_sorted_allocList(element);

			return (void*) element->sva;
		}
		else
		{
			return NULL;
		}

	}

	return NULL;
	//change this "return" according to your answer
}

void kfree(void* virtual_address) {
	//TODO: [PROJECT MS2] [KERNEL HEAP] kfree
	// Write your code here, remove the panic and write your code
	//panic("kfree() is not implemented yet...!!");

	struct MemBlock* element ;
	element = find_block(&AllocMemBlocksList,(uint32) (uint32*) virtual_address);
	if(element == NULL){
		return;
	}
	LIST_REMOVE(&AllocMemBlocksList, element);

	uint32 rangeStart = element->sva;
	uint32 rangeEnd = ROUNDUP(element->sva + element->size, PAGE_SIZE);
	for (uint32 i = rangeStart; i < rangeEnd; i += PAGE_SIZE) {
		unmap_frame(ptr_page_directory, i);
	}

		insert_sorted_with_merge_freeList(element);


}

unsigned int kheap_virtual_address(unsigned int physical_address) {
	//TODO: [PROJECT MS2] [KERNEL HEAP] kheap_virtual_address
	// Write your code here, remove the panic and write your code
	//panic("kheap_virtual_address() is not implemented yet...!!");
     struct FrameInfo * frame = to_frame_info(physical_address);

    	 return frame->va;
	//return the virtual address corresponding to given physical_address
	//refer to the project presentation and documentation for details
	//EFFICIENT IMPLEMENTATION ~O(1) IS REQUIRED ==================
}

unsigned int kheap_physical_address(unsigned int virtual_address) {
	//TODO: [PROJECT MS2] [KERNEL HEAP] kheap_physical_address
	// Write your code here, remove the panic and write your code
	//panic("kheap_physical_address() is not implemented yet...!!");
    	return virtual_to_physical(ptr_page_directory, virtual_address);

	//return the physical address corresponding to given virtual_address
	//refer to the project presentation and documentation for details
}

void kfreeall() {
	panic("Not implemented!");

}

void kshrink(uint32 newSize) {
	panic("Not implemented!");
}

void kexpand(uint32 newSize) {
	panic("Not implemented!");
}

//=================================================================================//
//============================== BONUS FUNCTION ===================================//
//=================================================================================//
// krealloc():

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to kmalloc().
//	A call with new_size = zero is equivalent to kfree().

void *krealloc(void *virtual_address, uint32 new_size) {
	//TODO: [PROJECT MS2 - BONUS] [KERNEL HEAP] krealloc
	// Write your code here, remove the panic and write your code
	panic("krealloc() is not implemented yet...!!");
}
