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
	list->head = dummynode;
	list->equals = NULL;
	list->destructor = NULL;
	list->constructor = NULL;
	return list;
}

/* Releases all the memory allocated by the linked list */
void freeList(linkedlist *list, copytype type) {
	struct _list_node_ *nextnode = NULL;
	struct _list_node_ *currentnode = NULL;

	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->destructor != NULL);

	nextnode = list->head->next;
	currentnode = NULL;
	while (nextnode != NULL) {
		currentnode = nextnode;
		nextnode = currentnode->next;
		assert(currentnode->value != NULL);
		if (type == DEEP) {
			list->destructor(currentnode->value);
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
	assert(list->constructor != NULL);

	newnode = malloc(sizeof(struct _list_node_));
	if (newnode == NULL) {
		printCToolsMessage("addValue","Not enough memory");
		return;
	}

	newvalue = list->constructor(value);
	newnode->value = newvalue;
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
	assert(list->destructor != NULL);

	previousnode = list->head;
	currentnode = list->head->next;
	while (currentnode != NULL) {
		assert(currentnode->value != NULL);
		if (list->equals(currentnode->value, value)) {
			previousnode->next = currentnode->next;
			list->destructor(currentnode->value);
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
linkedlist* copyList(linkedlist *list, copytype type) {
	linkedlist *newlist = NULL;
	struct _list_node_ *newnode = NULL;
	struct _list_node_ *previousnode = NULL;
	struct _list_node_ *node = NULL;
	void *newvalue = NULL;

	assert(list != NULL);
	assert(list->head != NULL);
	assert(list->constructor != NULL);

	newlist = createList();
	newlist->equals = list->equals;
	newlist->constructor = list->constructor;
	newlist->destructor = list->destructor;

	previousnode = newlist->head;
	node = list->head->next;
	while (node != NULL) {
		newnode = malloc(sizeof(struct _list_node_));
		if (newnode == NULL) {
			printCToolsMessage("copyList","Not enough memory");
			freeList(newlist, type);
			return NULL;
		}
		assert(node->value != NULL);
		if (type == DEEP) {
			newvalue = list->constructor(node->value);
			if (newvalue == NULL) {
				printCToolsMessage("copyList","Not enough memory");
				freeList(newlist, DEEP);
				return NULL;
			}
		} else {
			newvalue = node->value;
		}
		newnode->value = newvalue;
		newnode->next = NULL;
		previousnode->next = newnode;
		previousnode = newnode;
		node = node->next;
	}

	return newlist;
}

/* Returns an array with pointers to all the elements of the list */
void** listToArray(linkedlist *list, int *size) {
	struct _list_node_ *node = NULL;
	void **array = NULL;
	int count = 0;
	int i = 0;

	assert(size != NULL);
	assert(list != NULL);
	assert(list->head != NULL);

	node = list->head->next;
	count = 0;
	while (node != NULL) {
		count++;
		node = node->next;
	}

	array = malloc(count * sizeof(void*));
	if (array == NULL) {
		printCToolsMessage("listToArray", "Not enough memory");
		return NULL;
	}

	i = 0;
	node = list->head->next;
	while (node != NULL) {
		assert(node->value != NULL);
		array[i++] = node->value;
		node = node->next;
	}

	*size = count;
	return array;
}
