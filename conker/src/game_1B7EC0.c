#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* A fixed-capacity FIFO of values with optional per-value release callbacks. */


struct QueueNode {
    EffectHeader header;
    s32 value;
    QueueNode *older;     /* towards the tail */
    QueueNode *newer;     /* towards the head */
    u8 releaseFunc;       /* index into D_8008D5C0, 0 = none */
};

struct Queue {
    EffectHeader header;
    QueueNode *head;      /* newest node */
    QueueNode *tail;      /* oldest node */
    s32 capacity;
    s32 count;
    s16 lifetime;         /* timer reload value on each push */
    s16 timer;
    u8 flags;             /* bit 0: timer counts down */
};

extern void (*D_8008D5C0[])(s32);


/* Ticks the queue timer; once it has expired, drops the oldest node each call. */
void func_1518AA10(Queue *arg) {
    Queue *queue;
    QueueNode *node;

    queue = arg; // matching: IDO allocates registers differently without this param copy
    if (queue->count != 0) {
        if (queue->timer < 0) {
            node = queue->tail;
            if (node->newer == NULL) {
                queue->head = NULL;
                queue->tail = NULL;
            } else {
                node->newer->older = NULL;
                queue->tail = node->newer;
            }
            if (node->releaseFunc != 0) {
                D_8008D5C0[node->releaseFunc](node->value);
            }
            func_1516972C(&node->header);
            queue->count--;
        } else if (queue->flags & 1) {
            queue->timer -= D_800BE9E4;
        }
    }
}

/* Allocates an empty queue. */
Queue *func_1518AADC(s32 capacity, s16 lifetime, u8 flags) {
    Queue *queue;

    queue = func_15167A68(0x1D, 0, sizeof(Queue), 1, 0xFF, 1);
    if (queue == NULL) {
        return NULL;
    }
    queue->count = 0;
    queue->capacity = capacity;
    queue->timer = lifetime;
    queue->lifetime = lifetime;
    queue->head = NULL;
    queue->tail = NULL;
    queue->flags = flags;
    return queue;
}

/* Allocates an unlinked queue node. */
QueueNode *func_1518AB60(s32 value, u8 releaseFunc) {
    QueueNode *node;

    node = func_15167A68(0x1E, 0, sizeof(QueueNode), 1, 0xFF, 1);
    if (node == NULL) {
        return NULL;
    }
    node->value = value;
    node->releaseFunc = releaseFunc;
    node->older = NULL;
    node->newer = NULL;
    return node;
}

/*
 * Pushes a value at the head of the queue, dropping the oldest node when over capacity.
 * Returns 0 (after releasing the value) if it could not be queued.
 */
s32 func_1518ABD0(Queue *queue, s32 value, u8 releaseFunc) {
    QueueNode *node;

    if (queue == NULL) {
        if (releaseFunc != 0) {
            D_8008D5C0[releaseFunc](value);
        }
        return 0;
    }

    node = func_1518AB60(value, releaseFunc);
    if (node == NULL) {
        if (releaseFunc != 0) {
            D_8008D5C0[releaseFunc](value);
        }
        return 0;
    }

    node->older = queue->head;
    if (node->older != NULL) {
        node->older->newer = node;
    } else {
        queue->tail = node;
    }
    queue->head = node;
    node->newer = NULL;

    queue->timer = queue->lifetime;
    queue->count++;
    if (queue->capacity < queue->count) {
        node = queue->tail;
        queue->count--;
        if (node->newer == NULL) {
            queue->head = NULL;
            queue->tail = NULL;
        } else {
            node->newer->older = NULL;
            queue->tail = node->newer;
        }
        if (node->releaseFunc != 0) {
            D_8008D5C0[node->releaseFunc](node->value);
        }
        func_1516972C(&node->header);
    }

    return 1;
}
