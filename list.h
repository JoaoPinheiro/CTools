#ifndef _C_TOOLS_LIST_H_
#define _C_TOOLS_LIST_H_

struct _list_node_ {
	struct _list_node_ *next;
	int value;
};

typedef struct _linked_list_ {
	struct _list_node_ *head;
} linkedlist;

linkedlist* createLinkedList();
void freeLinkedList(linkedlist *list);
void addNode(linkedlist *list, int value);
void removeNode(linkedlist *list, int value);
int containsValue(linkedlist *list, int value);
void mapLinkedList(linkedlist *list, void (*funcp)(int));

#endif
