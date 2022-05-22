#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdbool.h>
#include "GrafoSt21.h"


typedef struct q_queue * queue;


queue queue_empty(void);

queue queue_enqueue(queue q, u32 elem);

size_t queue_size(queue q);

bool queue_is_empty(queue q);

u32 queue_first(queue q);

queue queue_dequeue(queue q);

queue queue_free(queue q);

#endif