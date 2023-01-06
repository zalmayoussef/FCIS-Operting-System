#include <inc/lib.h>

//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//

int FirstTimeFlag = 1;
void InitializeUHeap()
{
	if(FirstTimeFlag)
	{
		initialize_dyn_block_system();
		cprintf("DYNAMIC BLOCK SYSTEM IS INITIALIZED\n");
#if UHP_USE_BUDDY
		initialize_buddy();
		cprintf("BUDDY SYSTEM IS INITIALIZED\n");
#endif
		FirstTimeFlag = 0;
	}
}

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

//=================================
// [1] INITIALIZE DYNAMIC ALLOCATOR:
//=================================
void initialize_dyn_block_system()
{
	//TODO: [PROJECT MS3] [USER HEAP - USER SIDE] initialize_dyn_block_system
	// your code is here, remove the panic and write your code
	//panic("initialize_dyn_block_system() is not implemented yet...!!");
	LIST_INIT(&AllocMemBlocksList);
	LIST_INIT(&FreeMemBlocksList);
	uint32 SizeUHeap = USER_HEAP_MAX - USER_HEAP_START;//0xA0000000-0x80000000
	uint32 NumPages = (USER_HEAP_MAX - USER_HEAP_START)/ PAGE_SIZE;
	MAX_MEM_BLOCK_CNT = NumPages;
	MemBlockNodes = (struct MemBlock *) USER_DYN_BLKS_ARRAY;
	uint32 Memsize = sizeof(struct MemBlock);
	uint32 size = Memsize * NumPages;
	sys_allocate_chunk(USER_DYN_BLKS_ARRAY,size,PERM_USER|PERM_WRITEABLE);
	initialize_MemBlocksList(MAX_MEM_BLOCK_CNT);
	struct MemBlock* freeblock =  LIST_LAST(&AvailableMemBlocksList);
	LIST_REMOVE(&AvailableMemBlocksList, freeblock);
	uint32 sizeRemain = SizeUHeap - ROUNDUP(size, PAGE_SIZE);
	uint32 startFreeNode = KERNEL_HEAP_START + ROUNDUP(size, PAGE_SIZE);
	freeblock->sva = USER_HEAP_START;
	freeblock->size = SizeUHeap;
	LIST_INSERT_HEAD(&FreeMemBlocksList, freeblock);
	//[1] Initialize two lists (AllocMemBlocksList & FreeMemBlocksList) [Hint: use LIST_INIT()]
	//[2] Dynamically allocate the array of MemBlockNodes at VA USER_DYN_BLKS_ARRAY
	//      (remember to set MAX_MEM_BLOCK_CNT with the chosen size of the array)
	//[3] Initialize AvailableMemBlocksList by filling it with the MemBlockNodes
	//[4] Insert a new MemBlock with the heap size into the FreeMemBlocksList
}

//=================================
// [2] ALLOCATE SPACE IN USER HEAP:
//=================================
void* malloc(uint32 size)
{
	//==============================================================
	//DON'T CHANGE THIS CODE========================================
	InitializeUHeap();
	if (size == 0) return NULL ;
	//==============================================================
	//==============================================================

	//TODO: [PROJECT MS3] [USER HEAP - USER SIDE] malloc
	// your code is here, remove the panic and write your code
	//panic("malloc() is not implemented yet...!!");
	struct MemBlock* element;
	uint32 newSize = ROUNDUP(size, PAGE_SIZE);
	if (sys_isUHeapPlacementStrategyFIRSTFIT()){
	    element = alloc_block_FF (newSize);
	}else if (sys_isUHeapPlacementStrategyBESTFIT()) {
		cprintf("Bestfit");
	}

	else if (sys_isUHeapPlacementStrategyNEXTFIT()) {
		cprintf("Nextfit");
	}
	else{
		return NULL;
	}
	if(element != NULL){
		LIST_INSERT_HEAD(&AllocMemBlocksList,element);
	    return (void*) element->sva;
	}
	else{return NULL;}
	// Steps:
	//	1) Implement FF strategy to search the heap for suitable space
	//		to the required allocation size (space should be on 4 KB BOUNDARY)
	//	2) if no suitable space found, return NULL
	// 	3) Return pointer containing the virtual address of allocated space,
	//
	//Use sys_isUHeapPlacementStrategyFIRSTFIT()... to check the current strategy
}

