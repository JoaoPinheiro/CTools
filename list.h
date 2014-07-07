#ifndef _C_TOOLS_LIST_H_
#define _C_TOOLS_LIST_H_

struct _list_node_ {
	struct _list_node_ *next;
	void *value;
	unsigned int references;
};

typedef struct _linked_list_ {
	struct _list_node_ *head;
	int (*equals)(void*, void*);
	void* (*constructor)(void*);
	void (*destructor)(void*);
} linkedlist;

linkedlist* createList();
void freeList(linkedlist *list);
void addValue(linkedlist *list, void *value);
void removeValue(linkedlist *list, void *value);
int containsValue(linkedlist *list, void *value);
void* getItem(linkedlist *list, void *value);
void mapList(linkedlist *list, void (*funcp)(void*));
linkedlist* copyList(linkedlist *list);
void* listToArray(linkedlist *list, unsigned int *length, size_t size);

#endif
