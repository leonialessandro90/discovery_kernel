#ifndef MEMORY_H_
#define MEMORY_H_

#include "stm32f4xx.h"
#include "types.h"

/*

Super-Block
__________
|Block
__________
|Block
  ...
|Block
__________




Block
__________
status {free || not_free}
content ()


 */

typedef enum mem_block_status_t {
	FREE,
	NOT_FREE
} mem_block_status;

typedef struct mem_block_t {
	mem_block_status status;
	uint32_t content[DYNAMIC_MEMORY_BLOCK_CONTENT_SIZE/4];
} mem_block;

mem_block mem_main_memory[DYNAMIC_MEMORY_BLOCK_NUM];
uint32_t mem_free_blocks;

void mem_init();
void * mem_alloc(uint32_t size);
void mem_free(void * address);

#endif /* MEMORY_H_ */
