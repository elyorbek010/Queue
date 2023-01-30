#include "cqueue.h"

#define DEBUG 0
#define debug_print(fmt, ...) \
            do { if (DEBUG == 1) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

static inline size_t queue_next_index(const size_t index, const size_t capacity) {
    return (index + 1) % (capacity + 1);
}

static inline size_t queue_prev_index(const size_t index, const size_t capacity) {
    if (index == 0) {
        return capacity;
    }
    else {
        return index - 1;
    }
}

struct cqueue_s
{
    size_t capacity;
    size_t begin;
    size_t end;
    void *element[];
};

cqueue_t* queue_create(const size_t capacity)
{
    cqueue_t* cqueue = malloc(sizeof(*cqueue) + (capacity + 1) * sizeof(cqueue->element[0]));
    if (cqueue == NULL) {
        #if (DEBUG == 1)
            debug_print("Not enough memory for capacity: %zd\n", capacity);
        #endif
        return NULL;
    }
    cqueue->capacity = capacity;
    cqueue->begin = cqueue->end = 0;

    return cqueue;
}

cqueue_ret_t queue_destroy(cqueue_t* cqueue)
{
    if (cqueue == NULL) {
        #if (DEBUG == 1)
            debug_print("Not valid queue\n");
        #endif
        return CQUEUE_FAILURE;
    }
    free(cqueue);
}

cqueue_ret_t queue_push_end(cqueue_t* cqueue, void* element)
{   
    if (cqueue == NULL) {
        #if (DEBUG == 1)
            debug_print("Queue does not exist\n");
        #endif
        return CQUEUE_FAILURE;
    }

    cqueue->element[cqueue->end] = element;
    cqueue->end = queue_next_index(cqueue->end, cqueue->capacity);

    if (cqueue->begin == cqueue->end) {
        cqueue->begin = queue_next_index(cqueue->begin, cqueue->capacity);
        return CQUEUE_OVERFLOW;
    }
    return CQUEUE_SUCCESS;
}

cqueue_ret_t queue_push_begin(cqueue_t* cqueue, void* element) {
    if (cqueue == NULL) {
        #if (DEBUG == 1)
            debug_print("Queue does not exist\n");
        #endif
        return CQUEUE_FAILURE;
    }

    cqueue->begin = queue_prev_index(cqueue->begin, cqueue->capacity);
    cqueue->element[cqueue->begin] = element;

    if (cqueue->begin == cqueue->end) {
        cqueue->end = queue_prev_index(cqueue->end, cqueue->capacity);
        return CQUEUE_OVERFLOW;
    }
    return CQUEUE_SUCCESS;
}

cqueue_ret_t queue_pop_begin(cqueue_t* cqueue, void** p_element) {
    if (cqueue == NULL) {
        #if (DEBUG == 1)
            debug_print("Queue does not exist\n");
        #endif
        return CQUEUE_FAILURE;
    }

    if (cqueue->begin == cqueue->end) {
        return CQUEUE_UNDERFLOW;
    }

    *p_element = cqueue->element[cqueue->begin];
    cqueue->begin = queue_next_index(cqueue->begin, cqueue->capacity);
    return CQUEUE_SUCCESS;
}

cqueue_ret_t queue_pop_end(cqueue_t* cqueue, void** p_element) {
    if (cqueue == NULL) {
        #if (DEBUG == 1)
            debug_print("Queue does not exist\n");
        #endif
        return CQUEUE_FAILURE;
    }

    if (cqueue->begin == cqueue->end) {
        return CQUEUE_UNDERFLOW;
    }

    cqueue->end = queue_prev_index(cqueue->end, cqueue->capacity);
    *p_element = cqueue->element[cqueue->end];
    return CQUEUE_SUCCESS;
}

cqueue_ret_t queue_peek_begin(const cqueue_t* cqueue, void** p_element) {
    if (cqueue == NULL) {
        #if (DEBUG == 1)
            debug_print("Queue does not exist\n");
        #endif
        return CQUEUE_FAILURE;
    }

    if (cqueue->begin == cqueue->end) {
        return CQUEUE_UNDERFLOW;
    }

    *p_element = cqueue->element[cqueue->begin];
    return CQUEUE_SUCCESS;
}

cqueue_ret_t queue_peek_end(const cqueue_t* cqueue, void** p_element) {
    if (cqueue == NULL) {
        #if (DEBUG == 1)
            debug_print("Queue does not exist\n");
        #endif
        return CQUEUE_FAILURE;
    }

    if (cqueue->begin == cqueue->end) {
        return CQUEUE_UNDERFLOW;
    }

    *p_element = cqueue->element[queue_prev_index(cqueue->end, cqueue->capacity)];
    return CQUEUE_SUCCESS;
}

cqueue_ret_t queue_status(const cqueue_t* cqueue) {
    if (cqueue == NULL) {
        #if (DEBUG == 1)
            debug_print("Queue does not exist\n");
        #endif
        return CQUEUE_FAILURE;
    }

    if (queue_next_index(cqueue->end, cqueue->capacity) == cqueue->begin) {
        return CQUEUE_FULL;
    }
    else if (cqueue->begin == cqueue->end) {
        return CQUEUE_EMPTY;
    }
    else {
        return CQUEUE_NONEMPTY;
    }
}