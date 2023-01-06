/*
 * dyn_block_management.c
 *
 *  Created on: Sep 21, 2022
 *      Author: HP
 */
#include <inc/assert.h>
#include <inc/string.h>
#include "../inc/dynamic_allocator.h"


//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//

//===========================
// PRINT MEM BLOCK LISTS:
//===========================

void print_mem_block_lists()
{
	cprintf("\n=========================================\n");
	struct MemBlock* blk ;
	struct MemBlock* lastBlk = NULL ;
	cprintf("\nFreeMemBlocksList:\n");
	uint8 sorted = 1 ;
	LIST_FOREACH(blk, &FreeMemBlocksList)
	{
		if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
			sorted = 0 ;
		cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size) ;
		lastBlk = blk;
	}
	if (!sorted)	cprintf("\nFreeMemBlocksList is NOT SORTED!!\n") ;

	lastBlk = NULL ;
	cprintf("\nAllocMemBlocksList:\n");
	sorted = 1 ;
	LIST_FOREACH(blk, &AllocMemBlocksList)
	{
		if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
			sorted = 0 ;
		cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size) ;
		lastBlk = blk;
	}
	if (!sorted)	cprintf("\nAllocMemBlocksList is NOT SORTED!!\n") ;
	cprintf("\n=========================================\n");

}

//********************************************************************************//
//********************************************************************************//

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

//===============================
// [1] INITIALIZE AVAILABLE LIST:
//===============================
void initialize_MemBlocksList(uint32 numOfBlocks)
{
    LIST_INIT(&AvailableMemBlocksList);
    int i;
    for (int i = 0; i< numOfBlocks; i++){

        LIST_INSERT_HEAD(&AvailableMemBlocksList, &(MemBlockNodes[i]));

    }
		    /* 4. change prev of head node to new node */
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] initialize_MemBlocksList
	// Write your code here, remove the panic and write your code

	//panic("initialize_MemBlocksList() is not implemented yet...!!");

}

//===============================
// [2] FIND BLOCK:
//===============================
struct MemBlock *find_block(struct MemBlock_List *blockList, uint32 va)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] find_block
	// Write your code here, remove the panic and write your code
	//panic("find_block() is not implemented yet...!!");
	struct MemBlock* node ;
		LIST_FOREACH(node, blockList)
		{
			if(node->sva == va)
			{
			return node;
			}
		}
		return NULL;

}

//=========================================
// [3] INSERT BLOCK IN ALLOC LIST [SORTED]:
//=========================================
void insert_sorted_allocList(struct MemBlock *blockToInsert)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_allocList
	// Write your code here, remove the panic and write your code
	//panic("insert_sorted_allocList() is not implemented yet...!!");
	struct MemBlock* node ;
	struct MemBlock* node2 ;
	struct MemBlock* node3 ;
	struct MemBlock* nextnode ;

    node3 = LIST_FIRST(&AllocMemBlocksList);
	node2= LIST_LAST(&AllocMemBlocksList);
		if (LIST_SIZE(&AllocMemBlocksList)==0)
			{
			LIST_INSERT_HEAD(&AllocMemBlocksList,blockToInsert);
			}
			else if (node3->sva > blockToInsert->sva){
		                LIST_INSERT_HEAD(&AllocMemBlocksList,blockToInsert);
		            }
		else
		{
			// 1:n:0:0
			LIST_FOREACH(node,&AllocMemBlocksList)
				{
						if (node->sva<blockToInsert->sva )
							{
							if (node == node2  )
							LIST_INSERT_AFTER(&AllocMemBlocksList,node,blockToInsert);
							}
						else{
							continue;
						}
				}
			LIST_FOREACH(node,&AllocMemBlocksList)
				{

				nextnode = LIST_NEXT(node);
					if(node->sva<blockToInsert->sva)
					{
						if(nextnode->sva>blockToInsert->sva)
						{
							LIST_INSERT_AFTER(&AllocMemBlocksList,node,blockToInsert);
						}
						else{
							continue;
						}
					}
				}
					}

}
//LIST_INSERT_HEAD(&AllocMemBlocksList,blockToInsert);
//=========================================
// [4] ALLOCATE BLOCK BY FIRST FIT:
//=========================================
struct MemBlock *alloc_block_FF(uint32 size)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_FF
	// Write your code here, remove the panic and write your code
	//panic("alloc_block_FF() is not implemented yet...!!");
	struct MemBlock* node ;
	struct MemBlock* node2 ;
			LIST_FOREACH(node,&FreeMemBlocksList)
				{
				 if(node->size==size)
				{
					LIST_REMOVE(&FreeMemBlocksList, node);
					return node;
				}
				else if(node->size > size)
				{
					//1 new Block
					node2 = LIST_LAST(&AvailableMemBlocksList);
					node2->size=size;
					node2->sva = node->sva;
					node->sva = node->sva + size;
					node->size = node->size - size;
					LIST_REMOVE(&AvailableMemBlocksList, node2);
					//LIST_REMOVE(&FreeMemBlocksList, node3);
					return node2;
					//2 return
								}
		}
			return NULL;

}

