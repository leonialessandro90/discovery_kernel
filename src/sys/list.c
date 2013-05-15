#include "list.h"

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
