#pragma once

#include <stdint.h>

#define container_of(ptr, type, member) \
    (type *)((char *)ptr - ((size_t)&((type*)0)->member))

struct list_head {
    struct list_head *prev, *next;
};

#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *lst) {
    lst->prev = lst->next = lst;
}

#define list_for_each(pos, head) \
    for (auto *pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_r(pos, head) \
    for (auto *pos = (head)->prev; pos != (head); pos = pos->prev)

static inline void __list_add(struct list_head *n, struct list_head *prev,
                              struct list_head *next) {
    n->next = next;
    n->prev = prev;
    next->prev = n;
    prev->next = n;
}

static inline void list_add(struct list_head *n, struct list_head *head) {
    __list_add(n, head, head->next);
}

static inline void list_add_tail(struct list_head *n, struct list_head *head) {
    __list_add(n, head->prev, head);
}

static inline void __list_del(struct list_head *prev, struct list_head *next) {
    next->prev = prev;
    prev->next = next;
}

static inline void list_del(struct list_head *entry) {
    __list_del(entry->prev, entry->next);
    entry->prev = entry->next = entry;
}

static inline bool list_empty(const struct list_head *head) {
    return head->next == head;
}

