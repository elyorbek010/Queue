#include <gtest/gtest.h>

extern "C" {
#include "../queueNew/cqueue.h"
}


TEST(NullArguments, allFunctions) {
    cqueue_t* queue;
    
    queue = queue_create(5);

    int val = 0;
    void * ptr;

    EXPECT_EQ(queue_destroy(nullptr), CQUEUE_FAILURE);
    EXPECT_EQ(queue_push_end(nullptr, &val), CQUEUE_FAILURE);
    EXPECT_EQ(queue_push_begin(nullptr, &val), CQUEUE_FAILURE);
    EXPECT_EQ(queue_pop_begin(nullptr, &ptr), CQUEUE_FAILURE);
    EXPECT_EQ(queue_pop_end(nullptr, &ptr), CQUEUE_FAILURE);
    EXPECT_EQ(queue_peek_begin(nullptr, &ptr), CQUEUE_FAILURE);
    EXPECT_EQ(queue_status(nullptr), CQUEUE_FAILURE);
    EXPECT_EQ(queue_status(nullptr), CQUEUE_FAILURE);

    queue_destroy(queue);
}

TEST(SmokeTest, pushPeekPop)
{
    cqueue_t* queue = nullptr;

    int var1 = 10;
    void* var2;

    queue = queue_create(5);

    EXPECT_EQ(queue_push_end(queue, &var1), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_peek_begin(queue, &var2), CQUEUE_SUCCESS);

    EXPECT_EQ(&var1, var2);
    var2 = NULL;

    EXPECT_EQ(queue_pop_begin(queue, &var2), CQUEUE_SUCCESS);
    EXPECT_EQ(&var1, var2);

    queue_destroy(queue);
}

TEST(FullQueue, pushReturnsOverflow)
{
    double var1 = 1;

    cqueue_t* queue = nullptr;

    queue = queue_create(3);

    ASSERT_EQ(queue_push_end(queue, &var1 + 0), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &var1 + 1), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &var1 + 2), CQUEUE_SUCCESS);

    EXPECT_EQ(queue_push_end(queue, &var1 + 3), CQUEUE_OVERFLOW);

    queue_destroy(queue);
}

TEST(FullQueue, pushOverwritesAtFront)
{
    cqueue_t* queue = nullptr;
    float var1 = 5;
    void* var2;

    queue = queue_create(3);

    ASSERT_EQ(queue_push_end(queue, &var1 + 0), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &var1 + 1), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &var1 + 2), CQUEUE_SUCCESS);

    ASSERT_EQ(queue_push_end(queue, &var1 + 3), CQUEUE_OVERFLOW);

    ASSERT_EQ(queue_pop_begin(queue, &var2), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_pop_begin(queue, &var2), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_pop_begin(queue, &var2), CQUEUE_SUCCESS);
    EXPECT_EQ(var2, &var1 + 3);

    queue_destroy(queue);
}

TEST(EmptyQueue, popReturnsUnderflow)
{
    cqueue_t* queue = nullptr;
    void* ptr;

    queue = queue_create(3);

    EXPECT_EQ(queue_pop_begin(queue, &ptr), CQUEUE_UNDERFLOW);

    queue_destroy(queue);
}

TEST(EmptyQueue, popUnderflowDoesNotChangeValueOfArgument)
{
    cqueue_t* queue = nullptr;
    void* ptr = nullptr;

    queue = queue_create(3);

    ASSERT_EQ(queue_pop_begin(queue, &ptr), CQUEUE_UNDERFLOW);
    EXPECT_EQ(ptr, nullptr);

    queue_destroy(queue);
}

