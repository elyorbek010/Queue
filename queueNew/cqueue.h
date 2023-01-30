#ifndef CQUEUE_H
#define CQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DEBUG 0

typedef struct cqueue_s cqueue_t;

typedef enum cqueue_ret_e
{
	CQUEUE_SUCCESS = 0,
	CQUEUE_FAILURE = 1,
	CQUEUE_OVERFLOW,
	CQUEUE_UNDERFLOW,
	CQUEUE_EMPTY,
	CQUEUE_FULL,
	CQUEUE_NONEMPTY
} cqueue_ret_t;

/**
 * Creates a circular queue with `capacity` elements at most.
 */
cqueue_t* queue_create(size_t capacity);

/**
 * Destroys the entire cqueue with all its elements.
 */
cqueue_ret_t queue_destroy(cqueue_t* cqueue);

/**
 * Adds an element to the cqueue.
 *
 * If the cqueue is full, CQUEUE_OVERFLOW is returned
 * and `element` is written in place of the first element.
 */
cqueue_ret_t queue_push_end(cqueue_t* cqueue, void* element);

/**
 * Adds an element to the cqueue.
 *
 * If the cqueue is full, CQUEUE_OVERFLOW is returned
 * and `element` is written in place of the last element.
 */
cqueue_ret_t queue_push_begin(cqueue_t* cqueue, void* element);

/**
 * Removes an element from the cqueue.
 */
cqueue_ret_t queue_pop_begin(cqueue_t* cqueue, void** p_element);

/**
 * Removes an element from the cqueue.
 */
cqueue_ret_t queue_pop_end(cqueue_t* cqueue, void** p_element);

/**
 * Peeks the first element at the front of the queue.
 */
cqueue_ret_t queue_peek_begin(const cqueue_t* cqueue, void** p_element);

/**
 * Peeks the last element at the end of the queue.
 */
cqueue_ret_t queue_peek_end(const cqueue_t* cqueue, void** p_element);

/**
 * Returns CQUEUE_FULL if queue is full, CQUEUE_NOT_FULL otherwise
 */
cqueue_ret_t queue_status(const cqueue_t* cqueue);

#endif

