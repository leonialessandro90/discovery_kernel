#include "list.h"

void * list_round_shift(list ** lst)
{
	list * iterator = *lst;
	list * tail;

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

void list_delete_tail(list ** lst)
{
	list * iterator = *lst;

	if(*lst == null)
		return;

	if((*lst)->next == null )
		free(*lst);

	while(iterator->next->next != null)
		iterator = iterator->next;
	free(iterator->next);
	iterator->next = null;

}

void list_insert(list ** lst, void * elem)
{
	list * new_block;
	list * iterator = *lst;

	new_block = malloc(sizeof(list));
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

void * list_remove(list ** lst)
{
	void * ret;
	list * head = (*lst);
	if (*lst == null)
		return null;
	ret = (*lst)->elem;
	(*lst) = (*lst)->next;
	free(head);
	return ret;
}
