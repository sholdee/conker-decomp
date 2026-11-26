#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* A fixed-capacity FIFO of values with optional per-value release callbacks. */


struct QueueNode {
    EffectHeader header;
    void *value;
    QueueNode *older;     /* towards the tail */
    QueueNode *newer;     /* towards the head */
    u8 releaseFunc;       /* index into gQueueReleaseFuncs, 0 = none */
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

/* Release callbacks; declared without a prototype, as each takes its own value type. */
static void (*gQueueReleaseFuncs[])() = { NULL, effect_startDecalFade, func_150CBF5C };


/* Ticks the queue timer; once it has expired, drops the oldest node each call. */
void effect_updateQueue(Queue *arg) {
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
                gQueueReleaseFuncs[node->releaseFunc](node->value);
            }
            effect_destroyObject(&node->header);
            queue->count--;
        } else if (queue->flags & 1) {
            queue->timer -= D_800BE9E4;
        }
    }
}

/* Allocates an empty queue. */
Queue *effect_newQueue(s32 capacity, s16 lifetime, u8 flags) {
    Queue *queue;

    queue = effect_newObject(0x1D, 0, sizeof(Queue), 1, 0xFF, 1);
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
QueueNode *effect_newQueueNode(void *value, u8 releaseFunc) {
    QueueNode *node;

    node = effect_newObject(0x1E, 0, sizeof(QueueNode), 1, 0xFF, 1);
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
s32 effect_pushQueue(Queue *queue, void *value, u8 releaseFunc) {
    QueueNode *node;

    if (queue == NULL) {
        if (releaseFunc != 0) {
            gQueueReleaseFuncs[releaseFunc](value);
        }
        return 0;
    }

    node = effect_newQueueNode(value, releaseFunc);
    if (node == NULL) {
        if (releaseFunc != 0) {
            gQueueReleaseFuncs[releaseFunc](value);
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
            gQueueReleaseFuncs[node->releaseFunc](node->value);
        }
        effect_destroyObject(&node->header);
    }

    return 1;
}
