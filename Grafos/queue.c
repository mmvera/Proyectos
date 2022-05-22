#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

#include "queue.h"
#include "list.h"

struct q_queue {

	list q_list;
	size_t lenght;
};

queue queue_empty(void){

	queue q = malloc(sizeof(struct q_queue));
	q->q_list = empty();
	q->lenght = 0;
	return q;
}

queue queue_enqueue(queue q, u32 elem){

	q->q_list = addr(q->q_list, elem);
	q->lenght = q->lenght + 1;
	return q;
}

size_t queue_size(queue q){
	return(q->lenght);
}

bool queue_is_empty(queue q){
	return(q->lenght == 0);
}

u32 queue_first(queue q){
	assert(!queue_is_empty(q));
	u32 elem;
	elem = head(q->q_list);
	return elem;
}

queue queue_dequeue(queue q){
	assert(!queue_is_empty(q));
	q->q_list = tail(q->q_list);
	q->lenght = q->lenght - 1;
	return q;
}

queue queue_free(queue q){
	destroy(q->q_list);
	free(q);
	q = NULL;
	return q;
}



