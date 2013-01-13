#include "hashmap.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

/**
 * 参考cloudwu的mread实现一个开放定址法map
 * map存储正、负、零整数
 * 2013-01-11
 */

struct node {
    int num;
    int use;                    /* 节点是否使用 1使用 -1未使用 */
    struct node * next;
};

struct hashmap {
    int size;
    struct node * hash;
};


struct hashmap * map_new(int max) {
    int sz = 1;
    while (sz <= max) {
        sz *= 2;
    }
    
    struct hashmap * m = malloc(sizeof(struct hashmap));
    assert(m != NULL);
    m->size = sz;
    m->hash = malloc(sizeof(struct node) * sz);
    assert(m->hash != NULL);
    int i;
    for (i = 0; i < sz; i++) {
        m->hash[i].num = 0;
        m->hash[i].use = -1;
        m->hash[i].next = NULL;
    }

    return m;
}

void map_delete(struct hashmap * m) {
    assert(m != NULL);
    
    free(m->hash);
    free(m);
}

void map_insert(struct hashmap * m, int num) {
    int hash = num & (m->size - 1);
    struct node * n = &m->hash[hash];

    /* 空 */
    if (n->use < 0) {
        n->num = num;
        n->use = 1;
        return;
    }

    /* 非空，冲突 */
    int ohash = n->num & (m->size - 1);
    if (hash != ohash) {
        struct node * last = &m->hash[ohash];
        while (last->next != &m->hash[hash]) {
            last = last->next;
        }
        last->next = n->next;

        int onum = n->num;
        n->num = num;
        n->use = 1;
        n->next = NULL;
        map_insert(m, onum);
        return;
    }

    /* 非空，不冲突;寻找一个空白节点 */
    int last = (n - m->hash) * 2;
    int i;
    for (i = 0; i < m->size; i++) {
        int idx = (i + last + 1) & (m->size - 1);
        struct node * temp = &m->hash[idx];
        if (temp->use < 0) {
            temp->num = num;
            temp->use = 1;
            temp->next = n->next;
            n->next = temp;
            return;
        }
    }
    assert(0);
}

int map_search(struct hashmap * m, int num) {
    int hash = num & (m->size - 1);
    struct node * n = &m->hash[hash];
    do {
        if (n->num == num) {
            return 1;
        }
        n = n->next;
    } while (n);

    return -1;
}

void map_earse(struct hashmap * m, int num) {
    int hash = num & (m->size - 1);
    struct node * n = &m->hash[hash];
    if (n->num == num) {
        if (n->next == NULL) {
            n->use = -1;
            return;
        }
        struct node * next = n->next;
        n->num = next->num;
        n->next = next->next;
        next->use = -1;
        next->next = NULL;
        return;
    }
    if (n->next == NULL) {
        return;                 /* 无此数 */
    }
    struct node * last = n;
    n = n->next;
    for (;;) {
        if (n->num == num) {
            last->next = n->next;
            n->use = -1;
            n->next = NULL;
            return;
        }
        if (n->next == NULL) {
            return;
        }
        last = n;
        n = n->next;
    }
}

void map_dump(struct hashmap * m) {
    int i;
    for (i = 0; i < m->size; i++) {
        struct node * n = &(m->hash[i]);
        printf("[%d] num = %d, use = %d, next = %d\n", i, n->num, n->use, (int)(n->next - m->hash));
    }
}