TEST(ReuseAfterUnderflow, pushPop)
{
    cqueue_t* queue = nullptr;
    int val = 101;
    void* ptr;

    queue = queue_create(3);

    ASSERT_EQ(queue_push_end(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_pop_begin(queue, &ptr), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_pop_begin(queue, &ptr), CQUEUE_UNDERFLOW);
    EXPECT_EQ(queue_push_end(queue, &val), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_pop_begin(queue, &ptr), CQUEUE_SUCCESS);
    EXPECT_EQ(&val, ptr);

    queue_destroy(queue);
}

TEST(EmptyQueue, peekReturnsUnderflow)
{
    cqueue_t* queue = nullptr;
    void*ptr;

    queue = queue_create(3);

    EXPECT_EQ(queue_peek_begin(queue, &ptr), CQUEUE_UNDERFLOW);

    queue_destroy(queue);
}

TEST(EmptyQueue, peekUnderflowDoesNotChangeValueOfArgument)
{
    cqueue_t* queue = nullptr;
    int val = 101;
    void* ptr = &val;

    queue = queue_create(3);

    ASSERT_EQ(queue_peek_begin(queue, &ptr), CQUEUE_UNDERFLOW);
    EXPECT_EQ(ptr, &val);

    queue_destroy(queue);
}

TEST(Circulation, queue_capacity_1)
{
    cqueue_t* queue = nullptr;
    int val = 101;
    void* ptr;
    
    queue = queue_create(1);

    ASSERT_EQ(queue_push_end(queue, &val + 0), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_pop_begin(queue, &ptr), CQUEUE_SUCCESS);
    ASSERT_EQ(&val, ptr);

    EXPECT_EQ(queue_push_end(queue, &val + 1), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_pop_begin(queue, &ptr), CQUEUE_SUCCESS);
    EXPECT_EQ(&val + 1, ptr);

    queue_destroy(queue);
}

TEST(Circulation, queue_capacity_2)
{
    cqueue_t* queue = nullptr;
    int val = 101;
    void* ptr;

    queue = queue_create(2);

    ASSERT_EQ(queue_push_end(queue, &val + 0), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 1), CQUEUE_SUCCESS);

    ASSERT_EQ(queue_pop_begin(queue, &ptr), CQUEUE_SUCCESS);
    ASSERT_EQ(&val, ptr);

    EXPECT_EQ(queue_push_end(queue, &val + 2), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_pop_begin(queue, &ptr), CQUEUE_SUCCESS);
    ASSERT_EQ(ptr, &val + 1);
    EXPECT_EQ(queue_pop_begin(queue, &ptr), CQUEUE_SUCCESS);
    EXPECT_EQ(ptr, &val + 2);

    queue_destroy(queue);
}

TEST(Circulation, queue_capacity_3)
{
    cqueue_t* queue = nullptr;
    int val = 101;
    void* ptr;

    queue = queue_create(3);

    ASSERT_EQ(queue_push_end(queue, &val + 0), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 1), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 2), CQUEUE_SUCCESS);

    ASSERT_EQ(queue_pop_begin(queue, &ptr), CQUEUE_SUCCESS);
    ASSERT_EQ(&val + 0, ptr);

    EXPECT_EQ(queue_push_end(queue, &val + 3), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_pop_begin(queue, &ptr), CQUEUE_SUCCESS);
    ASSERT_EQ(&val + 1, ptr);
    EXPECT_EQ(queue_pop_begin(queue, &ptr), CQUEUE_SUCCESS);
    ASSERT_EQ(&val + 2, ptr);
    EXPECT_EQ(queue_pop_begin(queue, &ptr), CQUEUE_SUCCESS);
    EXPECT_EQ(&val + 3, ptr);

    queue_destroy(queue);
}

TEST(Circulation, queue_capacity_4)
{
    cqueue_t* queue = nullptr;
    int val_i = 101;
    void* ptr_i;

    double val_d = 101.5;
    void* ptr_d;

    queue = queue_create(4);

    ASSERT_EQ(queue_push_end(queue, &val_i), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val_d), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val_i), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val_d), CQUEUE_SUCCESS);

    ASSERT_EQ(queue_pop_begin(queue, &ptr_i), CQUEUE_SUCCESS);
    ASSERT_EQ(&val_i, ptr_i);

    EXPECT_EQ(queue_push_end(queue, &val_i), CQUEUE_SUCCESS);

    EXPECT_EQ(queue_pop_begin(queue, &ptr_d), CQUEUE_SUCCESS);
    ASSERT_EQ(&val_d, ptr_d);
    EXPECT_EQ(queue_pop_begin(queue, &ptr_i), CQUEUE_SUCCESS);
    ASSERT_EQ(&val_i, ptr_i);
    EXPECT_EQ(queue_pop_begin(queue, &ptr_d), CQUEUE_SUCCESS);
    ASSERT_EQ(&val_d, ptr_d);
    EXPECT_EQ(queue_pop_begin(queue, &ptr_i), CQUEUE_SUCCESS);
    EXPECT_EQ(&val_i, ptr_i);

    queue_destroy(queue);
}


