
struct elt {
    struct elt *next;
    char *key;
    void *value;
};

typedef struct {
    int size;           /* size of the pointer table */
    int n;              /* number of elements stored */
    struct elt **table;
} Dictionary;

/* create a new empty dictionary */
Dictionary* dict_new();

/* destroy a dictionary */
void dict_free(Dictionary*);

/* insert a new key-value pair into an existing dictionary */
void dict_add(Dictionary*, char *key, void *value);

/* return the most recently inserted value associated with a key */
/* or 0 if no matching key is present */
void *dict_get(Dictionary*, char *key);

void display_keys(Dictionary* d);

/* delete the most recently inserted record with the given key */
/* if there is no such record, has no effect */
void dict_delete(Dictionary*, const char *key);