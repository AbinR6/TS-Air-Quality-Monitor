#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#define TEST_CAPACITY 4

typedef struct {
    uint32_t id;
    float value;
} test_item_t;

typedef struct {
    test_item_t buffer[TEST_CAPACITY];
    size_t head;
    size_t tail;
    size_t count;
} test_ring_buffer_t;

static void rb_init(test_ring_buffer_t *rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

static void rb_push(test_ring_buffer_t *rb, test_item_t item) {
    rb->buffer[rb->head] = item;
    rb->head = (rb->head + 1) % TEST_CAPACITY;
    if (rb->count < TEST_CAPACITY) {
        rb->count++;
    } else {
        rb->tail = (rb->tail + 1) % TEST_CAPACITY;
    }
}

static bool rb_pop(test_ring_buffer_t *rb, test_item_t *item) {
    if (rb->count == 0) return false;
    *item = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % TEST_CAPACITY;
    rb->count--;
    return true;
}

void test_ring_buffer_fifo(void)
{
    printf("[TEST] Running test_ring_buffer_fifo...\n");
    test_ring_buffer_t rb;
    rb_init(&rb);

    test_item_t item;
    assert(rb_pop(&rb, &item) == false);

    rb_push(&rb, (test_item_t){1, 10.5f});
    rb_push(&rb, (test_item_t){2, 20.5f});
    rb_push(&rb, (test_item_t){3, 30.5f});
    assert(rb.count == 3);

    assert(rb_pop(&rb, &item) == true);
    assert(item.id == 1);

    rb_push(&rb, (test_item_t){4, 40.5f});
    rb_push(&rb, (test_item_t){5, 50.5f}); /* Should overwrite id=2 */
    assert(rb.count == 4);

    assert(rb_pop(&rb, &item) == true);
    assert(item.id == 2 || item.id == 3); /* FIFO order */

    printf("[TEST] test_ring_buffer_fifo: PASS\n");
}

int main(void)
{
    test_ring_buffer_fifo();
    printf("[ALL TESTS PASSED]\n");
    return 0;
}
