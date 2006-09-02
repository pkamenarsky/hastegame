/*
==============================================================================
Lucifer Engine source code
Copyright (c) 2001 - 2002 by Morgoth

******************************************************************************
IMPORTANT:

This engine is under an absolutely secret devlopment. If you have a copy
of its source (e.g. you can read this), you are strongly encouraged to
destroy it as fast as you can. In case that you don't, you are breaking
applicable international laws, and your life will most likely be in
great danger.
******************************************************************************

llist.h -- linked list implementation
==============================================================================
*/

#ifndef _LLIST_H_
#define _LLIST_H_

#define	INIT_LIST(list)			list = NULL;

#define ADD_TO_LIST(list, new)	(new).next = list; \
								(new).prev = NULL; \
								if(list) (list)->prev = &(new); \
								(list) = &(new);

#define REMOVE_FROM_LIST(list, rem)	if((rem).prev) (rem).prev->next = (rem).next; \
									if((rem).next) (rem).next->prev = (rem).prev; \
									if((list) == &(rem)) (list) = (rem).next;

#define FOR_EACH_IN_LIST(list, c)	for(c = (list); c; c = c->next)

#endif
