
#include <stddef.h>
#include <stdbool.h>

struct ListDataItem {
    void* data;
    struct ListDataItem* prev;
    struct ListDataItem* next;
};

struct List {
    int size;
    struct ListDataItem* listPointer;
    struct ListDataItem* iteratorPointer;
    void  (*append)(struct List*, void*);
    void* (*iteratorForward)(struct List*);
    void* (*iteratorBackward)(struct List*);
    bool  (*iteratorHasNext)(struct List*);
    bool  (*iteratorHasBefore)(struct List*);
};

void listAdd(struct List* list, void* item) {
    struct ListDataItem* pItem = malloc(sizeof(struct ListDataItem));
    pItem->data = item;
    if (list->listPointer != NULL) {
        pItem->prev = list->listPointer;
        list->listPointer->next = pItem;
    }
    if (list->iteratorPointer == NULL) {
        list->iteratorPointer = pItem;
        list->iteratorPointer->next = NULL;
        list->iteratorPointer->prev = NULL;
    }
    list->listPointer = pItem;
    list->size++;
}

void* iteratorForward(struct List* list) {
    void* data = list->iteratorPointer->data;
    list->iteratorPointer = list->iteratorPointer->next;
    return data;
}

void* iteratorBackward(struct List* list) {
    void* data = list->iteratorPointer->data;
    list->iteratorPointer = list->iteratorPointer->prev;
    return data;
}

bool iteratorHasNext(struct List* list) {
    return (list->iteratorPointer != NULL);
}

bool iteratorHasBefore(struct List* list) {
    return (list->iteratorPointer != NULL);
}