#include "cqueue.h"

static inline void inc_index(size_t* index, size_t capacity) {
    *index = (*index + 1) % (capacity + 1);
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
    if (capacity == 0)
    {
        fprintf(stderr, "Invalid capacity 0");
        return NULL;
    }

    cqueue_t* cqueue = malloc(sizeof(cqueue_t) + (capacity + 1) * sizeof(int));
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
    if (cqueue == NULL) {
        fprintf(stderr, "Queue does not exist");
        return;
    }
    free(cqueue);
}

cqueue_ret_t queue_push(cqueue_t* cqueue, int element)
{
    if (cqueue == NULL) {
        fprintf(stderr, "Queue does not exist");
        return CQUEUE_FAILURE;
    }
    
    cqueue->element[cqueue->end] = element;
    inc_index(&(cqueue->end), cqueue->capacity);

    if (cqueue->begin == cqueue->end) {
        inc_index(&(cqueue->begin), cqueue->capacity);
        return CQUEUE_OVERFLOW;
    }
    return CQUEUE_SUCCESS;
}

cqueue_ret_t queue_pop(cqueue_t* cqueue, int* p_element) {
    if (cqueue == NULL) {
        fprintf(stderr, "Queue does not exist");
        return CQUEUE_FAILURE;
    }

    if (cqueue->begin == cqueue->end) {
        return CQUEUE_UNDERFLOW;
    }

    *p_element = cqueue->element[cqueue->begin];
    inc_index(&(cqueue->begin), cqueue->capacity);
    return CQUEUE_SUCCESS;
}

cqueue_ret_t queue_peek(cqueue_t* cqueue, int* p_element) {
    if (cqueue == NULL) {
        fprintf(stderr, "Queue does not exist");
        return CQUEUE_FAILURE;
    }
    
    if (cqueue->begin == cqueue->end) {
        return CQUEUE_UNDERFLOW;
    }

    *p_element = cqueue->element[cqueue->begin];
    return CQUEUE_SUCCESS;
}