TEST(isFull, EmptyQueueCapacity1) {
    cqueue_t* queue = nullptr;

    queue = queue_create(1);
    EXPECT_EQ(queue_status(queue), CQUEUE_EMPTY);

    queue_destroy(queue);
}

TEST(isFull, FullQueueCapacity1) {
    cqueue_t* queue = nullptr;
    int var = 101;
    void* ptr;

    queue = queue_create(1);

    ASSERT_EQ(queue_push_end(queue, &var), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_status(queue), CQUEUE_FULL);
    ASSERT_EQ(queue_pop_begin(queue, &ptr), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_status(queue), CQUEUE_EMPTY);

    queue_destroy(queue);
}

TEST(isFull, EmptyQueue) {
    cqueue_t* queue = nullptr;

    queue = queue_create(5);

    EXPECT_EQ(queue_status(queue), CQUEUE_EMPTY);

    queue_destroy(queue);
}

TEST(isFull, HalfFullQueue) {
    cqueue_t* queue = nullptr;
    double val = 5.55;

    queue = queue_create(5);

    ASSERT_EQ(queue_push_end(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 1), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 2), CQUEUE_SUCCESS);

    EXPECT_EQ(queue_status(queue), CQUEUE_NONEMPTY);

    queue_destroy(queue);
}

TEST(isFull, FullQueue) {
    cqueue_t* queue = nullptr;
    char ch = 'a';

    queue = queue_create(5);

    ASSERT_EQ(queue_push_end(queue, &ch + 0), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &ch + 1), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &ch + 2), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &ch + 3), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &ch + 4), CQUEUE_SUCCESS);

    EXPECT_EQ(queue_status(queue), CQUEUE_FULL);

    queue_destroy(queue);
}

TEST(isEmpty, EmptyQueueCapacity1) {
    cqueue_t* queue = nullptr;

    queue = queue_create(1);
    EXPECT_EQ(queue_status(queue), CQUEUE_EMPTY);

    queue_destroy(queue);
}

TEST(isEmpty, FullQueueCapacity1) {
    cqueue_t* queue = nullptr;
    int var = 101;
    void* ptr;

    queue = queue_create(1);

    ASSERT_EQ(queue_push_end(queue, &var), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_status(queue), CQUEUE_FULL);
    ASSERT_EQ(queue_pop_begin(queue, &ptr), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_status(queue), CQUEUE_EMPTY);

    queue_destroy(queue);
}

TEST(isEmpty, EmptyQueue) {
    cqueue_t* queue = nullptr;

    queue = queue_create(5);

    EXPECT_EQ(queue_status(queue), CQUEUE_EMPTY);

    queue_destroy(queue);
}

TEST(isEmpty, HalfFullQueue) {
    cqueue_t* queue = nullptr;
    int val = 101;

    queue = queue_create(5);

    ASSERT_EQ(queue_push_end(queue, &val + 0), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 1), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 2), CQUEUE_SUCCESS);

    EXPECT_EQ(queue_status(queue), CQUEUE_NONEMPTY);

    queue_destroy(queue);
}

TEST(isEmpty, FullQueue) {
    cqueue_t* queue = nullptr;
    int val = 101;

    queue = queue_create(5);

    ASSERT_EQ(queue_push_end(queue, &val + 0), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 1), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 2), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 3), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 4), CQUEUE_SUCCESS);

    EXPECT_EQ(queue_status(queue), CQUEUE_FULL);

    queue_destroy(queue);
}

