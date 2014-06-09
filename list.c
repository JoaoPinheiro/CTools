#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* Creates a new linked list structure */
linkedlist* createLinkedList() {
	linkedlist *list = NULL;
	struct _list_node_ *dummynode = NULL;

	list = malloc(sizeof(linkedlist));
	if (list == NULL) {
		fprintf(stderr, "[CTools] createLinkedList: Not enough memory\n");
		return NULL;
	}

	dummynode = malloc(sizeof(struct _list_node_));
	if (dummynode == NULL) {
		fprintf(stderr, "[CTools] createLinkedList: Not enough memory\n");
		return NULL;
	}

	dummynode->next = NULL;
	list->head = dummynode;
	return list;
}

/* Releases all the memory allocated by the linked list */
void freeLinkedList(linkedlist *list) {
	struct _list_node_ *nextnode = NULL;
	struct _list_node_ *currentnode = NULL;

	if (list == NULL) {
		fprintf(stderr, "[CTools] freeLinkedList: List is NULL\n");
		return;
	} else if (list->head == NULL) {
		fprintf(stderr, "[CTools] freeLinkedList: List HEAD is NULL\n");
		return;
	}

	nextnode = list->head;
	currentnode = NULL;

	while (nextnode != NULL) {
		currentnode = nextnode;
		nextnode = currentnode->next;
		free(currentnode);
	}
	free(list);
}

/* Adds a new value to the beginning of the list */
void addNode(linkedlist *list, int value) {
	struct _list_node_ *newnode = NULL;

	if (list == NULL) {
		fprintf(stderr, "[CTools] addNode: List is NULL\n");
		return;
	} else if (list->head == NULL) {
		fprintf(stderr, "[CTools] addNode: List HEAD is NULL\n");
		return;
	}

	newnode = malloc(sizeof(struct _list_node_));
	if (newnode == NULL) {
		fprintf(stderr, "[CTools] addNode: Not enough memory\n");
		return;
	}

	newnode->value = value;
	newnode->next = list->head->next;
	list->head->next = newnode;
}

/* Removes all occurrences of a given value */
void removeNode(linkedlist *list, int value) {
	struct _list_node_ *previousnode = NULL;
	struct _list_node_ *currentnode = NULL;

	if (list == NULL) {
		fprintf(stderr, "[CTools] removeNode: List is NULL\n");
		return;
	} else if (list->head == NULL) {
		fprintf(stderr, "[CTools] removeNode: List HEAD is NULL\n");
		return;
	}

	previousnode = list->head;
	currentnode = list->head->next;

	while (currentnode != NULL) {
		if (currentnode->value == value) {
			previousnode->next = currentnode->next;
			free(currentnode);
			currentnode = previousnode->next;
		} else {
			previousnode = currentnode;
			currentnode = currentnode->next;
		}
	}
}

/* Checks whether the list contains a value */
int containsValue(linkedlist *list, int value) {
	struct _list_node_ *node = NULL;

	if (list == NULL) {
		fprintf(stderr, "[CTools] containsValue: List is NULL\n");
		return 0;
	} else if (list->head == NULL) {
		fprintf(stderr, "[CTools] containsValue: List HEAD is NULL\n");
		return 0;
	}

	node = list->head->next;

	while (node != NULL) {
		if (node->value == value)
			return 1;
		node = node->next;
	}
	return 0;
}

/* Applies a function to each element of the list */
void mapLinkedList(linkedlist *list, void (*funcp)(int)) {
	struct _list_node_ *node = NULL;

	if (list == NULL) {
		fprintf(stderr, "[CTools] mapLinkedList: List is NULL\n");
		return;
	} else if (list->head == NULL) {
		fprintf(stderr, "[CTools] mapLinkedList: List HEAD is NULL\n");
		return;
	} else if (funcp == NULL) {
		fprintf(stderr, "[CTools] mapLinkedList: Function is NULL\n");
		return;
	}

	node = list->head->next;

	while (node != NULL) {
		(*funcp)(node->value);
		node = node->next;
	}
}
