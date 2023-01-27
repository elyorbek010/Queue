#ifndef CQUEUE_H
#define CQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct cqueue_s cqueue_t;

typedef enum cqueue_ret_e
{
	CQUEUE_SUCCESS = 0,
	CQUEUE_OVERFLOW,
	CQUEUE_UNDERFLOW
} cqueue_ret_t;

/**
 * Creates a circular queue with `capacity` elements at most.
 */
cqueue_t* queue_create(size_t capacity);

/**
 * Destroies the entire cqueue with all its elements.
 */
void queue_destroy(cqueue_t* cqueue);

/**
 * Adds an element to the cqueue.
 *
 * If the cqueue is full, CQUEUE_OVERFLOW is returned
 * and `element` is written in place of the last element.
 */
cqueue_ret_t queue_push_begin(cqueue_t* cqueue, int element);

/**
 * Removes an element from the cqueue.
 */
cqueue_ret_t queue_pop_end(cqueue_t* cqueue, int* p_element);

/**
 * Peeks the first element at the front of the queue.
 */
cqueue_ret_t queue_peek(cqueue_t* cqueue, int* p_element);

#endif

