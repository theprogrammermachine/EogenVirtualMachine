
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <boost/any.hpp>

#define SIZE 1000

struct DataItem {
    int key;
    struct DataItem* prevItemPointer;
    void* data;
};

struct DataItem* hashArray[SIZE];
struct DataItem* dummyItem;

struct DataItem* stackTop;

void bufferSetup() {
    dummyItem = (struct DataItem*) malloc(sizeof(struct DataItem));
    strcpy(dummyItem->data, "dummy");
    dummyItem->key = -1;
}

int hashCode(int key) {
    return key % SIZE;
}

struct DataItem *bufferTableSearch(int key) {
    //get the hash
    int hashIndex = hashCode(key);

    //move in array until an empty
    while(hashArray[hashIndex] != NULL) {

        if(hashArray[hashIndex]->key == key)
            return hashArray[hashIndex];

        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    return NULL;
}

void bufferTableInsert(int key, char data[]) {

    struct DataItem *pItem = (struct DataItem*) malloc(sizeof(struct DataItem));
    pItem->data = data;
    pItem->key = key;

    //get the hash
    int hashIndex = hashCode(key);

    //move in array until an empty or deleted cell
    while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1) {
        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    hashArray[hashIndex] = pItem;
}

struct DataItem* bufferTableDelete(struct DataItem* pItem) {
    int key = pItem->key;

    //get the hash
    int hashIndex = hashCode(key);

    //move in array until an empty
    while(hashArray[hashIndex] != NULL) {

        if(hashArray[hashIndex]->key == key) {
            struct DataItem* temp = hashArray[hashIndex];

            //assign a dummy pItem at deleted position
            hashArray[hashIndex] = dummyItem;
            return temp;
        }

        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    return NULL;
}

void bufferTableDisplay() {
    int i = 0;

    for(i = 0; i<SIZE; i++) {

        if(hashArray[i] != NULL)
            printf(" (%d,%s)", hashArray[i]->key, hashArray[i]->data);
        else
            printf(" ~~ ");
    }

    printf("\n");
}

void bufferStackLower() {
    struct DataItem *pItem = (struct DataItem*) malloc(sizeof(struct DataItem));
    pItem->data = "start";
    pItem->key = 1;
    pItem->prevItemPointer = stackTop;
}

struct DataItem bufferStackCurrent() {
    return *stackTop;
}

struct DataItem bufferStackHigher() {
    struct DataItem topItem = *stackTop;
    stackTop = stackTop->prevItemPointer;
    return topItem;
}



