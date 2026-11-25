#include <ultra64.h>
#include "libc/stdarg.h"
#include "functions.h"
#include "variables.h"

/*
 * Variable-size per-id record, kept in the doubly linked list D_800D2F48 sorted by id.
 * The payload (dataSize bytes) lives inside the same allocation at dataOffset.
 */
typedef struct C8950Node {
    /* 0x00 */ u16 key;         // bits 0-9: id; NODE_FLAGS bits: state
    /* 0x02 */ u16 unk2;        // compared with D_800BE9F0
    /* 0x04 */ u16 size;        // size of the whole allocation
    /* 0x06 */ u16 dataSize;
    /* 0x08 */ u16 unk8;
    /* 0x0A */ u16 dataOffset;
    /* 0x0C */ u8  padC[0xC];
    /* 0x18 */ struct C8950Node *next;
    /* 0x1C */ struct C8950Node *prev;
} C8950Node;

typedef struct {
    /* 0x00 */ u16 count;
    /* 0x04 */ C8950Node *head;
    /* 0x08 */ C8950Node *tail;
} C8950List;

#define NODE_FLAGS 0xFC00

/* variables.h declares these with partial types (struct250, struct102 *, s32). */
#define NODE_LIST (*(C8950List *)&D_800D2F48)
/* Bit `id` of the 0x1B-byte bit array at D_800D2E4C. */
// matching: offset + base operand order (indexing puts the base first and swaps registers)
#define ID_FLAG(id) (*(((id) >> 3) + (u8 *)D_800D2E4C) & (1 << ((id) & 7)))
/* One state byte per id. */
#define ID_STATE ((u8 *)&D_800D2E70)

C8950Node *func_1509B950(C8950Node *node);
C8950Node *func_1509B704(s16 id);
C8950Node *func_1509B5AC(s32 id, s32 reload);
void func_1509B810(C8950Node *node);
C8950Node *func_1509B8FC(s16 id);
void func_1509C120(void);
void func_1509C3A0(void);
void func_150ADAF0(C8950Node *node, s32 mode);

/* Gets (and, if needed, creates) the record of every id listed in D_800D2F40 that func_1509CBD4 accepts. */
void func_1509B4A0(s32 unused, s32 reload) {
    s32 i;

    D_8003C8E0 = 0x5000000;
    func_1509C120();
    func_15096970();

    for (i = 0; i < D_800D2F3C; i++) {
        if (!func_1509CBD4(D_800D2F40[i])) {
            continue;
        }
        func_1509B5AC(D_800D2F40[i], reload);
    }

    func_1509C3A0();
    D_8003C8E0 = 0;
}

/* Returns the payload of the record for `id`, or NULL if none is loaded. */
void *func_1509B570(s32 id) {
    C8950Node *node;

    node = func_1509B704(id);
    if (node != NULL) {
        return (u8 *)node + node->dataOffset;
    }
    return NULL;
}

/* Returns the record for `id`, creating it if needed; NULL if `reload` is 0 and the id's flag is set. */
C8950Node *func_1509B5AC(s32 id, s32 reload) {
    C8950Node *node;

    node = func_1509B704(id);
    if (!reload && ID_FLAG(id)) {
        return NULL;
    }

    if (node == NULL) {
        node = func_1509B8FC(id);
        func_1509B810(node);
        if (!ID_FLAG(node->key & ~NODE_FLAGS)) {
            D_8003C8E0 = 0x6000000;
            func_150ADAF0(node, 0);
            D_8003C8E0 = 0;
        }
        if (!reload || (node->key & 0x400)) {
            node->key |= 0x4000;
        }
    } else if (reload) {
        if ((node->key & 0x1000) && !(node->key & 0x800)) {
            bzero((u8 *)node + node->dataOffset, node->dataSize);
        }
    }

    return node;
}