//=========================================
// [5] ALLOCATE BLOCK BY BEST FIT:
//=========================================
struct MemBlock *alloc_block_BF(uint32 size)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_BF
	// Write your code here, remove the panic and write your code

	    struct MemBlock* node ;
	    struct MemBlock* node2;
	    struct MemBlock* node3;
	    int c=0;
	    uint32 MINsize=0xFFFFFFF;
	             LIST_FOREACH(node,&FreeMemBlocksList)
	                    {
	                       if (node->size == size)
	                          {
	                             LIST_REMOVE(&FreeMemBlocksList, node);
	                              return node;
	                          }
	                   }
	             LIST_FOREACH(node,&FreeMemBlocksList)
	                        {
	                            if(node->size>size)
	                            {
	                            	c++;
	                                if(node->size<MINsize)
	                                          MINsize=node->size;

	                            }
	                        }
	             LIST_FOREACH(node,&FreeMemBlocksList)// 2  4  5  *13  1  *15
	          	  	  	{
	        	  	  	  	  if (node->size > size)
	        	  	  	  	  	  	  {
	        	  	  	  		  	  	  if(c==1)
	        	  	  	  		  	  	  	  {
	        	  	  	  		  	  		  	  	  node2 = LIST_LAST(&AvailableMemBlocksList);
	        	  	  	  		  	  		  	  	  node2->size=size;
	        	  	  	  		  	  		  	  	  node2->sva = node->sva;
	        	  	  	  		  	  		  	  	  node->sva = node->sva + size;
	        	  	  	  		  	  		  	  	  node->size = node->size - size;
	        	  	  	  		  	  		  	  	  LIST_REMOVE(&AvailableMemBlocksList, node2);
	        	  	  	  		  	  		  	  	  return node2;
	        	  	  	  		  	  	  	  }
	        	  	  	  	else if(node->size==MINsize)
	        	  	  	  				{
	        	  	  	  						node2 = LIST_LAST(&AvailableMemBlocksList);
	        	  	  	  						node2->size=size;
	        	  	  	  						node2->sva = node->sva;
	        	  	  	  						node->sva = node->sva + size;
	        	  	  	  						node->size = node->size - size;
	        	  	  	  						LIST_REMOVE(&AvailableMemBlocksList, node2);
	        	  	  	  						return node2;

	        	  	  	  					}
	        	  	  	  	  	  	  }
	          	  	  	}
	            return NULL;
}
	//panic("alloc_block_BF() is not implemented yet...!!");