//=================================
// [3] FREE SPACE FROM USER HEAP:
//=================================
// free():
//	This function frees the allocation of the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	FROM main memory AND free pages from page file then switch back to the user again.
//
//	We can use sys_free_user_mem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls free_user_mem() in
//		"kern/mem/chunk_operations.c", then switch back to the user mode here
//	the free_user_mem function is empty, make sure to implement it.
void free(void* virtual_address)
{
	//TODO: [PROJECT MS3] [USER HEAP - USER SIDE] free
	    // your code is here, remove the panic and write your code
//	    //panic("free() is not implemented yet...!!");
	    struct MemBlock* element ;
	    element = find_block(&AllocMemBlocksList,(uint32)virtual_address);
	    if(element != NULL){
	    	sys_free_user_mem((uint32) (uint32*) virtual_address, element->size);
			LIST_REMOVE(&AllocMemBlocksList, element);
			insert_sorted_with_merge_freeList(element);
	    }

	    //you should get the size of the given allocation using its address
	    //you need to call sys_free_user_mem()
	    //refer to the project presentation and documentation for details

	//you need to call sys_free_user_mem()
	//refer to the project presentation and documentation for details
}

//=================================
// [4] ALLOCATE SHARED VARIABLE:
//=================================
void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable)
{
	//==============================================================
	//DON'T CHANGE THIS CODE========================================
	InitializeUHeap();
	if (size == 0) return NULL ;
	//==============================================================

	struct MemBlock* node;
				size = ROUNDUP(size, PAGE_SIZE);

				if(sys_isUHeapPlacementStrategyFIRSTFIT())
				{
					node = alloc_block_FF(size);
				}
					if(node != NULL)
					{
						    int ID = sys_createSharedObject(sharedVarName,size,isWritable,(void*)node->sva);

							if(ID >= 0){
								return (void*)node->sva;
							}
							else
							{
								return NULL;
							}
					}
				return NULL;
	//* ON 4KB BOUNDARY ******************* //
	//Use sys_isUHeapPlacementStrategyFIRSTFIT() to check the current strategy
}

//========================================
// [5] SHARE ON ALLOCATED SHARED VARIABLE:
//========================================
void* sget(int32 ownerEnvID, char *sharedVarName)
{
	//==============================================================
	//DON'T CHANGE THIS CODE========================================
	InitializeUHeap();
	//==============================================================

	//TODO: [PROJECT MS3] [SHARING - USER SIDE] sget()
	// Write your code here, remove the panic and write your code
	//panic("sget() is not implemented yet...!!");

	// Steps:
		//	1) Get the size of the shared variable (use sys_getSizeOfSharedObject())
		int retSize = sys_getSizeOfSharedObject(ownerEnvID, sharedVarName);
		retSize = ROUNDUP(retSize, PAGE_SIZE);
		struct MemBlock* node;

		//	2) If not exists, return NULL
		if (retSize == 0){
			return NULL;
		}
		else {
			if(sys_isUHeapPlacementStrategyFIRSTFIT())
			{
				node = alloc_block_FF(retSize);
			}
			if(node != NULL)
			{
				int ID = sys_getSharedObject(ownerEnvID, sharedVarName, (void*)node->sva);
				if(ID >= 0){
					return (void*)node->sva;
				}
				else
				{
					return NULL;
				}
			}
			return NULL;
		}
}


//==================================================================================//
//============================== BONUS FUNCTIONS ===================================//
//==================================================================================//


//=================================
// REALLOC USER SPACE:
//=================================
//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to malloc().
//	A call with new_size = zero is equivalent to free().

//  Hint: you may need to use the sys_move_user_mem(...)
//		which switches to the kernel mode, calls move_user_mem(...)
//		in "kern/mem/chunk_operations.c", then switch back to the user mode here
//	the move_user_mem() function is empty, make sure to implement it.
void *realloc(void *virtual_address, uint32 new_size)
{
	//==============================================================
	//DON'T CHANGE THIS CODE========================================
	InitializeUHeap();
	//==============================================================
	// [USER HEAP - USER SIDE] realloc
	// Write your code here, remove the panic and write your code
	panic("realloc() is not implemented yet...!!");
}


//=================================
// FREE SHARED VARIABLE:
//=================================
//	This function frees the shared variable at the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from main memory then switch back to the user again.
//
//	use sys_freeSharedObject(...); which switches to the kernel mode,
//	calls freeSharedObject(...) in "shared_memory_manager.c", then switch back to the user mode here
//	the freeSharedObject() function is empty, make sure to implement it.

void sfree(void* virtual_address)
{
	//TODO: [PROJECT MS3 - BONUS] [SHARING - USER SIDE] sfree()

	// Write your code here, remove the panic and write your code
	panic("sfree() is not implemented yet...!!");
}




//==================================================================================//
//========================== MODIFICATION FUNCTIONS ================================//
//==================================================================================//
void expand(uint32 newSize)
{
	panic("Not Implemented");

}
void shrink(uint32 newSize)
{
	panic("Not Implemented");

}
void freeHeap(void* virtual_address)
{
	panic("Not Implemented");
}
