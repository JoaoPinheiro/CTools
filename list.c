#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* Creates a new linked list structure */
linkedlist* createLinkedList() {
	linkedlist *list = NULL;
	struct _list_node_ *dummynode = NULL;

	list = malloc(sizeof(linkedlist));
	if (list == NULL) {
		fprintf(stderr, "[CTools] createLinkedList Error: Not enough memory\n");
		return NULL;
	}

	dummynode = malloc(sizeof(struct _list_node_));
	if (dummynode == NULL) {
		fprintf(stderr, "[CTools] createLinkedList Error: Not enough memory\n");
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
void freeLinkedList(linkedlist *list) {
	struct _list_node_ *nextnode = NULL;
	struct _list_node_ *currentnode = NULL;

	if (list == NULL) {
		fprintf(stderr, "[CTools] freeLinkedList Error: List is NULL\n");
		return;
	} else if (list->head == NULL) {
		fprintf(stderr, "[CTools] freeLinkedList Error: List HEAD is NULL\n");
		return;
	} else if (list->destructor == NULL) {
		fprintf(stderr, "[CTools] freeLinkedList Error: Destructor function is NULL\n");
		return;
	}

	nextnode = list->head;
	currentnode = NULL;

	while (nextnode != NULL) {
		currentnode = nextnode;
		nextnode = currentnode->next;
		if (currentnode->value == NULL) {
			fprintf(stderr, "[CTools] freeLinkedList Warning: Contained value is NULL\n");
		} else {
			list->destructor(currentnode->value);
		}
		free(currentnode);
	}

	free(list);
}

/* Adds a new value to the beginning of the list */
void addNode(linkedlist *list, void *value) {
	struct _list_node_ *newnode = NULL;

	if (value == NULL) {
		fprintf(stderr, "[CTools] addNode Error: Value is NULL\n");
		return;
	} else if (list == NULL) {
		fprintf(stderr, "[CTools] addNode Error: List is NULL\n");
		return;
	} else if (list->head == NULL) {
		fprintf(stderr, "[CTools] addNode Error: List HEAD is NULL\n");
		return;
	} else if (list->constructor == NULL) {
		fprintf(stderr, "[CTools] addNode Error: Constructor function is NULL\n");
		return;
	}

	newnode = malloc(sizeof(struct _list_node_));
	if (newnode == NULL) {
		fprintf(stderr, "[CTools] addNode Error: Not enough memory\n");
		return;
	}

	newnode->value = list->constructor(value);
	newnode->next = list->head->next;
	list->head->next = newnode;
}

/* Removes all occurrences of a given value */
void removeNode(linkedlist *list, void *value) {
	struct _list_node_ *previousnode = NULL;
	struct _list_node_ *currentnode = NULL;

	if (value == NULL) {
		fprintf(stderr, "[CTools] removeNode Error: Value is NULL\n");
		return;
	} else if (list == NULL) {
		fprintf(stderr, "[CTools] removeNode Error: List is NULL\n");
		return;
	} else if (list->head == NULL) {
		fprintf(stderr, "[CTools] removeNode Error: List HEAD is NULL\n");
		return;
	} else if (list->equals == NULL) {
		fprintf(stderr, "[CTools] removeNode Error: Equals function is NULL\n");
		return;
	} else if (list->destructor == NULL) {
		fprintf(stderr, "[CTools] removeNode Error: Destructor function is NULL\n");
		return;
	}

	previousnode = list->head;
	currentnode = list->head->next;

	while (currentnode != NULL) {
		if (currentnode->value == NULL) {
			fprintf(stderr, "[CTools] removeNode Warning: Contained value is NULL\n");
			currentnode = currentnode->next;
		} else if (list->equals(currentnode->value, value)) {
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

	if (value == NULL) {
		fprintf(stderr, "[CTools] containsValue Error: Value is NULL\n");
		return 0;
	} else if (list == NULL) {
		fprintf(stderr, "[CTools] containsValue Error: List is NULL\n");
		return 0;
	} else if (list->head == NULL) {
		fprintf(stderr, "[CTools] containsValue Error: List HEAD is NULL\n");
		return 0;
	} else if (list->equals == NULL) {
		fprintf(stderr, "[CTools] containsValue Error: Equals function is NULL\n");
		return 0;
	}

	node = list->head->next;

	while (node != NULL) {
		if (node->value == NULL) {
			fprintf(stderr, "[CTools] containsValue Warning: Contained value is NULL\n");
		} else if (list->equals(node->value, value)) {
			return 1;
		}
		node = node->next;
	}
	return 0;
}

/* Applies a function to each element of the list */
void mapLinkedList(linkedlist *list, void (*funcp)(void*)) {
	struct _list_node_ *node = NULL;

	if (funcp == NULL) {
		fprintf(stderr, "[CTools] mapLinkedList Error: Function is NULL\n");
		return;
	} else if (list == NULL) {
		fprintf(stderr, "[CTools] mapLinkedList Error: List is NULL\n");
		return;
	} else if (list->head == NULL) {
		fprintf(stderr, "[CTools] mapLinkedList Error: List HEAD is NULL\n");
		return;
	}

	node = list->head->next;

	while (node != NULL) {
		if (node->value == NULL) {
			fprintf(stderr, "[CTools] mapLinkedList Warning: Contained value is NULL\n");
		} else {
			(*funcp)(node->value);
		}
		node = node->next;
	}
}