//=========================================
// [7] ALLOCATE BLOCK BY NEXT FIT:
//=========================================
struct MemBlock* FirstNextFit ;
struct MemBlock *alloc_block_NF(uint32 size)
{
	//TODO: [PROJECT MS1 - BONUS] [DYNAMIC ALLOCATOR] alloc_block_NF
	// Write your code here, remove the panic and write your code
	panic("alloc_block_NF() is not implemented yet...!!");
//	struct MemBlock* node ;
//	struct MemBlock* node2 ;
//	struct MemBlock* node5 ;
//	struct MemBlock* node6 ;
//	node6 = LIST_FIRST(&FreeMemBlocksList);
//	node5 = LIST_LAST(&FreeMemBlocksList);
//	 if(FirstNextFit==node5)
//			{
//			//cprintf("-%d---%x",FirstNextFit->size,FirstNextFit->sva);
//				LIST_FOREACH(node,&FreeMemBlocksList)
//					{
//					if(node->sva<=FirstNextFit->sva)
//						{
//						if (node->size == size){
//								FirstNextFit = LIST_NEXT(node);
//								LIST_REMOVE(&FreeMemBlocksList, node);
//								return node;
//							  }
//							else if (node->size > size){
//								node2 = LIST_LAST(&AvailableMemBlocksList);
//								FirstNextFit = node;
//								node2->size=size;
//								node2->sva=node->sva;
//								node->sva=node->sva+size;
//								node->size=node->size-size;
//								LIST_REMOVE(&AvailableMemBlocksList, node2);
//								return node2;
//								  }
//						}
//					}
//			}
//	 else if(FirstNextFit!=NULL){
//		//cprintf("%d---%x--",FirstNextFit->size,FirstNextFit->sva);
//		LIST_FOREACH(node,&FreeMemBlocksList)
//			{
//				if(node->sva>=FirstNextFit->sva)
//				{
//					if (node==node5){
//						LIST_FOREACH(node,&FreeMemBlocksList)
//						{
//							if(node->sva<=FirstNextFit->sva)
//								{
//								if (node->size == size){
//									FirstNextFit = LIST_NEXT(node);
//									LIST_REMOVE(&FreeMemBlocksList, node);
//									return node;
//											  }
//								else if (node->size > size){
//									node2 = LIST_LAST(&AvailableMemBlocksList);
//									FirstNextFit = node;
//									node2->size=size;
//									node2->sva=node->sva;
//									node->sva=node->sva+size;
//									node->size=node->size-size;
//									LIST_REMOVE(&AvailableMemBlocksList, node2);
//									return node2;
//											  }
//								}
//						}
//					}
//					else if (node->size == size){
//						FirstNextFit = LIST_NEXT(node);
//						LIST_REMOVE(&FreeMemBlocksList, node);
//						return node;
//								  }
//					else if (node->size > size){
//						node2 = LIST_LAST(&AvailableMemBlocksList);
//						FirstNextFit = node;
//						node2->size=size;
//						node2->sva=node->sva;
//						node->sva=node->sva+size;
//						node->size=node->size-size;
//						LIST_REMOVE(&AvailableMemBlocksList, node2);
//						return node2;
//								  }
//					}
//				}
//			}
//	else {
//		LIST_FOREACH(node,&FreeMemBlocksList)
//			{
//			if (node->size == size){
//			FirstNextFit = LIST_NEXT(node);
//			LIST_REMOVE(&FreeMemBlocksList, node);
//			return node;
//					  }
//		else if (node->size > size){
//			node2 = LIST_LAST(&AvailableMemBlocksList);
//			FirstNextFit = node;
//			node2->size=size;
//			node2->sva=node->sva;
//			node->sva=node->sva+size;
//			node->size=node->size-size;
//			LIST_REMOVE(&AvailableMemBlocksList, node2);
//			return node2;
//					  }
//			 }
//		}
//				return NULL;
}
//				if(node->size==size)
//									{
//										 node3 =LIST_NEXT(node);
//										LIST_REMOVE(&FreeMemBlocksList, node);
//										return node;
//									}
//===================================================
// [8] INSERT BLOCK (SORTED WITH MERGE) IN FREE LIST:
//===================================================
void insert_sorted_with_merge_freeList(struct MemBlock *blockToInsert) {

		struct MemBlock *node;
		struct MemBlock *node3;
		struct MemBlock *node4;
		struct MemBlock *nextnode;
		struct MemBlock *prevnode;
		struct MemBlock *nodeforitra = NULL;
		node4 =LIST_LAST(&FreeMemBlocksList);
		node3 = LIST_FIRST(&FreeMemBlocksList);
		if (LIST_SIZE(&FreeMemBlocksList) == 0) {
			LIST_INSERT_HEAD(&FreeMemBlocksList, blockToInsert);
		}
		else
		{
			LIST_FOREACH(node, &FreeMemBlocksList)
			{
				if (node->sva < blockToInsert->sva) {
					nodeforitra = node;
					prevnode = LIST_PREV(node);
					nextnode = LIST_NEXT(node);
				} else {
					break;
				}
			}
			if (nodeforitra == NULL) {
				if (node3->sva==blockToInsert->sva+blockToInsert->size) {
						node3->sva = blockToInsert->sva;
						node3->size = blockToInsert->size + node3->size;
						blockToInsert->size = 0;
						blockToInsert->sva = 0;
						LIST_INSERT_HEAD(&AvailableMemBlocksList, blockToInsert);
				}
				else
						LIST_INSERT_HEAD(&FreeMemBlocksList, blockToInsert);
			}
			else if (nextnode != NULL)
			{
				if (blockToInsert->sva == nodeforitra->sva + nodeforitra->size && blockToInsert->sva + blockToInsert->size == nextnode->sva) {
					nodeforitra->size = nodeforitra->size + blockToInsert->size + nextnode->size;
					blockToInsert->sva = 0;
					blockToInsert->size = 0;
					nextnode->sva = 0;
					nextnode->size = 0;
					LIST_REMOVE(&FreeMemBlocksList, nextnode);
					LIST_INSERT_HEAD(&AvailableMemBlocksList, blockToInsert);
					LIST_INSERT_HEAD(&AvailableMemBlocksList, nextnode);
				}
				else if (blockToInsert->sva == nodeforitra->sva + nodeforitra->size)
				{
					nodeforitra->size += blockToInsert->size;
					blockToInsert->sva = 0;
					blockToInsert->size = 0;
					LIST_INSERT_HEAD(&AvailableMemBlocksList, blockToInsert);
				}
				else if (blockToInsert->sva + blockToInsert->size == nextnode->sva)
				{
					nextnode->sva = blockToInsert->sva;
					nextnode->size += blockToInsert->size;
					blockToInsert->sva = 0;
					blockToInsert->size = 0;
					LIST_INSERT_HEAD(&AvailableMemBlocksList, blockToInsert);
				}
				else
				{
					LIST_PREV(nextnode)=blockToInsert;
					LIST_NEXT(blockToInsert) = nextnode;
					LIST_PREV(blockToInsert) = nodeforitra;
					LIST_NEXT(nodeforitra) = blockToInsert;
					FreeMemBlocksList.size++;
				}
			}
			else
			{
				if (blockToInsert->sva == nodeforitra->sva + nodeforitra->size) {
					nodeforitra->size =nodeforitra->size+ blockToInsert->size;
					blockToInsert->sva = 0;
					blockToInsert->size = 0;
					LIST_INSERT_HEAD(&AvailableMemBlocksList, blockToInsert);
				}
				else
					LIST_INSERT_TAIL(&FreeMemBlocksList, blockToInsert);
			}
		}

//	struct MemBlock* node ;
//			struct MemBlock* node2;
//			struct MemBlock* node3;
//			struct MemBlock* node4;
//			struct MemBlock* node5;
//			struct MemBlock* nextnode;
//			struct MemBlock* prevnode;
//			node4 =LIST_LAST(&FreeMemBlocksList);
//		    node3 = LIST_FIRST(&FreeMemBlocksList);
//
//			if (LIST_SIZE(&FreeMemBlocksList)==0)//S1
//				{
//					LIST_INSERT_HEAD(&FreeMemBlocksList,blockToInsert);
//				}
//			else if (blockToInsert->sva < node3->sva)
//				{
//				if(node3->sva==blockToInsert->sva+blockToInsert->size){
//							node3->sva =blockToInsert->sva;
//							node3->size =node3->size+blockToInsert->size;
//							blockToInsert->sva = 0;
//							blockToInsert->size = 0;
//							LIST_INSERT_HEAD(&AvailableMemBlocksList,blockToInsert);
//						}
//						else
//						LIST_INSERT_HEAD(&FreeMemBlocksList,blockToInsert);
//				}
//			else if (blockToInsert->sva>node4->sva)
//				{
//				    if(node4->sva+node4->size==blockToInsert->sva){
//				    	node4->size =node4->size+blockToInsert->size;
//						blockToInsert->sva = 0;
//						blockToInsert->size = 0;
//						LIST_INSERT_HEAD(&AvailableMemBlocksList,blockToInsert);
//				    }
//				    else
//					LIST_INSERT_TAIL(&FreeMemBlocksList,blockToInsert);
//				}
//			else
//				{
//				LIST_FOREACH(node,&FreeMemBlocksList)
//					{
//						prevnode = node->prev_next_info.le_prev;
//						nextnode = node->prev_next_info.le_next;
//						if(node->sva>blockToInsert->size)
//						{
//							if(prevnode->sva + prevnode->size<blockToInsert->sva && node->sva >blockToInsert->sva+blockToInsert->size)
//							{
//								LIST_INSERT_BEFORE(&FreeMemBlocksList,node,blockToInsert);
//							}
//							else if(prevnode->sva + prevnode->size==blockToInsert->sva && node->sva ==blockToInsert->sva+blockToInsert->size)
//							{
//								prevnode->size=prevnode->size+blockToInsert->size+node->size;
//								blockToInsert->sva = 0;
//								blockToInsert->size = 0;
//								node->size=0;
//								node->sva=0;
//								LIST_REMOVE(&FreeMemBlocksList,node);
//								LIST_INSERT_TAIL(&AvailableMemBlocksList,blockToInsert);
//								LIST_INSERT_HEAD(&AvailableMemBlocksList,node);
//							}
//							else if(node->sva ==blockToInsert->sva+blockToInsert->size)
//							{
//								node->sva=blockToInsert->sva;
//								node->size =node->size+blockToInsert->size;
//								blockToInsert->sva = 0;
//								blockToInsert->size = 0;
//								LIST_INSERT_HEAD(&AvailableMemBlocksList,blockToInsert);
//							}
//							else if (prevnode->size+prevnode->sva ==blockToInsert->sva ){
//								prevnode->size = prevnode->size+blockToInsert->size;
//								blockToInsert->sva = 0;
//								blockToInsert->size = 0;
//								LIST_INSERT_HEAD(&AvailableMemBlocksList,blockToInsert);
//							}
//						}
//					}
//				}




//	if (node2 == NULL) {
//				uint32 endOfBlock = blockToInsert->sva + blockToInsert->size;
//				if (endOfBlock == node3->sva) {
//					node3->sva = blockToInsert->sva;
//					node3->size = blockToInsert->size + node3->size;
//					blockToInsert->size = 0;
//					blockToInsert->sva = 0;
//					LIST_INSERT_HEAD(&AvailableMemBlocksList, blockToInsert);
//				} else {
//					LIST_INSERT_HEAD(&FreeMemBlocksList, blockToInsert);
//				}
//			}  else {
//				uint32 endOfPrev = node2->sva + node2->size;
//
//				if (blockToInsert->sva == endOfPrev) {
//					node2->size += blockToInsert->size;
//					blockToInsert->sva = 0;
//					blockToInsert->size = 0;
//					LIST_INSERT_HEAD(&AvailableMemBlocksList, blockToInsert);
//				} else {
//					LIST_INSERT_TAIL(&FreeMemBlocksList, blockToInsert);
//				}
//			}
}
