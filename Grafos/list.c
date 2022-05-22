#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "list.h"

struct node {

    type_elem elem;
    struct node * next; 
};
    
list empty(void){

	list l = NULL;
	return l;
}

list addl(list l, type_elem e){

	struct node * p = malloc(sizeof(struct node));
	p->elem = e;
	p->next = l;
	l = p;
	return l;
}

void destroy(list l){

	if(l!=NULL){
		
		struct node *p = NULL;
		
		while(l!=NULL){
			
			p = l;
			l = l->next;
			free(p);
			p = NULL;
		}
	}	
}

bool is_empty(list l){

	bool b = (l == NULL);
	return b;
}

type_elem head(list l){

	assert(!is_empty(l));
	type_elem e;
	e = l->elem;
	return e;
}

list tail(list l){

	assert(!is_empty(l));
	struct node * p = l;
	l = l->next;
	free(p);
	p = NULL;
	return l;
}

list addr(list l, type_elem e){

	struct node *p = NULL;
	struct node *q = malloc(sizeof(struct node));
	q->elem = e;
	q->next = NULL;
	
	if(!is_empty(l)){

		p = l;
		
		while(p->next != NULL){

			p = p->next;
		}
		p->next = q;
	}	
	else{

		l = q;
	}

	return l;
}

unsigned int length(list l){

	unsigned int n = 0u;
	struct node *p = l;

	while(p != NULL){

		n++;
		p = p->next;
	}

	return n;
}

list concat(list l1, list l2){ 
	
	struct node *p = l2;

	for (unsigned int i = 0u; i < length(l2); ++i){

		l1 = addr(l1,p->elem);
		p = p->next;
	}

	return l1;	
}

type_elem index(list l, unsigned int n){

	assert(length(l) > n);

	type_elem e;
	struct node *p = l;

	for(unsigned int i=0; i < n; i++){

		p = p->next;
	}
	
	e = p->elem;
	return e;
}

list take(list l, unsigned int n){

	assert(length(l) > n);

		struct node *p = l;
		struct node *q = NULL;
		
		for(unsigned int i=0u; i<n; i++){

			p = p->next;
		}

		if(p->next != NULL){
			
			q = p->next;
			p->next = NULL;

			while (q!=NULL){

				p = q;
				q = q->next;
				free(p);
				p = NULL;
			}	
		}
		else{

			l = NULL;

			while(p!=NULL){

				q = p;
				p = p->next;
				free(q);
				q = NULL;
			}
		}

	return l;
}

list drop(list l, unsigned int n){

	assert(length(l) > n);
	struct node *p= NULL;

	for(unsigned int i=0u; i < n; i++){

			p = l;
			l = l->next;
			free(p); 
			p = NULL;
		}

	return l;	
}

list copy_list(list l1, list l2){

	if(l1==NULL){

		l2 = NULL;
	}
	else{

		struct node *p = l1;

		for(unsigned int i=0u; i < length(l1); i++){

			l2 = addr(l2,p->elem);
			p = p->next;
		}
	}

	return l2;
}
