#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

static void printCToolsMessage(char *location, char *message) {
	assert(location != NULL);
	assert(message != NULL);
	
	fprintf(stderr, "[CTools] %s: %s\n", location, message);
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
	iterator->list = list;
	iterator->previous = list->head;
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
void* getNext(listiterator *iterator) {
	assert(iterator != NULL);
	assert(iterator->node != NULL);
	assert(iterator->node->next != NULL);
	
	iterator->previous = iterator->node;
	iterator->node = iterator->node->next;
	return iterator->node->value;
}

/* Removes the current value from the list associated with the iterator */
void removeCurrent(listiterator *iterator) {
	assert(iterator != NULL);
	assert(iterator->node != NULL);
	assert(iterator->previous != NULL);
	assert(iterator->list != NULL);
	assert(iterator->list->free != NULL);
	
	iterator->previous->next = iterator->node->next;
	assert(iterator->node->references != NULL);
	if (--(*iterator->node->references) == 0) {
		assert(iterator->node->value != NULL);
		iterator->list->free(iterator->node->value);
	}
	free(iterator->node);
	iterator->node = iterator->previous;
}

/* Releases all the memory allocated by the list iterator */
void freeIterator(listiterator *iterator) {
	assert(iterator != NULL);
	
	free(iterator);
}

/* Returns the references from the current item in the list associated with the iterator */
static unsigned int* getCurrentReferences(listiterator *iterator) {
	assert(iterator != NULL);
	assert(iterator->node != NULL);
	assert(iterator->node->references != NULL);
	
	return iterator->node->references;
}

/* Creates a new linked list structure */
linkedlist* createList() {
	linkedlist *list = NULL;
	struct _list_node_ *dummynode = NULL;
	unsigned int *references = NULL;
	
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
	references = malloc(sizeof(unsigned int));
	if (references == NULL) {
		printCToolsMessage("createList", "Not enough memory");
		free(list);
		free(dummynode);
		return NULL;
	}
	*references = 1;
	dummynode->value = NULL;
	dummynode->next = NULL;
	dummynode->references = references;
	list->head = dummynode;
	list->elemsize = 0;
	list->equals = NULL;
	list->copy = NULL;
	list->free = NULL;
	return list;
}

/* Releases all the memory allocated by the linked list */
void freeList(linkedlist *list) {
	listiterator *iterator = NULL;
	
	assert(list != NULL);
	assert(list->head != NULL);
	
	iterator = createIterator(list);
	while (hasNext(iterator)) {
		getNext(iterator);
		removeCurrent(iterator);
	}
	freeIterator(iterator);
	free(list->head);
	free(list);
}

/* Adds a copy of the value to the beginning of the list */
void addValue(linkedlist *list, void *value) {
	void *newvalue = NULL;
	struct _list_node_ *newnode = NULL;
	unsigned int *references = NULL;
	
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
	references = malloc(sizeof(unsigned int));
	if (references == NULL) {
		printCToolsMessage("addValue", "Not enough memory");
		free(newnode);
		free(newvalue);
		return;
	}
	*references = 1;
	list->copy(newvalue, value);
	newnode->value = newvalue;
	newnode->references = references;
	newnode->next = list->head->next;
	list->head->next = newnode;
}

/* Removes all occurrences of a given value */
void removeValue(linkedlist *list, void *value) {
	listiterator *iterator = NULL;
	void *nodevalue = NULL;
	
	assert(value != NULL);
	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->equals != NULL);
	
	iterator = createIterator(list);
	while (hasNext(iterator)) {
		nodevalue = getNext(iterator);
		assert(nodevalue != NULL);
		if (list->equals(nodevalue, value)) {
			removeCurrent(iterator);
		}
	}
	freeIterator(iterator);
}

