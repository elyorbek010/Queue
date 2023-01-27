#include <gtest/gtest.h>

extern "C" {
#include "../queueNew/cqueue.h"
}

TEST(SmokeTest, pushPeekPop)
{
    cqueue_t* queue = NULL;

    int var1 = 10;
    int var2 = 0;

    queue = queue_create(5);

    EXPECT_EQ(queue_push_end(queue, var1), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_peek(queue, &var2), CQUEUE_SUCCESS);

    EXPECT_EQ(var1, var2);

    EXPECT_EQ(queue_pop_begin(queue, &var2), CQUEUE_SUCCESS);
    EXPECT_EQ(var1, var2);

    queue_destroy(queue);
}

TEST(FullQueue, pushReturnsOverflow)
{
    cqueue_t* queue = NULL;

    queue = queue_create(3);

    ASSERT_EQ(queue_push_end(queue, 1), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 2), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 3), CQUEUE_SUCCESS);

    EXPECT_EQ(queue_push_end(queue, 4), CQUEUE_OVERFLOW);

    queue_destroy(queue);
}

TEST(FullQueue, pushOverwritesAtFront)
{
    cqueue_t* queue = NULL;
    int val;

    queue = queue_create(3);

    ASSERT_EQ(queue_push_end(queue, 1), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 2), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 3), CQUEUE_SUCCESS);

    ASSERT_EQ(queue_push_end(queue, 4), CQUEUE_OVERFLOW);

    ASSERT_EQ(queue_pop_begin(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_pop_begin(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_pop_begin(queue, &val), CQUEUE_SUCCESS);
    EXPECT_EQ(val, 4);

    queue_destroy(queue);
}

TEST(EmptyQueue, popReturnsUnderflow)
{
    cqueue_t* queue = NULL;
    int val = 101;

    queue = queue_create(3);

    EXPECT_EQ(queue_pop_begin(queue, &val), CQUEUE_UNDERFLOW);

    queue_destroy(queue);
}

TEST(EmptyQueue, popUnderflowDoesNotChangeValueOfArgument)
{
    cqueue_t* queue = NULL;
    int val = 101;

    queue = queue_create(3);

    ASSERT_EQ(queue_pop_begin(queue, &val), CQUEUE_UNDERFLOW);
    EXPECT_EQ(val, 101);

    queue_destroy(queue);
}

TEST(ReuseAfterUnderflow, pushPop)
{
    cqueue_t* queue = NULL;
    int val = 101;

    queue = queue_create(3);

    ASSERT_EQ(queue_push_end(queue, 101), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_pop_begin(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_pop_begin(queue, &val), CQUEUE_UNDERFLOW);
    EXPECT_EQ(queue_push_end(queue, 101), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_pop_begin(queue, &val), CQUEUE_SUCCESS);
    EXPECT_EQ(val, 101);

    queue_destroy(queue);
}

TEST(EmptyQueue, peekReturnsUnderflow)
{
    cqueue_t* queue = NULL;
    int val = 101;

    queue = queue_create(3);

    EXPECT_EQ(queue_peek(queue, &val), CQUEUE_UNDERFLOW);

    queue_destroy(queue);
}

TEST(EmptyQueue, peekUnderflowDoesNotChangeValueOfArgument)
{
    cqueue_t* queue = NULL;
    int val = 101;

    queue = queue_create(3);

    ASSERT_EQ(queue_peek(queue, &val), CQUEUE_UNDERFLOW);
    EXPECT_EQ(val, 101);

    queue_destroy(queue);
}

TEST(Circulation, queue_capacity_1)
{
    cqueue_t* queue = NULL;
    int val;
    
    queue = queue_create(1);

    ASSERT_EQ(queue_push_end(queue, 101), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_pop_begin(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(val, 101);

    EXPECT_EQ(queue_push_end(queue, 202), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_pop_begin(queue, &val), CQUEUE_SUCCESS);
    EXPECT_EQ(val, 202);

    queue_destroy(queue);
}

TEST(Circulation, queue_capacity_2)
{
    cqueue_t* queue = NULL;
    int val;

    queue = queue_create(2);

    ASSERT_EQ(queue_push_end(queue, 101), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 202), CQUEUE_SUCCESS);

    ASSERT_EQ(queue_pop_begin(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(val, 101);

    EXPECT_EQ(queue_push_end(queue, 303), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_pop_begin(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(val, 202);
    EXPECT_EQ(queue_pop_begin(queue, &val), CQUEUE_SUCCESS);
    EXPECT_EQ(val, 303);

    queue_destroy(queue);
}

TEST(Circulation, queue_capacity_3)
{
    cqueue_t* queue = NULL;
    int val;

    queue = queue_create(3);

    ASSERT_EQ(queue_push_end(queue, 101), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 202), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 303), CQUEUE_SUCCESS);

    ASSERT_EQ(queue_pop_begin(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(val, 101);

    EXPECT_EQ(queue_push_end(queue, 404), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_pop_begin(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(val, 202);
    EXPECT_EQ(queue_pop_begin(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(val, 303);
    EXPECT_EQ(queue_pop_begin(queue, &val), CQUEUE_SUCCESS);
    EXPECT_EQ(val, 404);

    queue_destroy(queue);
}

TEST(Circulation, queue_capacity_4)
{
    cqueue_t* queue = NULL;
    int val;

    queue = queue_create(4);

    ASSERT_EQ(queue_push_end(queue, 101), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 202), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 303), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 404), CQUEUE_SUCCESS);

    ASSERT_EQ(queue_pop_begin(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(val, 101);

    EXPECT_EQ(queue_push_end(queue, 505), CQUEUE_SUCCESS);

    EXPECT_EQ(queue_pop_begin(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(val, 202);
    EXPECT_EQ(queue_pop_begin(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(val, 303);
    EXPECT_EQ(queue_pop_begin(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(val, 404);
    EXPECT_EQ(queue_pop_begin(queue, &val), CQUEUE_SUCCESS);
    EXPECT_EQ(val, 505);

    queue_destroy(queue);
}


TEST(isFull, EmptyQueueCapacity1) {
    cqueue_t* queue = NULL;

    queue = queue_create(1);
    EXPECT_EQ(is_full(queue), false);

    queue_destroy(queue);
}

TEST(isFull, FullQueueCapacity1) {
    cqueue_t* queue = NULL;
    int var = 101;

    queue = queue_create(1);

    ASSERT_EQ(queue_push_end(queue, 101), CQUEUE_SUCCESS);
    EXPECT_EQ(is_full(queue), true);
    ASSERT_EQ(queue_pop_begin(queue, &var), CQUEUE_SUCCESS);
    EXPECT_EQ(is_full(queue), false);

    queue_destroy(queue);
}

TEST(isFull, EmptyQueue) {
    cqueue_t* queue = NULL;

    queue = queue_create(5);

    EXPECT_EQ(is_full(queue), false);

    queue_destroy(queue);
}

TEST(isFull, HalfFullQueue) {
    cqueue_t* queue = NULL;

    queue = queue_create(5);

    ASSERT_EQ(queue_push_end(queue, 101), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 202), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 303), CQUEUE_SUCCESS);

    EXPECT_EQ(is_full(queue), false);

    queue_destroy(queue);
}

TEST(isFull, FullQueue) {
    cqueue_t* queue = NULL;

    queue = queue_create(5);

    ASSERT_EQ(queue_push_end(queue, 101), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 202), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 303), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 404), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 505), CQUEUE_SUCCESS);

    EXPECT_EQ(is_full(queue), true);

    queue_destroy(queue);
}

/////////////////////////////////////////
TEST(isEmpty, EmptyQueueCapacity1) {
    cqueue_t* queue = NULL;

    queue = queue_create(1);
    EXPECT_EQ(is_empty(queue), true);

    queue_destroy(queue);
}

TEST(isEmpty, FullQueueCapacity1) {
    cqueue_t* queue = NULL;
    int var = 101;

    queue = queue_create(1);

    ASSERT_EQ(queue_push_end(queue, 101), CQUEUE_SUCCESS);
    EXPECT_EQ(is_empty(queue), false);
    ASSERT_EQ(queue_pop_begin(queue, &var), CQUEUE_SUCCESS);
    EXPECT_EQ(is_empty(queue), true);

    queue_destroy(queue);
}

TEST(isEmpty, EmptyQueue) {
    cqueue_t* queue = NULL;

    queue = queue_create(5);

    EXPECT_EQ(is_empty(queue), true);

    queue_destroy(queue);
}

TEST(isEmpty, HalfFullQueue) {
    cqueue_t* queue = NULL;

    queue = queue_create(5);

    ASSERT_EQ(queue_push_end(queue, 101), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 202), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 303), CQUEUE_SUCCESS);

    EXPECT_EQ(is_empty(queue), false);

    queue_destroy(queue);
}

TEST(isEmpty, FullQueue) {
    cqueue_t* queue = NULL;

    queue = queue_create(5);

    ASSERT_EQ(queue_push_end(queue, 101), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 202), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 303), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 404), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push_end(queue, 505), CQUEUE_SUCCESS);

    EXPECT_EQ(is_empty(queue), false);

    queue_destroy(queue);
}