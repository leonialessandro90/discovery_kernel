#include "list.h"
#include "task.h"

void * list_round_shift(list ** lst)
{
	list * iterator = *lst;

	if (*lst == null)
		return null;
	if((*lst)->next == null)
		return (*lst)->elem;

	while(iterator->next != null)
		iterator = iterator->next;

	iterator->next = *lst;
	*lst = (*lst)->next;
	iterator->next->next = null;

	return (*lst)->elem;
}

void * list_remove_tail(list ** lst)
{
	list * iterator = *lst;

	if(*lst == null)
		return null;

	if((*lst)->next == null) {
		mem_free(*lst);
		*lst = null;
		return null;
	}

	while(iterator->next->next != null)
		iterator = iterator->next;
	mem_free(iterator->next);
	iterator->next = null;
	return iterator;
}

void list_insert_tail(list ** lst, void * elem)
{
	list * new_block;
	register list * iterator = *lst;

	new_block = mem_alloc(sizeof(list));
	new_block->elem = elem;
	new_block->next = null;
	if (*lst == null) {
		*lst = new_block;
	} else {
		while (iterator->next != null)
			iterator = iterator->next;
		iterator->next = new_block;
	}
}

void * list_remove_head(list ** lst)
{
	void * content;
	list * head = *lst;
	if (head == null)
		return null;
	content = head->elem;
	*lst = (*lst)->next;
	mem_free(head);
	return content;
}

uint8_t list_count(list * lst){
	list* iterator = lst;
	uint8_t count = 0;
	while(iterator != null){
		iterator=iterator->next;
		count++;
	}
	return count;
}

void * list_getDescriptor(uint8_t index, list * lst){
	list* iterator = lst;
	uint8_t count = 0;
	if ( lst==null || index<0 || index>list_count(lst))
		return null;
	while(iterator != null && count<=index){
		if (count == index)
			return iterator->elem;
		iterator=iterator->next;
		count++;
	}
	return null;
}


void list_Transfer(list ** ready, list ** waiting, uint8_t index){
	list * pre_iterator = *ready;
	list * iterator = *waiting;
	list * selected = *waiting;
	uint8_t i=0;
	uint8_t n = list_count(*waiting);
	if (*waiting == null || index > n)
		return;
	if (index==0)
		*waiting = (*waiting)->next;
	else{
		while (i<index){
			iterator = selected;
			selected=selected->next;
			i++;
		}
		iterator->next=selected->next;
		iterator = *ready;
	}



	iterator = *ready;
	if (*ready == null) {
			*ready = selected;
			selected->next = null;
	}
	else {
		while ((iterator->next != null) &&
				((((des_task_block *)(iterator->elem))->period)<(((des_task_block*)(selected->elem))->period))){
			pre_iterator = iterator;
			iterator = iterator->next;
		}
		if (iterator->next == null &&
				((((des_task_block *)(iterator->elem))->period)<(((des_task_block*)(selected->elem))->period))){
			iterator->next=selected;
			selected->next=null;
		}
		else{
			if (iterator==*ready){
				selected->next = *ready;
				*ready=selected;
			}
			else{
				pre_iterator->next=selected;
				selected->next=iterator;
			}
		}
	}
}

void periodic_list_insert(list ** lst, void * elem)
{
	list * new_block;
	list * iterator = *lst;
	list * pre_iterator = *lst;

	new_block = mem_alloc(sizeof(list));
	new_block->elem = elem;
	new_block->next = null;
	if (*lst == null) {
		*lst = new_block;
	} else {
		while ((iterator->next != null) &&
				((((des_task_block *)(iterator->elem))->period)<(((des_task_block*)elem)->period))){
			pre_iterator = iterator;
			iterator = iterator->next;
		}
		if (iterator->next == null &&
				((((des_task_block *)(iterator->elem))->period)<(((des_task_block*)elem)->period)))
				iterator->next=new_block;
		else{
			if (iterator==*lst){
				new_block->next = *lst;
				*lst=new_block;
			}
			else{
				pre_iterator->next=new_block;
				new_block->next=iterator;
			}

		}

	}
}

void priority_list_insert(list ** lst, void * elem)
{
	list * new_block;
	list * iterator = *lst;
	list * pre_iterator = *lst;

	new_block = mem_alloc(sizeof(list));
	new_block->elem = elem;
	new_block->next = null;
	if (*lst == null) {
		*lst = new_block;
	} else {
		while ((iterator->next != null) &&
				((((des_task_block *)(iterator->elem))->priority)<(((des_task_block*)elem)->priority))){
			pre_iterator = iterator;
			iterator = iterator->next;
		}
		if (iterator->next == null &&
				((((des_task_block *)(iterator->elem))->priority)<(((des_task_block*)elem)->priority)))
				iterator->next=new_block;
		else{
			if (iterator==*lst){
				new_block->next = *lst;
				*lst=new_block;
			}
			else{
				pre_iterator->next=new_block;
				new_block->next=iterator;
			}

		}

	}
}
