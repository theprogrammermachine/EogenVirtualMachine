
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define SIZE 1000

struct DictDataItem {
    char* key;
    void* data;
};

struct DictDataItem* dictHashArray[SIZE];
struct DictDataItem* dictDummyItem;
struct DictDataItem* dictItem;

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

uint32_t dictHashCode(char* key) {
    return jenkins_one_at_a_time_hash(key, sizeof(key)) % SIZE;
}


void* search(char* key) {
    //get the hash
    uint32_t hashIndex = dictHashCode(key);

    //move in array until an empty
    while(dictHashArray[hashIndex] != NULL) {

        if(dictHashArray[hashIndex]->key == key)
            return dictHashArray[hashIndex]->data;

        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    return NULL;
}

void insert(char* key, void* data) {

    struct DictDataItem* pItem = (struct DictDataItem*) malloc(sizeof(struct DictDataItem));
    pItem->data = data;
    pItem->key = key;

    //get the hash
    int hashIndex = dictHashCode(key);

    //move in array until an empty or deleted cell
    while(dictHashArray[hashIndex] != NULL && strcmp(dictHashArray[hashIndex]->key, "dummy") != 0) {
        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    dictHashArray[hashIndex] = pItem;
}

void* delete(char* key) {

    //get the hash
    int hashIndex = dictHashCode(key);

    //move in array until an empty
    while(dictHashArray[hashIndex] != NULL) {

        if(dictHashArray[hashIndex]->key == key) {
            struct DictDataItem* temp = dictHashArray[hashIndex];

            //assign a dummy item at deleted position
            dictHashArray[hashIndex] = dictDummyItem;
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

        if(dictHashArray[i] != NULL)
            printf(" (%s,%s)", dictHashArray[i]->key, dictHashArray[i]->data);
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