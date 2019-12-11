
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define SIZE 1000

int stackSize = 0;

struct StackDataItem {
    void* data;
    struct StackDataItem* prev;
};

struct StackDataItem* item;

void push(void* data) {
    struct StackDataItem* pItem = (struct StackDataItem*) malloc(sizeof(struct StackDataItem));
    pItem->data = data;
    pItem->prev = item;
    stackSize++;
}

void* top() {
    if (item == NULL)
        return NULL;
    return item->data;
}

void* pop() {
    if (item == NULL)
        return NULL;
    void* topData = item->data;
    item = item->prev;
    stackSize--;
    return topData;
}

int size() {
    return stackSize;
}

bool isEmpty() {
    return stackSize == 0;
}

typedef struct {
    void (*push)(void*);
    void* (*top)();
    void* (*pop)();
    int (*size)();
    bool (*isEmpty)();
} Stack;

Stack createStack() {
    Stack stack;
    stack.push = push;
    stack.top = top;
    stack.pop = pop;
    stack.size = size;
    stack.isEmpty = isEmpty;
    return stack;
}