#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "list.h"

static void printCToolsMessage(char *location, char *message) {
	assert(location != NULL);
	assert(message != NULL);
	
	fprintf(stderr, "[CTools] %s: %s\n", location, message);
}

/* Creates a new list iterator structure */
listiterator* createIterator(linkedlist *list) {
	assert(list != NULL);
	
	listiterator *iterator = malloc(sizeof(listiterator));
	if (iterator == NULL) {
		printCToolsMessage("createIterator", "Not enough memory");
		return NULL;
	}
	*iterator = (listiterator) { .list = list, .previous = list->head, .node = list->head };
	return iterator;
}

/* Returns true if there is a next element for the given iterator */
bool hasNext(listiterator *iterator) {
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
	linkedlist *list = malloc(sizeof(linkedlist));
	if (list == NULL) {
		printCToolsMessage("createList", "Not enough memory");
		return NULL;
	}
	struct _list_node_ *dummynode = malloc(sizeof(struct _list_node_));
	if (dummynode == NULL) {
		printCToolsMessage("createList", "Not enough memory");
		free(list);
		return NULL;
	}
	unsigned int *references = malloc(sizeof(unsigned int));
	if (references == NULL) {
		printCToolsMessage("createList", "Not enough memory");
		free(list);
		free(dummynode);
		return NULL;
	}
	*references = 1;
	*dummynode = (struct _list_node_) { .value = NULL, .next = NULL, .references = references };
	*list = (linkedlist) { .head = dummynode, .elemsize = 0, .equals = NULL, .copy = NULL, .free = NULL };
	return list;
}

/* Releases all the memory allocated by the linked list */
void freeList(linkedlist *list) {
	assert(list != NULL);
	assert(list->head != NULL);
	
	listiterator *iterator = createIterator(list);
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
	assert(value != NULL);
	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->elemsize != 0);
	assert(list->copy != NULL);
	
	struct _list_node_ *newnode = malloc(sizeof(struct _list_node_));
	if (newnode == NULL) {
		printCToolsMessage("addValue","Not enough memory");
		return;
	}
	void *newvalue = malloc(list->elemsize);
	if (newvalue == NULL) {
		printCToolsMessage("addValue","Not enough memory");
		free(newnode);
		return;
	}
	unsigned int *references = malloc(sizeof(unsigned int));
	if (references == NULL) {
		printCToolsMessage("addValue", "Not enough memory");
		free(newnode);
		free(newvalue);
		return;
	}
	*references = 1;
	list->copy(newvalue, value);
	*newnode = (struct _list_node_) { .value = newvalue, .references = references, .next = list->head->next };
	list->head->next = newnode;
}

/* Removes all occurrences of a given value */
void removeValue(linkedlist *list, void *value) {
	assert(value != NULL);
	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->equals != NULL);
	
	listiterator *iterator = createIterator(list);
	while (hasNext(iterator)) {
		void *nodevalue = getNext(iterator);
		assert(nodevalue != NULL);
		if (list->equals(nodevalue, value)) {
			removeCurrent(iterator);
		}
	}
	freeIterator(iterator);
}

/* Checks whether the list contains a value */
bool containsValue(linkedlist *list, void *value) {
	assert(value != NULL);
	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->equals != NULL);
	
	listiterator *iterator = createIterator(list);
	while (hasNext(iterator)) {
		void *nodevalue = getNext(iterator);
		assert(nodevalue != NULL);
		if (list->equals(nodevalue, value)) {
			freeIterator(iterator);
			return true;
		}
	}
	freeIterator(iterator);
	return false;
}

/* Returns a reference to the first element with the given value */
void* getItem(linkedlist *list, void *value) {
	assert(value != NULL);
	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->equals != NULL);
	
	listiterator *iterator = createIterator(list);
	while (hasNext(iterator)) {
		void *nodevalue = getNext(iterator);
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
	assert(funcp != NULL);
	assert(list != NULL);
	assert(list->head != NULL);
	
	listiterator *iterator = createIterator(list);
	while (hasNext(iterator)) {
		void *value = getNext(iterator);
		assert(value != NULL);
		(*funcp)(value);
	}
	freeIterator(iterator);
}

/* Generates a copy of the list */
linkedlist* copyList(linkedlist *list) {
	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->elemsize != 0);
	assert(list->copy != NULL);
	
	linkedlist *newlist = createList();
	newlist->equals = list->equals;
	newlist->copy = list->copy;
	newlist->free = list->free;
	struct _list_node_ *previousnode = newlist->head;
	listiterator *iterator = createIterator(list);
	while (hasNext(iterator)) {
		struct _list_node_ *newnode = malloc(sizeof(struct _list_node_));
		if (newnode == NULL) {
			printCToolsMessage("copyList","Not enough memory");
			freeList(newlist);
			freeIterator(iterator);
			return NULL;
		}
		void *value = getNext(iterator);
		assert(value != NULL);
		unsigned int *references = getCurrentReferences(iterator);
		assert(references != NULL);
		(*references)++;
		*newnode = (struct _list_node_) { .value = value, .next = NULL, .references = references };
		
		previousnode->next = newnode;
		previousnode = newnode;
	}
	freeIterator(iterator);
	return newlist;
}

/* Returns an array with a copy of each element of the list */
void* listToArray(linkedlist *list, unsigned int *length) {
	assert(length != NULL);
	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->elemsize != 0);
	assert(list->copy != NULL);
	
	unsigned int count = getLength(list);
	void *array = malloc(count * list->elemsize);
	if (array == NULL) {
		printCToolsMessage("listToArray", "Not enough memory");
		return NULL;
	}
	unsigned int i = 0;
	listiterator *iterator = createIterator(list);
	while (hasNext(iterator)) {
		void *value = getNext(iterator);
		assert(value != NULL);
		/* Casting the array to char* to be able to perform pointer arithmetic on a byte level */
		list->copy((char*) array + list->elemsize * i++, value);
	}
	freeIterator(iterator);
	*length = count;
	return array;
}

unsigned int getLength(linkedlist *list) {
	assert(list != NULL);
	assert(list->head != NULL);
	
	unsigned int length = 0;
	listiterator *iterator = createIterator(list);
	while (hasNext(iterator)) {
		getNext(iterator);
		length++;
	}
	freeIterator(iterator);
	return length;
}
