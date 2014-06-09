#ifndef _C_TOOLS_LIST_H_
#define _C_TOOLS_LIST_H_

struct _list_node_ {
	struct _list_node_ *next;
	void *value;
};

typedef struct _linked_list_ {
	struct _list_node_ *head;
	int (*equals)(void*, void*);
	void* (*constructor)(void*);
	void (*destructor)(void*);
} linkedlist;

linkedlist* createLinkedList();
void freeLinkedList(linkedlist *list);
void mapLinkedList(linkedlist *list, void (*funcp)(void*));
void addNode(linkedlist *list, void *value);
void removeNode(linkedlist *list, void *value);
int containsValue(linkedlist *list, void *value);

#endif