TEST(FullQueue, PopEnd) {
    cqueue_t* queue = nullptr;
    int val = 101;
    void* ptr;

    queue = queue_create(5);

    ASSERT_EQ(queue_push_end(queue, &val + 0), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 1), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 2), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 3), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 4), CQUEUE_SUCCESS);

    EXPECT_EQ(queue_pop_end(queue, &ptr), CQUEUE_SUCCESS);
    EXPECT_EQ(ptr, &val + 4);
    EXPECT_EQ(queue_pop_end(queue, &ptr), CQUEUE_SUCCESS);
    EXPECT_EQ(ptr, &val + 3);
    EXPECT_EQ(queue_pop_end(queue, &ptr), CQUEUE_SUCCESS);
    EXPECT_EQ(ptr, &val + 2);
    EXPECT_EQ(queue_pop_end(queue, &ptr), CQUEUE_SUCCESS);
    EXPECT_EQ(ptr, &val + 1);
    EXPECT_EQ(queue_pop_end(queue, &ptr), CQUEUE_SUCCESS);
    EXPECT_EQ(ptr, &val + 0);

    EXPECT_EQ(queue_pop_end(queue, &ptr), CQUEUE_UNDERFLOW);
    EXPECT_EQ(ptr, &val + 0);

    queue_destroy(queue);
}

TEST(OverflowQueue, PopEnd) {
    cqueue_t* queue = nullptr;
    int val;
    void* ptr;

    queue = queue_create(3);

    ASSERT_EQ(queue_push_end(queue, &val + 0), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 1), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 2), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 3), CQUEUE_OVERFLOW);

    EXPECT_EQ(queue_pop_end(queue, &ptr), CQUEUE_SUCCESS);
    EXPECT_EQ(ptr, &val + 3);
    EXPECT_EQ(queue_pop_end(queue, &ptr), CQUEUE_SUCCESS);
    EXPECT_EQ(ptr, &val + 2);
    EXPECT_EQ(queue_pop_end(queue, &ptr), CQUEUE_SUCCESS);
    EXPECT_EQ(ptr, &val + 1);
    EXPECT_EQ(queue_pop_end(queue, &ptr), CQUEUE_UNDERFLOW);
    EXPECT_EQ(ptr, &val + 1);

    queue_destroy(queue);
}

TEST(EmptyQueue, pushBegin) {
    cqueue_t* queue = nullptr;
    int val;
    void* ptr;

    queue = queue_create(3);

    EXPECT_EQ(queue_push_begin(queue, &val), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_peek_begin(queue, &ptr), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_pop_begin(queue, &ptr), CQUEUE_SUCCESS);

    queue_destroy(queue);
}

TEST(FullQueue, pushBegin) {
    cqueue_t* queue = nullptr;
    int val;
    void* ptr;

    queue = queue_create(3);

    ASSERT_EQ(queue_push_end(queue, &val + 0), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 1), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 2), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_push_begin(queue, &val + 3), CQUEUE_OVERFLOW);
    EXPECT_EQ(queue_pop_begin(queue, &ptr), CQUEUE_SUCCESS);
    EXPECT_EQ(&val + 3, ptr);

    queue_destroy(queue);
}


TEST(EmptyQueue, popEnd) {
    cqueue_t* queue = nullptr;
    void* ptr;

    queue = queue_create(5);

    EXPECT_EQ(queue_pop_end(queue, &ptr), CQUEUE_UNDERFLOW);

    queue_destroy(queue);
}

TEST(FullQueue, popEnd) {
    cqueue_t* queue = nullptr;
    int val;
    void* ptr;

    queue = queue_create(3);

    ASSERT_EQ(queue_push_end(queue, &val + 0), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 1), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, &val + 2), CQUEUE_SUCCESS);

    EXPECT_EQ(queue_push_begin(queue, &val + 3), CQUEUE_OVERFLOW);
    EXPECT_EQ(queue_pop_end(queue, &ptr), CQUEUE_SUCCESS);
    EXPECT_EQ(&val + 1, (int *) ptr);

    queue_destroy(queue);
}

TEST(PeekBeginPeekEnd, pushBeginPushEnd) {
    cqueue_t* queue = nullptr;
    int val = 101;
    void* ptr;

    queue = queue_create(3);

    ASSERT_EQ(queue_push_end(queue, &val + 0), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_begin(queue, &val + 1), CQUEUE_SUCCESS);

    EXPECT_EQ(queue_peek_end(queue, &ptr), CQUEUE_SUCCESS);
    EXPECT_EQ(&val + 0, ptr);

    EXPECT_EQ(queue_peek_begin(queue, &ptr), CQUEUE_SUCCESS);
    EXPECT_EQ(&val + 1, ptr);

    queue_destroy(queue);
}