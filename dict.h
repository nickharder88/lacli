/* redefine pointer to dict */
typedef struct dict* Dict;

/* create a new empty dictionary */
Dict DictCreate(void);

/* destory a dictionary */
void DictDestory(Dict);

/* insert a new key-value pair into an existing dictionary*/
void DictInsert(Dict, const char* key, int* value);

/* return the most recently inserted value associated with a key */
int* DictSearch(Dict, const char* key);

/* delete the most recently inserted record with the given key */
void DictDelete(Dict, const char* key);
