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

typedef struct mem_block_properties_t {
	mem_block_status status;
	uint8_t num_allocated;
} mem_block_properties;


typedef struct mem_block_t {
	uint8_t block[DYNAMIC_MEMORY_BLOCK_CONTENT_SIZE];
} mem_block;


mem_block mem_dynamic[DYNAMIC_MEMORY_BLOCK_NUM];
mem_block_properties mem_status[DYNAMIC_MEMORY_BLOCK_NUM];




uint32_t mem_free_blocks;

void mem_init();
void * mem_alloc(uint32_t size);
void mem_free(void * address);


/**********************
 *                    *
 *  Debugging Stuff   *
 *                    *
 *********************/


typedef enum mem_dbg_t {
	MEM_OK = null,
	MEM_ALLOCATION,
	MEM_DEALLOCATION,
	MEM_FULL,
	MEM_ADDR_NOT_FOUND
} mem_dbg;

typedef struct mem_dbg_data_t {
	uint8_t valid;
	uint8_t task_id;
	mem_dbg mem_operation;
	void * address;
	uint32_t block;
	mem_block_status block_status;
	mem_dbg mem_result;
} mem_dbg_data;

#define MEM_DBG_ARRAY_SIZE 260
mem_dbg_data mem_dbg_array[MEM_DBG_ARRAY_SIZE];
uint16_t mem_dbg_array_pointer;
uint16_t mem_dbg_valid;
uint8_t mem_debug;

void mem_dbg_init();
void mem_dbg_add(uint8_t task_id,
		mem_dbg mem_operation,
		void * address,
		uint32_t block,
		mem_block_status block_status,
		mem_dbg mem_result);

#endif /* MEMORY_H_ */
