#ifndef _C_TOOLS_LIST_H_
#define _C_TOOLS_LIST_H_

#include <stdbool.h>

struct _list_node_ {
	struct _list_node_ *next;
	void *value;
	unsigned int *references;
};

typedef struct _linked_list_ {
	struct _list_node_ *head;
	size_t elemsize;
	int (*equals)(void*, void*);
	void (*copy)(void*, void*);
	void (*free)(void*);
} linkedlist;

typedef struct _list_iterator_ {
	struct _linked_list_ *list;
	struct _list_node_ *node;
	struct _list_node_ *previous;
} listiterator;

listiterator* createIterator(linkedlist *list);
bool hasNext(listiterator *iterator);
void* getNext(listiterator *iterator);
void removeCurrent(listiterator *iterator);
void freeIterator(listiterator *iterator);

linkedlist* createList();
void freeList(linkedlist *list);
void addValue(linkedlist *list, void *value);
void removeValue(linkedlist *list, void *value);
bool containsValue(linkedlist *list, void *value);
void* getItem(linkedlist *list, void *value);
void mapList(linkedlist *list, void (*funcp)(void*));
linkedlist* copyList(linkedlist *list);
void* listToArray(linkedlist *list, unsigned int *length);
unsigned int getLength(linkedlist *list);

#endif