/* Checks whether the list contains a value */
int containsValue(linkedlist *list, void *value) {
	listiterator *iterator = NULL;
	void *nodevalue = NULL;
	
	assert(value != NULL);
	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->equals != NULL);
	
	iterator = createIterator(list);
	while (hasNext(iterator)) {
		nodevalue = getNext(iterator);
		assert(nodevalue != NULL);
		if (list->equals(nodevalue, value)) {
			freeIterator(iterator);
			return 1;
		}
	}
	freeIterator(iterator);
	return 0;
}

/* Returns a reference to the first element with the given value */
void* getItem(linkedlist *list, void *value) {
	listiterator *iterator = NULL;
	void *nodevalue = NULL;
	
	assert(value != NULL);
	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->equals != NULL);
	
	iterator = createIterator(list);
	while (hasNext(iterator)) {
		nodevalue = getNext(iterator);
		assert(nodevalue != NULL);
		if (list->equals(nodevalue, value)) {
			freeIterator(iterator);
			return nodevalue;
		}
	}
	freeIterator(iterator);
	return NULL;
}

/* Applies a function to each element of the list */
void mapList(linkedlist *list, void (*funcp)(void*)) {
	listiterator *iterator = NULL;
	void *value = NULL;
	
	assert(funcp != NULL);
	assert(list != NULL);
	assert(list->head != NULL);
	
	iterator = createIterator(list);
	while (hasNext(iterator)) {
		value = getNext(iterator);
		assert(value != NULL);
		(*funcp)(value);
	}
	freeIterator(iterator);
}

/* Generates a copy of the list */
linkedlist* copyList(linkedlist *list) {
	linkedlist *newlist = NULL;
	struct _list_node_ *newnode = NULL;
	struct _list_node_ *previousnode = NULL;
	listiterator *iterator = NULL;
	void *value = NULL;
	unsigned int *references = NULL;
	
	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->elemsize != 0);
	assert(list->copy != NULL);
	
	newlist = createList();
	newlist->equals = list->equals;
	newlist->copy = list->copy;
	newlist->free = list->free;
	previousnode = newlist->head;
	iterator = createIterator(list);
	while (hasNext(iterator)) {
		newnode = malloc(sizeof(struct _list_node_));
		if (newnode == NULL) {
			printCToolsMessage("copyList","Not enough memory");
			freeList(newlist);
			freeIterator(iterator);
			return NULL;
		}
		value = getNext(iterator);
		assert(value != NULL);
		newnode->value = value;
		newnode->next = NULL;
		references = getCurrentReferences(iterator);
		assert(references != NULL);
		(*references)++;
		newnode->references = references;
		previousnode->next = newnode;
		previousnode = newnode;
	}
	freeIterator(iterator);
	return newlist;
}

/* Returns an array with a copy of each element of the list */
void* listToArray(linkedlist *list, unsigned int *length) {
	void *array = NULL;
	unsigned int count = 0;
	unsigned int i = 0;
	listiterator *iterator = NULL;
	void *value = NULL;
	
	assert(length != NULL);
	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->elemsize != 0);
	assert(list->copy != NULL);
	
	count = getLength(list);
	array = malloc(count * list->elemsize);
	if (array == NULL) {
		printCToolsMessage("listToArray", "Not enough memory");
		return NULL;
	}
	i = 0;
	iterator = createIterator(list);
	while (hasNext(iterator)) {
		value = getNext(iterator);
		assert(value != NULL);
		/* Casting the array to char* to be able to perform pointer arithmetic on a byte level */
		list->copy((char*) array + list->elemsize * i++, value);
	}
	freeIterator(iterator);
	*length = count;
	return array;
}

unsigned int getLength(linkedlist *list) {
	listiterator *iterator = NULL;
	unsigned int length = 0;
	
	assert(list != NULL);
	assert(list->head != NULL);
	
	iterator = createIterator(list);
	while (hasNext(iterator)) {
		getNext(iterator);
		length++;
	}
	freeIterator(iterator);
	return length;
}
