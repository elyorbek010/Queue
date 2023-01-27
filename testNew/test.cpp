#include "pch.h"

extern "C" {
#include "../queueNew/cqueue.h"
}

TEST(SmokeTest, pushPeekPop)
{
    cqueue_t* queue = NULL;

    int var1 = 10;
    int var2 = 0;

    queue = queue_create(5);

    EXPECT_EQ(queue_push(queue, var1), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_peek(queue, &var2), CQUEUE_SUCCESS);

    EXPECT_EQ(var1, var2);

    EXPECT_EQ(queue_pop(queue, &var2), CQUEUE_SUCCESS);
    EXPECT_EQ(var1, var2);

    queue_destroy(queue);
}

TEST(Capacity0, create)
{
    EXPECT_EQ(queue_create(0), (cqueue_t *)NULL);
}

TEST(FullQueue, pushReturnsOverflow)
{
    cqueue_t* queue = NULL;

    queue = queue_create(3);

    ASSERT_EQ(queue_push(queue, 1), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push(queue, 2), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push(queue, 3), CQUEUE_SUCCESS);

    EXPECT_EQ(queue_push(queue, 4), CQUEUE_OVERFLOW);

    queue_destroy(queue);
}

TEST(FullQueue, pushOverwritesAtFront)
{
    cqueue_t* queue = NULL;
    int val;

    queue = queue_create(3);

    ASSERT_EQ(queue_push(queue, 1), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push(queue, 2), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push(queue, 3), CQUEUE_SUCCESS);

    ASSERT_EQ(queue_push(queue, 4), CQUEUE_OVERFLOW);

    ASSERT_EQ(queue_pop(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_pop(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_pop(queue, &val), CQUEUE_SUCCESS);
    EXPECT_EQ(val, 4);

    queue_destroy(queue);
}

TEST(EmptyQueue, popReturnsUnderflow)
{
    cqueue_t* queue = NULL;
    int val = 101;

    queue = queue_create(3);

    EXPECT_EQ(queue_pop(queue, &val), CQUEUE_UNDERFLOW);
}

TEST(EmptyQueue, popUnderflowDoesNotChangeValueOfArgument)
{
    cqueue_t* queue = NULL;
    int val = 101;

    queue = queue_create(3);

    ASSERT_EQ(queue_pop(queue, &val), CQUEUE_UNDERFLOW);
    EXPECT_EQ(val, 101);
}

TEST(ReuseAfterUnderflow, pushPop)
{
    cqueue_t* queue;
    int val = 101;

    queue = queue_create(3);

    ASSERT_EQ(queue_push(queue, 101), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_pop(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_pop(queue, &val), CQUEUE_UNDERFLOW);
    EXPECT_EQ(queue_push(queue, 101), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_pop(queue, &val), CQUEUE_SUCCESS);
    EXPECT_EQ(val, 101);
}

TEST(EmptyQueue, peekReturnsUnderflow)
{
    cqueue_t* queue;
    int val = 101;

    queue = queue_create(3);

    EXPECT_EQ(queue_peek(queue, &val), CQUEUE_UNDERFLOW);
}

TEST(EmptyQueue, peekUnderflowDoesNotChangeValueOfArgument)
{
    cqueue_t* queue;
    int val = 101;

    queue = queue_create(3);

    ASSERT_EQ(queue_peek(queue, &val), CQUEUE_UNDERFLOW);
    EXPECT_EQ(val, 101);
}

TEST(Circulation, queue_capacity_1)
{
    int val;
    cqueue_t* queue;

    queue = queue_create(1);

    ASSERT_EQ(queue_push(queue, 101), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_pop(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(val, 101);

    EXPECT_EQ(queue_push(queue, 202), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_pop(queue, &val), CQUEUE_SUCCESS);
    EXPECT_EQ(val, 202);
}

TEST(Circulation, queue_capacity_2)
{
    cqueue_t* queue;
    int val;

    queue = queue_create(2);

    ASSERT_EQ(queue_push(queue, 101), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push(queue, 202), CQUEUE_SUCCESS);

    ASSERT_EQ(queue_pop(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(val, 101);

    EXPECT_EQ(queue_push(queue, 303), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_pop(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(val, 202);
    EXPECT_EQ(queue_pop(queue, &val), CQUEUE_SUCCESS);
    EXPECT_EQ(val, 303);
}

TEST(Circulation, queue_capacity_3)
{
    cqueue_t* queue;
    int val;

    queue = queue_create(3);

    ASSERT_EQ(queue_push(queue, 101), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push(queue, 202), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push(queue, 303), CQUEUE_SUCCESS);

    ASSERT_EQ(queue_pop(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(val, 101);

    EXPECT_EQ(queue_push(queue, 404), CQUEUE_SUCCESS);
    EXPECT_EQ(queue_pop(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(val, 202);
    EXPECT_EQ(queue_pop(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(val, 303);
    EXPECT_EQ(queue_pop(queue, &val), CQUEUE_SUCCESS);
    EXPECT_EQ(val, 404);
}

TEST(Circulation, queue_capacity_4)
{
    cqueue_t* queue;
    int val;

    queue = queue_create(4);

    ASSERT_EQ(queue_push(queue, 101), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push(queue, 202), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push(queue, 303), CQUEUE_SUCCESS);
    ASSERT_EQ(queue_push(queue, 404), CQUEUE_SUCCESS);

    ASSERT_EQ(queue_pop(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(val, 101);

    EXPECT_EQ(queue_push(queue, 505), CQUEUE_SUCCESS);

    EXPECT_EQ(queue_pop(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(val, 202);
    EXPECT_EQ(queue_pop(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(val, 303);
    EXPECT_EQ(queue_pop(queue, &val), CQUEUE_SUCCESS);
    ASSERT_EQ(val, 404);
    EXPECT_EQ(queue_pop(queue, &val), CQUEUE_SUCCESS);
    EXPECT_EQ(val, 505);
}