/* Returns the loaded record for `id`, or NULL. */
C8950Node *func_1509B704(s16 id) {
    C8950Node *node;
    s32 i;

    node = NODE_LIST.head;
    for (i = 0; i < NODE_LIST.count; i++) {
        if (id == (node->key & ~NODE_FLAGS)) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

void func_10004074(void *ptr);
void *allocate_memory(s32 size, s32 arg1, s32 arg2, s32 arg3);

/* Unlinks `node` from the record list and frees it. */
void func_1509B764(C8950Node *node) {
    if (NODE_LIST.count == 1) {
        NODE_LIST.head = NULL;
        NODE_LIST.tail = NULL;
    } else {
        if (node == NODE_LIST.head) {
            NODE_LIST.head = node->next;
            node->next->prev = NULL;
        } else {
            node->prev->next = node->next;
        }
        if (node == NODE_LIST.tail) {
            NODE_LIST.tail = node->prev;
            node->prev->next = NULL;
        } else {
            node->next->prev = node->prev;
        }
    }
    func_10004074(node);
    NODE_LIST.count--;
}

/* Inserts `node` into the record list, keeping it sorted by id. */
void func_1509B810(C8950Node *node) {
    C8950Node *cur;
    C8950Node *head;
    s32 id;
    s32 i;

    cur = NODE_LIST.tail;
    id = node->key & ~NODE_FLAGS;
    if (NODE_LIST.count == 0) {
        NODE_LIST.head = node;
        NODE_LIST.tail = node;
        node->next = NULL;
        node->prev = NULL;
        NODE_LIST.count += 1;
        return;
    }

    for (i = 0; i < NODE_LIST.count; i++) {
        if ((cur->key & ~NODE_FLAGS) < id) {
            if (cur == NODE_LIST.tail) {
                node->prev = cur;
                node->next = NULL;
                cur->next = node;
                NODE_LIST.tail = node;
                NODE_LIST.count += 1;
                return;
            }
            node->prev = cur;
            node->next = cur->next;
            cur->next->prev = node;
            cur->next = node;
            NODE_LIST.count += 1;
            return;
        }
        cur = cur->prev;
    }

    head = NODE_LIST.head;
    NODE_LIST.head = node;
    node->prev = NULL;
    node->next = head;
    head->prev = node;
    NODE_LIST.count += 1;
}

void *func_1502B5C8(s32 *size, s32 arg1, s32 arg2, s32 arg3);

/* Loads a new record for `id` (via func_1502B5C8) and tags it with D_800BE9F0. */
C8950Node *func_1509B8FC(s16 id) {
    C8950Node *node;
    s32 size;

    node = func_1502B5C8(&size, 2, 20, id);
    node->key |= id;
    node->unk2 = D_800BE9F0;
    return func_1509B950(node);
}

/* Reallocates `node` with its payload 8-byte aligned and zeroed, and frees the original. */
C8950Node *func_1509B950(C8950Node *node) {
    C8950Node *copy;
    s32 padding;

    padding = 8 - (((s32)node + node->size) & 7);
    node->dataOffset = node->size + padding;
    node->size = node->dataOffset + node->dataSize;
    node->size = (node->size - (((s32)node + node->size) & 7)) + 8;

    copy = allocate_memory(node->size, 0xFF, 2, 0);
    if (copy == NULL) {
        while (1) {}
    }

    bcopy(node, copy, node->size);
    bzero((u8 *)copy + copy->dataOffset, copy->dataSize);
    func_10004074(node);
    return copy;
}

void func_1509C228(void);
extern u8 D_800D2E69;

/*
 * Frees records. With `unloadAll` every record runs its mode-3 handler and is freed. Otherwise records
 * whose id flag is set are freed silently (state 3), and records with flag 0x8000, or with flag 0x1000
 * and an unk2 other than D_800BE9F0, run the mode-3 handler and are freed.
 */
void func_1509BA04(s32 unloadAll) {
    C8950Node *node;
    C8950Node *next;
    s32 count;
    s32 i;

    count = NODE_LIST.count;
    node = NODE_LIST.head;
    if (D_800D2E69 != 0) {
        func_1509C228();
        D_800D2E69 = 0;
    }
    for (i = 0; i < count; i++, node = next) {
        s32 id;
        s32 key;

        key = node->key;
        id = key & ~NODE_FLAGS;
        next = node->next;
        if (ID_FLAG(id) && !unloadAll) {
            func_1509B764(node);
            ID_STATE[id] = 3;
        } else if (unloadAll || (key & 0x8000) || ((key & 0x1000) && D_800BE9F0 != node->unk2)) {
            D_8003C8E0 = 0x6000001;
            func_150ADAF0(node, 3);
            D_8003C8E0 = 0;
            func_1509B764(node);
            if (!ID_FLAG(id)) {
                ID_STATE[id] = 0;
            } else {
                ID_STATE[id] = 3;
            }
        }
    }
}

void func_15096D08(void);
s32 func_1509BE40(s32, s32, s32, ...);
extern s16 D_800BE9F2;
extern s32 D_800D2E48;
extern s32 D_800D2F58;

/* Runs every record's handler for `mode` (1, 2 or 4) and updates the per-id state bytes. */
void func_1509BBA0(s32 mode) {
    C8950Node *node;
    s32 i;
    u16 id;
    s32 key;

    node = NODE_LIST.head;
    func_15096D08();
    if (!D_800BE616 && D_800BE9F0 != 50 && !func_1509BE40(0, 0x2000, 0x93) && mode == 2) {
        return;
    }
    func_1509C228();
    for (i = 0; i < NODE_LIST.count; i++) {
        id = node->key & 0x3FF;
        D_800D2E48 = id;
        key = node->key;
        if (key & 0x8000) {
            node = node->next;
            continue;
        }
        if (mode == 4) {
            if (ID_FLAG(id)) {
                D_8003C8E0 = 0x6000002;
                func_150ADAF0(node, 4);
                D_8003C8E0 = 0;
                ID_STATE[id] = 3;
                node->key |= 0x8000;
                node = node->next;
                continue;
            }
        } else if (!ID_FLAG(id)) {
            if (key & 0x4000) {
                D_8003C8E0 = 0x6000003;
                func_150ADAF0(node, 1);
                D_8003C8E0 = 0;
                key = node->key &= ~0x4000;
            }
            if (key & 0x8000) {
                node = node->next;
                continue;
            }
            if (!(key & 0x2000) || D_800BE9F0 == node->unk2) {
                if (mode == 2 || (mode == 1 && !(key & 0x400))) {
                    D_8003C8E0 = 0x6000004;
                    func_150ADAF0(node, mode);
                    D_8003C8E0 = 0;
                }
                if (mode == 2) {
                    ID_STATE[id] = 1;
                }
            } else {
                ID_STATE[id] = 2;
            }
        } else {
            ID_STATE[id] = 3;
        }
        node = node->next;
    }
    if (D_800D2F58 != 0) {
        D_800D2F58 = 0;
        func_1509B4A0(D_800BE9F2, 0);
    }
    D_800D3858 = 0;
}

// matching: a tentative definition here (not extern) lets as1 share one `lui` for the [0]/[1] stores
s32 D_800D2F60[16];
extern s32 D_800D3840;
extern s32 (*D_80088498[])(s32, s32, s32 *);
extern s32 (*D_800884D4[])(s32, s32, s32 *);

/*
 * Command dispatchers: bits 12-15 of `cmd` select a handler in the table, bits 0-11 are passed
 * to it with `arg` and `count` further variadic words (collected in D_800D2F60 with cmd and arg).
 * matching: cmdWord and arg are read back from their home slots via va_arg (va_start on `count`)
 * into a separate local `cmd` and the reused `arg`.
 */
s32 func_1509BE40(s32 count, s32 cmdWord, s32 arg, ...) {
    va_list ap;
    s32 i;
    s32 cmd;
    s32 type;
    s32 saved;
    s32 result;

    va_start(ap, count);
    cmd = va_arg(ap, s32);
    saved = D_800D3840;
    type = ((cmd & 0xF000) >> 12) - 1;
    cmd &= 0xFFF;
    arg = va_arg(ap, s32);
    D_800D2F60[0] = cmd;
    D_800D2F60[1] = arg;
    for (i = 0; i < count; i++) {
        D_800D2F60[i + 2] = va_arg(ap, s32);
    }
    va_end(ap);
    D_800D3840 = count + 2;
    result = D_80088498[type](cmd, arg, D_800D2F60);
    D_800D3840 = saved;
    return result;
}

void func_1509BFB0(s32 count, s32 cmdWord, s32 arg, ...) {
    va_list ap;
    s32 i;
    s32 cmd;
    s32 type;
    s32 saved;

    va_start(ap, count);
    cmd = va_arg(ap, s32);
    saved = D_800D3840;
    type = ((cmd & 0xF000) >> 12) - 1;
    cmd &= 0xFFF;
    arg = va_arg(ap, s32);
    D_800D2F60[0] = cmd;
    D_800D2F60[1] = arg;
    for (i = 0; i < count; i++) {
        D_800D2F60[i + 2] = va_arg(ap, s32);
    }
    va_end(ap);
    D_800D3840 = count + 2;
    D_800884D4[type](cmd, arg, D_800D2F60);
    D_800D3840 = saved;
}

/*
 * (Re)allocates the 0x1B-byte id bit array: allocates 1-4 buffers (count from func_150ADA20),
 * keeps the last one and frees the others, then copies the old array over and frees it.
 */
void func_1509C120(void) {
    s32 i;
    s32 last;
    struct102 *bufs[4];
    struct102 *old;

    last = func_150ADA20() & 3;
    old = D_800D2E4C;
    if (old != NULL) {
        if (!(func_150ADA20() & 4)) {
            return;
        }
    }
    for (i = 0; i <= last; i++) {
        bufs[i] = allocate_memory(0x1B, 0xFF, 2, 0);
    }
    for (i = 0; i <= last; i++) {
        if (i != last) {
            func_10004074(bufs[i]);
        }
    }
    D_800D2E4C = bufs[last];
    if (old != NULL) {
        bcopy(old, D_800D2E4C, 0x1B);
        func_10004074(old);
    }
}

void func_1509C228(void) {
    if ((D_800D2E68 & 8) || ID_FLAG(120)) {
        func_1509BFB0(2, 0x2000, 0x36, 0, 1);
        return;
    }
    func_1509BFB0(2, 0x2000, 0x36, 1, 1);
}

/* Returns 0 when D_800BE9F0 is one of the listed values or D_800D2E44 is set, else 1. */
s32 func_1509C2A4(void) {
    s32 value = D_800BE9F0;
    if ((value == 3) ||
        (value == 5) ||
        (value == 9) ||
        (value == 13) ||
        (value == 15) ||
        (value == 17) ||
        (value == 21) ||
        (value == 22) ||
        (value == 24) ||
        (value == 26) ||
        (value == 29) ||
        (value == 31) ||
        (value == 32) ||
        (value == 33) ||
        (value == 34) ||
        (value == 36) ||
        (value == 37) ||
        (value == 42) ||
        (value == 43) ||
        (value == 45) ||
        (value == 48) ||
        (value == 51) ||
        (value == 52) ||
        (value == 56) ||
        (value == 62) ||
        (value == 63) ||
        (D_800D2E44 != 0)
      ) {
        return 0;
    }
    return 1;
}

void func_1509C3A0(void) {
    if (D_800D2E43 && !D_800D2E44 && func_1509C2A4()) {
        D_800BE3DF = D_800BE9F4;
        if (D_800C35C4 == 0) {
            func_15007718(D_8008FDA8);
        }
        D_800D2E43 = 0;
    }
}

/* Returns index + 0x1400, plus 0x400 when id flag 24 is set. */
s32 func_1509C414(s32 index) {
    return (ID_FLAG(24) << 10) + index + 0x1400;
}
