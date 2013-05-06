#include "list.h"

void * list_round_shift(list ** lst)
{
	list * last = *lst;
	list * head = *lst;

	if (*lst == null)
		return null;

	while (iterator->next != null)
		iterator = iterator->next;

	iterator->next = *lst;
	iterator->next->next = null;

	if ((*lst)->next != null)
		*lst = (*lst)->next;
	return head->elem;
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
