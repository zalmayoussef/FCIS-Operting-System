/*
 * chunk_operations.c
 *
 *  Created on: Oct 12, 2022
 *      Author: HP
 */

#include <kern/trap/fault_handler.h>
#include <kern/disk/pagefile_manager.h>
#include "kheap.h"
#include "memory_manager.h"


/******************************/
/*[1] RAM CHUNKS MANIPULATION */
/******************************/

//===============================
// 1) CUT-PASTE PAGES IN RAM:
//===============================
//This function should cut-paste the given number of pages from source_va to dest_va
//if the page table at any destination page in the range is not exist, it should create it
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int cut_paste_pages(uint32* page_directory, uint32 source_va, uint32 dest_va, uint32 num_of_pages)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] cut_paste_pages
		// Write your code here, remove the panic and write your code
		//panic("cut_paste_pages() is not implemented yet...!!");
	uint32 start_s_addr=source_va;
	uint32 size=PAGE_SIZE*num_of_pages;
	uint32 end_s_addr= source_va + size;
	uint32 start_d_addr=dest_va;
	//start_s_addr = ROUNDDOWN(start_s_addr,PAGE_SIZE);
	//end_s_addr =ROUNDUP(end_s_addr,PAGE_SIZE);
	uint32 end_d_addr =dest_va+size ;
	//uint32 start_d_addr = ROUNDUP(dest_va,PAGE_SIZE);
uint32 sd=start_d_addr;
int i=0;
while(i<num_of_pages)
{

uint32 * ptr_page_table = NULL;
struct FrameInfo* ptr_frame_info = get_frame_info(page_directory,sd,&ptr_page_table);
if(ptr_frame_info!=NULL)
	{return -1;}
i++;
sd+=PAGE_SIZE;
}


for(uint32 i=start_d_addr;i<end_d_addr;i+=PAGE_SIZE)
{
		uint32 * ptr_page_table = NULL;
		int rett= get_page_table(page_directory,i,&ptr_page_table);
		if(rett!=TABLE_IN_MEMORY)
		{
			create_page_table(page_directory ,i);
		}

}

	uint32 s_src=start_s_addr;
	uint32 s_d=start_d_addr;
	for(int i=0;i<num_of_pages;i++)
	{
		uint32 * ptr_page_table = NULL;
		struct FrameInfo* ptr_frame_info = get_frame_info(page_directory,s_src,&ptr_page_table);
		uint32 perm=ptr_page_table[PTX(s_src)]&(0x00000FFF);
		map_frame(page_directory,ptr_frame_info,s_d,perm);
		unmap_frame(page_directory,s_src);
		s_src+=PAGE_SIZE;
		s_d+=PAGE_SIZE;

	}


	return 0;
}

//===============================
// 2) COPY-PASTE RANGE IN RAM:
//===============================
//This function should copy-paste the given size from source_va to dest_va
//if the page table at any destination page in the range is not exist, it should create it
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int copy_paste_chunk(uint32* page_directory, uint32 source_va, uint32 dest_va, uint32 size)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] copy_paste_chunk
	// Write your code here, remove the panic and write your code
	//panic("copy_paste_chunk() is not implemented yet...!!");

	uint32 start_src=source_va;
	uint32 start_dest= dest_va;
	uint32 end_src= source_va + size;
	uint32 end_dest =dest_va+size;
    //uint32 sdd=start_d_addr;
	uint32 j=start_src;
    for(int i=start_dest;i<end_dest;i+=PAGE_SIZE)
    {
		uint32 * ptr_page_table = NULL;
		//uint32 * ptr_frame_table = NULL;
		struct FrameInfo* ptr_frame_info=get_frame_info(page_directory,i,&ptr_page_table);
		int ret_page=get_page_table(page_directory,i,&ptr_page_table);
	    //If ANY of the destination pages exists with READ ONLY permission,deny the entire process and return -1.

	    if(ptr_frame_info!=NULL)
	    {
		   uint32 perm=ptr_page_table[PTX(i)]&(0x00000FFF);
			perm=perm&PERM_WRITEABLE;
		   if(perm==0)
			{return -1;}
		}
	    //If the page table at any destination page in the range is not exist, it should create it

		if(ret_page!=TABLE_IN_MEMORY)
		{
			create_page_table(page_directory,i);
		}

        if(ptr_frame_info==NULL)
        {
			struct FrameInfo* ptr_frame_info2=get_frame_info(page_directory,j,&ptr_page_table);
		    int x=allocate_frame(&ptr_frame_info2);
		   // if(x==E_NO_MEM)return 0;
		    //else
		    //{
				uint32 perm2=ptr_page_table[PTX(j)]&(0x00000FFF);
				perm2=(perm2|PERM_WRITEABLE)&(perm2|PERM_USER);
				int re=map_frame(page_directory,ptr_frame_info2,i,perm2);
				//if(re==E_NO_MEM)
				//free_frame(ptr_frame_info2);

		    //}
         }

        j+=PAGE_SIZE;

     }
     uint8 *SRC_ADD=(uint8 *) source_va;
     uint8 *DST_ADD=(uint8 *) dest_va;
	 for(int i=0;i<size;i++)
	{
		*DST_ADD=*SRC_ADD;
		DST_ADD++;
		SRC_ADD++;
	}
