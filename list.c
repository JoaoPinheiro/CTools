#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

static void printCToolsMessage(char *location, char *message) {
	assert(location != NULL);
	assert(message != NULL);

	fprintf(stderr, "[CTools] %s: %s\n", location, message);
}

/* Creates a new linked list structure */
linkedlist* createList() {
	linkedlist *list = NULL;
	struct _list_node_ *dummynode = NULL;

	list = malloc(sizeof(linkedlist));
	if (list == NULL) {
		printCToolsMessage("createList", "Not enough memory");
		return NULL;
	}
	dummynode = malloc(sizeof(struct _list_node_));
	if (dummynode == NULL) {
		printCToolsMessage("createList", "Not enough memory");
		free(list);
		return NULL;
	}

	dummynode->value = NULL;
	dummynode->next = NULL;
	dummynode->references = 1;
	list->head = dummynode;
	list->elemsize = 0;
	list->equals = NULL;
	list->copy = NULL;
	list->free = NULL;
	return list;
}

/* Releases all the memory allocated by the linked list */
void freeList(linkedlist *list) {
	struct _list_node_ *nextnode = NULL;
	struct _list_node_ *currentnode = NULL;

	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->free != NULL);

	nextnode = list->head->next;
	currentnode = NULL;
	while (nextnode != NULL) {
		currentnode = nextnode;
		nextnode = currentnode->next;
		assert(currentnode->value != NULL);
		if (--currentnode->references == 0) {
			list->free(currentnode->value);
		}
		free(currentnode);
	}

	free(list->head);
	free(list);
}

/* Adds a copy of the value to the beginning of the list */
void addValue(linkedlist *list, void *value) {
	void *newvalue = NULL;
	struct _list_node_ *newnode = NULL;

	assert(value != NULL);
	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->elemsize != 0);
	assert(list->copy != NULL);

	newnode = malloc(sizeof(struct _list_node_));
	if (newnode == NULL) {
		printCToolsMessage("addValue","Not enough memory");
		return;
	}
	newvalue = malloc(list->elemsize);
	if (newvalue == NULL) {
		printCToolsMessage("addValue","Not enough memory");
		free(newnode);
		return;
	}
	list->copy(newvalue, value);
	newnode->value = newvalue;
	newnode->references = 1;
	newnode->next = list->head->next;
	list->head->next = newnode;
}

/* Removes all occurrences of a given value */
void removeValue(linkedlist *list, void *value) {
	struct _list_node_ *previousnode = NULL;
	struct _list_node_ *currentnode = NULL;

	assert(value != NULL);
	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->equals != NULL);
	assert(list->free != NULL);

	previousnode = list->head;
	currentnode = list->head->next;
	while (currentnode != NULL) {
		assert(currentnode->value != NULL);
		if (list->equals(currentnode->value, value)) {
			previousnode->next = currentnode->next;
			if (--currentnode->references == 0) {
				list->free(currentnode->value);
			}
			free(currentnode);
			currentnode = previousnode->next;
		} else {
			previousnode = currentnode;
			currentnode = currentnode->next;
		}
	}
}

/* Checks whether the list contains a value */
int containsValue(linkedlist *list, void *value) {
	struct _list_node_ *node = NULL;

	assert(value != NULL);
	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->equals != NULL);

	node = list->head->next;
	while (node != NULL) {
		assert(node->value != NULL);
		if (list->equals(node->value, value)) {
			return 1;
		}
		node = node->next;
	}
	return 0;
}

/* Returns a reference to the first element with the given value */
void* getItem(linkedlist *list, void *value) {
	struct _list_node_ *node = NULL;

	assert(value != NULL);
	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->equals != NULL);

	node = list->head->next;
	while (node != NULL) {
		assert(node->value != NULL);
		if (list->equals(node->value, value)) {
			return node->value;
		}
		node = node->next;
	}
	return NULL;
}

/* Applies a function to each element of the list */
void mapList(linkedlist *list, void (*funcp)(void*)) {
	struct _list_node_ *node = NULL;

	assert(funcp != NULL);
	assert(list != NULL);
	assert(list->head != NULL);

	node = list->head->next;
	while (node != NULL) {
		assert(node->value != NULL);
		(*funcp)(node->value);
		node = node->next;
	}
}

/* Generates a copy of the list */
linkedlist* copyList(linkedlist *list) {
	linkedlist *newlist = NULL;
	struct _list_node_ *newnode = NULL;
	struct _list_node_ *previousnode = NULL;
	struct _list_node_ *node = NULL;
	void *newvalue = NULL;

	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->elemsize != 0);
	assert(list->copy != NULL);

	newlist = createList();
	newlist->equals = list->equals;
	newlist->copy = list->copy;
	newlist->free = list->free;

	previousnode = newlist->head;
	node = list->head->next;
	while (node != NULL) {
		newnode = malloc(sizeof(struct _list_node_));
		if (newnode == NULL) {
			printCToolsMessage("copyList","Not enough memory");
			freeList(newlist);
			return NULL;
		}
		newvalue = malloc(list->elemsize);
		if (newvalue == NULL) {
			printCToolsMessage("copyList","Not enough memory");
			freeList(newlist);
			free(newnode);
			return NULL;
		}
		assert(node->value != NULL);
		list->copy(newvalue, node->value);
		newnode->value = newvalue;
		newnode->next = NULL;
		newnode->references = 1;
		previousnode->next = newnode;
		previousnode = newnode;
		node = node->next;
	}

	return newlist;
}

/* Returns an array with a copy of each element of the list */
void* listToArray(linkedlist *list, unsigned int *length) {
	struct _list_node_ *node = NULL;
	void *array = NULL;
	unsigned int count = 0;
	unsigned int i = 0;

	assert(length != NULL);
	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->elemsize != 0);
	assert(list->copy != NULL);

	node = list->head->next;
	count = 0;
	while (node != NULL) {
		count++;
		node = node->next;
	}

	array = malloc(count * list->elemsize);
	if (array == NULL) {
		printCToolsMessage("listToArray", "Not enough memory");
		return NULL;
	}

	i = 0;
	node = list->head->next;
	while (node != NULL) {
		assert(node->value != NULL);
		/* Casting the array to char* to be able to perform pointer arithmetic on a byte level */
		list->copy((char*) array + list->elemsize * i++, node->value);
		node = node->next;
	}

	*length = count;
	return array;
}

/* Creates a new list iterator structure */
listiterator* createIterator(linkedlist *list) {
	listiterator *iterator = NULL;
	
	assert(list != NULL);
	
	iterator = malloc(sizeof(listiterator));
	if (iterator == NULL) {
		printCToolsMessage("createIterator", "Not enough memory");
		return NULL;
	}
	iterator->node = list->head;
	return iterator;
}

/* Returns true if there is a next element for the given iterator */
int hasNext(listiterator *iterator) {	
	assert(iterator != NULL);
	assert(iterator->node != NULL);
	
	return iterator->node->next != NULL;
}

/* Returns the next value from the list associated with the iterator */
void* getNext(listiterator *iterator){
	assert(iterator != NULL);
	assert(iterator->node != NULL);
	assert(iterator->node->next != NULL);
	
	iterator->node = iterator->node->next;
	return iterator->node->value;
}

/* Releases all the memory allocated by the list iterator */
void freeIterator(listiterator *iterator) {
	assert(iterator != NULL);
	
	free(iterator);
}
