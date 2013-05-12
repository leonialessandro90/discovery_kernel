#include "memory.h"

void mem_init()
{
	register int i;
	mem_free_blocks = DYNAMIC_MEMORY_BLOCK_NUM;
	for (i=0; i<DYNAMIC_MEMORY_BLOCK_NUM; i++)
		mem_main_memory[i].status = FREE;
}

void * mem_alloc(uint32_t size)
{
	register int i;
	if (size>DYNAMIC_MEMORY_BLOCK_CONTENT_SIZE || mem_free_blocks == 0)
		return 0;
	mem_free_blocks--;
	for (i=0; mem_main_memory[i].status != FREE; i++);
	mem_main_memory[i].status = NOT_FREE;
	return &(mem_main_memory[i].content);
}

void mem_free(void * address)
{
	register int i;
	for (i=0; i<DYNAMIC_MEMORY_BLOCK_NUM; i++) {
		if (&(mem_main_memory[i].content) == address) {
			mem_main_memory[i].status = FREE;
			mem_free_blocks++;
			return;
		}
	}
}
