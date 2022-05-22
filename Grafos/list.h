#ifndef _LIST_H
#define _LIST_H

#include <stdbool.h>
#include <stdio.h>

typedef struct node * list;
typedef unsigned int type_elem;


/* Constructors */

list empty(void);

/* create new list */

list addl(list l, type_elem e);

/* add the element 'e' the start of list */

void destroy(list l);

/* free memory if necessary */

/* Operations */

bool is_empty(list l);

/* return true if l is empty, false case contrary */

type_elem head(list l);

/* PRE: l !is_empty() 
   return first element of the list */

list tail(list l);

/*  PRE: l !is_empty()
	removes the first element */

list addr(list l, type_elem e);

/* add the element 'e' the to end of the list */

unsigned int length(list l);

/* return the size of the list */

list concat(list l1, list l2);

/* Add to the end of l1 all the elements of l2 in the same order*/

type_elem index(list l, unsigned int n);

/* PRE: length(l) > n 
   return the element of the position n */ 

list take(list l, unsigned int n);

/* removes the elements greater than or equal a n */

list drop(list l, unsigned int n);

/* removes the elements minors to n  */

list copy_list(list l1, list l2);

/* copy the elments de l1 of the list l2 */


#endif