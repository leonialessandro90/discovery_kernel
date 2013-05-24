#include "memory.h"
#include "task.h"

void mem_init()
{
	register uint32_t i;
	mem_dbg_init();
	mem_free_blocks = DYNAMIC_MEMORY_BLOCK_NUM;
	for (i=0; i<DYNAMIC_MEMORY_BLOCK_NUM; i++)
	{
		mem_status[i].status = FREE;
		mem_status[i].num_allocated = 0;
	}
}

void * mem_alloc(uint32_t size)
{
	asm("CPSID I");
	uint32_t i;
	uint32_t j;
	uint32_t k;
	int32_t size_remaining;


	for(i=0; i <  DYNAMIC_MEMORY_BLOCK_NUM; i++)
	{
		if(mem_status[i].status == FREE)
		{
			if(size <= DYNAMIC_MEMORY_BLOCK_CONTENT_SIZE)
			{
				mem_free_blocks--;
				mem_status[i].num_allocated = NOT_FREE;
				mem_status[i].num_allocated = 1;
				asm("CPSIE I");
				return &(mem_dynamic[i].block);

			}

			j = i + 1;
			size_remaining = size;
			while(size_remaining > 0)
			{
				if(mem_status[j].status == FREE)
				{
					size_remaining -= DYNAMIC_MEMORY_BLOCK_CONTENT_SIZE;
				}
				else
				{
					i = j;
					break;
				}
				j++;
			}
			if(size_remaining <= 0)
			{
				for(k=i; k < j; k++)
				{
					mem_status[k].status = NOT_FREE;
					mem_free_blocks--;
					mem_status[i].num_allocated++;
				}
				asm("CPSIE I");
				return &(mem_dynamic[i].block);
			}
		}
	}
	asm("CPSIE I");
	return 0;

	/*
	if (size < DYNAMIC_MEMORY_SMALL_BLOCK_CONTENT_SIZE) {

	} else {
		if (size>DYNAMIC_MEMORY_BLOCK_CONTENT_SIZE*4 || mem_free_blocks == 0) {
			if (mem_debug) mem_dbg_add(running->id, MEM_ALLOCATION, null, null, null, MEM_FULL);
			asm("CPSIE I");
			return 0;
		}
		mem_free_blocks--;
		for (i=0; mem_main_memory[i].status != FREE; i++);
		if (mem_debug) mem_dbg_add(running->id, MEM_ALLOCATION, &(mem_main_memory[i].content), i, mem_main_memory[i].status, MEM_OK);
		mem_main_memory[i].status = NOT_FREE;
		asm("CPSIE I");
		return &(mem_main_memory[i].content);
	}
	*/
}

void mem_free(void * address)
{
	asm("CPSID I");
	register uint32_t i;
	uint32_t j;
	if (mem_debug) mem_dbg_add(running->id, MEM_DEALLOCATION, address, null, null, null);
	for (i=0; i<DYNAMIC_MEMORY_BLOCK_NUM; i++) {
		if (&(mem_dynamic[i].block) == address) {
			//if (mem_debug) mem_dbg_add(running->id, MEM_DEALLOCATION, address, i, mem_main_memory[i].status, MEM_OK);
			for (j = 0; j < mem_status[i].num_allocated; j++)
			{
				mem_status[i + j].status = FREE;
				mem_free_blocks++;
			}
			mem_status[i].num_allocated = 0;

			asm("CPSIE I");
			return;
		}
	}
	if (mem_debug) mem_dbg_add(running->id, MEM_DEALLOCATION, address, null, null, MEM_ADDR_NOT_FOUND);
	asm("CPSIE I");
}

void mem_dbg_add(uint8_t task_id,
		mem_dbg mem_operation,
		void * address,
		uint32_t block,
		mem_block_status block_status,
		mem_dbg mem_result)
{
	mem_dbg_array[mem_dbg_array_pointer].valid = mem_dbg_valid;
	mem_dbg_array[mem_dbg_array_pointer].task_id = task_id;
	mem_dbg_array[mem_dbg_array_pointer].mem_operation = mem_operation;
	mem_dbg_array[mem_dbg_array_pointer].address = address;
	mem_dbg_array[mem_dbg_array_pointer].block = block;
	mem_dbg_array[mem_dbg_array_pointer].block_status = block_status;
	mem_dbg_array[mem_dbg_array_pointer].mem_result = mem_result;
	mem_dbg_array_pointer++;
	if (mem_dbg_array_pointer == MEM_DBG_ARRAY_SIZE) {
		mem_dbg_array_pointer = 0;
		mem_dbg_valid++;
	}
}

void mem_dbg_init()
{
	mem_dbg_valid = 1;
	mem_debug = 0;
	mem_dbg_array_pointer = 0;
}