/*for(uint32 i=start_d_addr;i<end_d_addr;i+=PAGE_SIZE)
{

	uint32 * ptr_page_table = NULL;
	struct FrameInfo* ptr_frame_info = get_frame_info(page_directory,i,&ptr_page_table);

}*/
    return 0;
}

//===============================
// 3) SHARE RANGE IN RAM:
//===============================
//This function should share the given size from dest_va with the source_va
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int share_chunk(uint32* page_directory, uint32 source_va,uint32 dest_va, uint32 size, uint32 perms)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] share_chunk
		// Write your code here, remove the panic and write your code
		//	panic("share_chunk() is not implemented yet...!!");
		    uint32 source_start_addr = source_va;
			uint32 source_end_addr= source_va + size;
			source_start_addr = ROUNDDOWN(source_start_addr,PAGE_SIZE);
			source_end_addr =ROUNDUP(source_end_addr,PAGE_SIZE);

			uint32 dest_start_addr = dest_va;
			uint32 dest_end_addr= dest_va + size;
			dest_start_addr = ROUNDDOWN(dest_start_addr,PAGE_SIZE);
			dest_end_addr =ROUNDUP(dest_end_addr,PAGE_SIZE);

			uint32 * ptr_page_table = NULL;
			uint32 * ptr_page_table_src = NULL;

				for (int i = source_start_addr,  j=dest_start_addr; i < source_end_addr
				&& j<dest_end_addr ;i+=PAGE_SIZE, j+=PAGE_SIZE) {

					struct FrameInfo* ptr_FrameInfo_source_iteration;
					ptr_FrameInfo_source_iteration= get_frame_info(page_directory,i,&ptr_page_table_src);

					int rett = get_page_table(page_directory, j, &ptr_page_table);

					if (rett == TABLE_IN_MEMORY){
							//check if page already exist:
							uint32 page_table_entry = ptr_page_table[PTX(j)];
							if ((page_table_entry & PERM_PRESENT) == PERM_PRESENT){
									return -1;
							}
					         else
								{
								  map_frame(page_directory,ptr_FrameInfo_source_iteration,j,perms);
								}
					}
					else {
						create_page_table(page_directory ,j);
						map_frame(page_directory,ptr_FrameInfo_source_iteration,j,perms);
					}
			}
			return 0;
}

//ptr_FrameInfo_dest_iteration ==ptr_FrameInfo_source_iteration ;

//===============================
// 4) ALLOCATE CHUNK IN RAM:
//===============================
//This function should allocate in RAM the given range [va, va+size)
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int allocate_chunk(uint32* page_directory, uint32 va, uint32 size, uint32 perms)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] allocate_chunk
	// Write your code here, remove the panic and write your code
	//panic("allocate_chunk() is not implemented yet...!!");
	struct FrameInfo* ptr_frame;
				uint32 start_addr = va;
				uint32 end_addr= va + size;
				start_addr = ROUNDDOWN(start_addr,PAGE_SIZE);
				end_addr =ROUNDUP(end_addr,PAGE_SIZE);
				uint32 * ptr_page_table = NULL;
				struct FrameInfo* ptr_frame_info = get_frame_info(page_directory,start_addr,&ptr_page_table);
				//if (ptr_frame_info != NULL){
				for (int i = start_addr; i < end_addr;i+=PAGE_SIZE)
					{
					struct FrameInfo* ptr_FrameInfo_iteration;
					//int  rettt = allocate_frame(&ptr_FrameInfo_iteration);
					int rett = get_page_table(page_directory, i, &ptr_page_table);
					if (rett == TABLE_IN_MEMORY){
						ptr_FrameInfo_iteration= get_frame_info(page_directory,i,&ptr_page_table);
						//check if page already exist:
						uint32 page_table_entry = ptr_page_table[PTX(i)];
						//If already mapped
						if ((page_table_entry & PERM_PRESENT) == PERM_PRESENT){
							if(ptr_FrameInfo_iteration->references!=0){
								return -1;
							}
						  else
							{
								allocate_frame(&ptr_FrameInfo_iteration);
								//ptr_FrameInfo_iteration->va = i;
								ptr_FrameInfo_iteration->references=ptr_FrameInfo_iteration->references +1;
								//map_frame(page_directory,ptr_FrameInfo_iteration,i,perms);
							}
							}
						else
						{
							allocate_frame(&ptr_FrameInfo_iteration);
							ptr_FrameInfo_iteration->va = i;
							map_frame(page_directory,ptr_FrameInfo_iteration,i,perms);
						}
					}
					else
					{
						create_page_table(page_directory ,i);
						allocate_frame(&ptr_FrameInfo_iteration);
						ptr_FrameInfo_iteration->va = i;
						map_frame(page_directory,ptr_FrameInfo_iteration,i,perms);
					}
					}
				return 0;
}
/*BONUS*/
//=====================================
// 5) CALCULATE ALLOCATED SPACE IN RAM:
//=====================================
void calculate_allocated_space(uint32* page_directory, uint32 sva, uint32 eva, uint32 *num_tables, uint32 *num_pages)
{
	//TODO: [PROJECT MS2 - BONUS] [CHUNK OPERATIONS] calculate_allocated_space
	// Write your code here, remove the panic and write your code
	panic("calculate_allocated_space() is not implemented yet...!!");
}

