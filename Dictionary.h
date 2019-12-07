
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define SIZE 1000

struct DataItem {
    char* key;
    void* data;
};

struct DataItem* hashArray[SIZE];
struct DataItem* dummyItem;
struct DataItem* item;

uint32_t jenkins_one_at_a_time_hash(const char* key, size_t len)
{
    uint32_t hash, i;
    for(hash = i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

uint32_t hashCode(char* key) {
    return jenkins_one_at_a_time_hash(key, sizeof(key)) % SIZE;
}


void* search(char* key) {
    //get the hash
    uint32_t hashIndex = hashCode(key);

    //move in array until an empty
    while(hashArray[hashIndex] != NULL) {

        if(hashArray[hashIndex]->key == key)
            return hashArray[hashIndex]->data;

        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    return NULL;
}

void insert(char* key, void* data) {

    struct DataItem* pItem = (struct DataItem*) malloc(sizeof(struct DataItem));
    pItem->data = data;
    pItem->key = key;

    //get the hash
    int hashIndex = hashCode(key);

    //move in array until an empty or deleted cell
    while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != "dummy") {
        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    hashArray[hashIndex] = pItem;
}

void* delete(char* key) {

    //get the hash
    int hashIndex = hashCode(key);

    //move in array until an empty
    while(hashArray[hashIndex] != NULL) {

        if(hashArray[hashIndex]->key == key) {
            struct DataItem* temp = hashArray[hashIndex];

            //assign a dummy item at deleted position
            hashArray[hashIndex] = dummyItem;
            return temp->data;
        }

        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    return NULL;
}

void display() {
    int i = 0;

    for(i = 0; i<SIZE; i++) {

        if(hashArray[i] != NULL)
            printf(" (%s,%s)", hashArray[i]->key, hashArray[i]->data);
        else
            printf(" ~~ ");
    }

    printf("\n");
}

typedef struct {
    void (*put)(char*, void*);
    void* (*get)(char*);
    void* (*delete)(char*);
    void (*display)();
} Dictionary;

Dictionary createDictionary() {
    Dictionary dictionary;
    dictionary.put = insert;
    dictionary.get = search;
    dictionary.delete = delete;
    dictionary.display = display;
    return dictionary;
}