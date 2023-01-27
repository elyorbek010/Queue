#include "cqueue.h"

static inline size_t queue_next_index(size_t index, size_t capacity) {
    return (index + 1) % (capacity + 1);
}

struct cqueue_s
{
    size_t capacity;
    size_t begin;
    size_t end;
    int element[];
};

cqueue_t* queue_create(size_t capacity)
{
    cqueue_t* cqueue = malloc(sizeof(*cqueue) + (capacity + 1) * sizeof(cqueue->element[0]));
    if (cqueue == NULL) {
        fprintf(stderr, "Not enough memory for capacity: %zd", capacity);
        return NULL;
    }
    cqueue->capacity = capacity;
    cqueue->begin = cqueue->end = 0;

    return cqueue;
}

void queue_destroy(cqueue_t* cqueue)
{
    free(cqueue);
}

cqueue_ret_t queue_push_end(cqueue_t* cqueue, int element)
{   
    cqueue->element[cqueue->end] = element;
    cqueue->end = queue_next_index(cqueue->end, cqueue->capacity);

    if (cqueue->begin == cqueue->end) {
        cqueue->begin = queue_next_index(cqueue->begin, cqueue->capacity);
        return CQUEUE_OVERFLOW;
    }
    return CQUEUE_SUCCESS;
}

cqueue_ret_t queue_pop_begin(cqueue_t* cqueue, int* p_element) {
    if (cqueue->begin == cqueue->end) {
        return CQUEUE_UNDERFLOW;
    }

    *p_element = cqueue->element[cqueue->begin];
    cqueue->begin = queue_next_index(cqueue->begin, cqueue->capacity);
    return CQUEUE_SUCCESS;
}

cqueue_ret_t queue_peek(cqueue_t* cqueue, int* p_element) {
    if (cqueue->begin == cqueue->end) {
        return CQUEUE_UNDERFLOW;
    }

    *p_element = cqueue->element[cqueue->begin];
    return CQUEUE_SUCCESS;
}

bool queue_is_full(cqueue_t* cqueue) {
    if (queue_next_index(cqueue->end, cqueue->capacity) == cqueue->begin) {
        return true;
    }
    return false;
}

bool queue_is_empty(cqueue_t* cqueue) {
    if (cqueue->begin == cqueue->end) {
        return true;
    }
    return false;
}