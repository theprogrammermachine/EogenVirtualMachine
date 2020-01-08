
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

struct StackDataItem {
    void* data;
    struct StackDataItem* prev;
};

struct Stack {

    int stackSize;
    struct StackDataItem* item;

    void (*push)(struct Stack*, void*);
    void* (*top)(struct Stack*);
    void* (*pop)(struct Stack*);
    int (*size)(struct Stack*);
    bool (*isEmpty)(struct Stack*);
    struct StackDataItem* (*iterator)(struct Stack*);
};

void push(struct Stack* stack ,void* data) {
    struct StackDataItem* pItem = (struct StackDataItem*) malloc(sizeof(struct StackDataItem));
    pItem->data = data;
    pItem->prev = stack->item;
    stack->item = pItem;
    stack->stackSize++;
}

struct StackDataItem* iterator(struct Stack* stack) {
    struct StackDataItem* iterator = stack->item;
    return iterator;
}

void* top(struct Stack* stack) {
    if (stack->item == NULL)
        return NULL;
    return stack->item->data;
}

void* pop(struct Stack* stack) {
    if (stack->item == NULL)
        return NULL;
    void* topData = stack->item->data;
    stack->item = stack->item->prev;
    stack->stackSize--;
    return topData;
}

int size(struct Stack* stack) {
    return stack->stackSize;
}

bool isEmpty(struct Stack* stack) {
    return stack->stackSize == 0;
}