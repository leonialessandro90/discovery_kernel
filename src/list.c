#include "list.h"

void list_insert(list ** lst, void * elem)
{
	if (*lst == 0) {
		*lst = malloc(sizeof(list));
		(*lst)->elem = elem;
		(*lst)->next = null;
	} else {
		while ((*lst)->next != 0)
			*lst = (*lst)->next;
		(*lst)->next = malloc(sizeof(list));
		(*lst)->next->elem = elem;
		(*lst)->next->next = null;
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
