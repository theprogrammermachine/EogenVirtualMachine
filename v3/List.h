
#include <stddef.h>
#include <stdbool.h>

struct ListDataItem {
    void* data;
    struct ListDataItem* prev;
    struct ListDataItem* next;
};

struct ListDataItem* listPointer;
struct ListDataItem* iteratorPointer;

void append(void* item) {
    struct ListDataItem pItem;
    pItem.data = item;
    if (listPointer->data != NULL) {
        pItem.prev = listPointer;
        listPointer->next = &pItem;
    }
    listPointer = &pItem;
}

void* iteratorForward() {
    iteratorPointer = iteratorPointer->prev;
    return iteratorPointer->data;
}

void* iteratorBackward() {
    iteratorPointer = iteratorPointer->next;
    return iteratorPointer->data;
}

bool iteratorHasNext() {
    return (iteratorPointer->next != NULL);
}

bool iteratorHasBefore() {
    return (iteratorPointer->prev != NULL);
}

typedef struct {
    void (*append)(void*);
    void* (*iteratorForward)();
    void* (*iteratorBackward)();
    bool (*iteratorHasNext)();
    bool (*iteratorHasBefore)();
} List;

List createList() {
    List list;
    list.append = append;
    list.iteratorHasNext = iteratorHasNext;
    list.iteratorHasBefore = iteratorHasBefore;
    list.iteratorForward = iteratorForward;
    list.iteratorBackward = iteratorBackward;
    return list;
}