/*BONUS*/
//=====================================
// 6) CALCULATE REQUIRED FRAMES IN RAM:
//=====================================
// calculate_required_frames:
// calculates the new allocation size required for given address+size,
// we are not interested in knowing if pages or tables actually exist in memory or the page file,
// we are interested in knowing whether they are allocated or not.
uint32 calculate_required_frames(uint32* page_directory, uint32 sva, uint32 size)
{
	//TODO: [PROJECT MS2 - BONUS] [CHUNK OPERATIONS] calculate_required_frames
	// Write your code here, remove the panic and write your code
	panic("calculate_required_frames() is not implemented yet...!!");
}

//=================================================================================//
//===========================END RAM CHUNKS MANIPULATION ==========================//
//=================================================================================//

/*******************************/
/*[2] USER CHUNKS MANIPULATION */
/*******************************/

//======================================================
/// functions used for USER HEAP (malloc, free, ...)
//======================================================

//=====================================
// 1) ALLOCATE USER MEMORY:
//=====================================
void allocate_user_mem(struct Env* e, uint32 virtual_address, uint32 size)
{
	// Write your code here, remove the panic and write your code
	panic("allocate_user_mem() is not implemented yet...!!");
}

//=====================================
// 2) FREE USER MEMORY:
//=====================================
void free_user_mem(struct Env* e, uint32 virtual_address, uint32 size)
{
	//TODO: [PROJECT MS3] [USER HEAP - KERNEL SIDE] free_user_mem
	// Write your code here, remove the panic and write your code
	//panic("free_user_mem() is not implemented yet...!!");
	int num_of_pages = size / PAGE_SIZE ;
	uint32 address =virtual_address;

	for(int i=0 ; i<num_of_pages ; i++)
	{
		pf_remove_env_page(e,address);
		address = address + PAGE_SIZE ;
	}
	address = ROUNDDOWN(virtual_address,PAGE_SIZE);
	int max_size = e->page_WS_max_size;
	int end = ROUNDUP(virtual_address+size , PAGE_SIZE);
	for(int i = 0 ; i<max_size; i++)
	{
		uint32 addrss_in_ws = env_page_ws_get_virtual_address(e , i);
		if(addrss_in_ws >= address && addrss_in_ws < end)
		{
			unmap_frame(e->env_page_directory, addrss_in_ws);
			env_page_ws_clear_entry(e , i);
			e->page_last_WS_index=i;
		}
	}
	address = virtual_address ;
	uint32* ptr;
	for(int i=0 ; i<num_of_pages ; i++)
	{
		ptr = NULL;
		int flag = 0 ;
		get_page_table(e->env_page_directory ,address ,&ptr);
		if(ptr!=NULL)
		{
			for(int j = 0 ; j < 1024 ; j++)
			{
				if(ptr[j]!=0)
				{
					flag = 1 ;
					break;
				}
			}
			if(flag==0)
			{
				kfree((void*)ptr);
				e->env_page_directory[PDX(address)] = 0 ;
			}
		}
		address = address + PAGE_SIZE ;
	}
	//This function should:
	//1. Free ALL pages of the given range from the Page File
	//2. Free ONLY pages that are resident in the working set from the memory
	//3. Removes ONLY the empty page tables (i.e. not used) (no pages are mapped in the table)
}

//=====================================
// 2) FREE USER MEMORY (BUFFERING):
//=====================================
void __free_user_mem_with_buffering(struct Env* e, uint32 virtual_address, uint32 size)
{
	// your code is here, remove the panic and write your code
	panic("__free_user_mem_with_buffering() is not implemented yet...!!");

	//This function should:
	//1. Free ALL pages of the given range from the Page File
	//2. Free ONLY pages that are resident in the working set from the memory
	//3. Free any BUFFERED pages in the given range
	//4. Removes ONLY the empty page tables (i.e. not used) (no pages are mapped in the table)
}

//=====================================
// 3) MOVE USER MEMORY:
//=====================================
void move_user_mem(struct Env* e, uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
{
	//TODO: [PROJECT MS3 - BONUS] [USER HEAP - KERNEL SIDE] move_user_mem
	//your code is here, remove the panic and write your code
	panic("move_user_mem() is not implemented yet...!!");

	// This function should move all pages from "src_virtual_address" to "dst_virtual_address"
	// with the given size
	// After finished, the src_virtual_address must no longer be accessed/exist in either page file
	// or main memory

	/**/
}

//=================================================================================//
//========================== END USER CHUNKS MANIPULATION =========================//
//=================================================================================//

