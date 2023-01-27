#ifndef CQUEUE_H
#define CQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct cqueue_s cqueue_t;

typedef enum cqueue_ret_e
{
	CQUEUE_SUCCESS = 0,
	CQUEUE_FAILURE,
	CQUEUE_OVERFLOW,
	CQUEUE_UNDERFLOW,
	CQUEUE_EMPTY,
	CQUEUE_NOT_EMPTY,
	CQUEUE_FULL,
	CQUEUE_NOT_FULL
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
 * and `element` is written in place of the first element.
 */
cqueue_ret_t queue_push_end(cqueue_t* cqueue, int element);

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
cqueue_ret_t queue_pop_begin(cqueue_t* cqueue, int* p_element);

/**
 * Peeks the first element at the front of the queue.
 */
cqueue_ret_t queue_peek(const cqueue_t* cqueue, int* p_element);

/**
 * Returns CQUEUE_FULL if queue is full, CQUEUE_NOT_FULL otherwise
 */
cqueue_ret_t queue_is_full(const cqueue_t* cqueue);

/**
 * Returns CQUEUE_EMPTY if queue is empty, CQUEUE_NOT_EMPTY otherwise
 */
cqueue_ret_t queue_is_empty(const cqueue_t* cqueue);

#endif

