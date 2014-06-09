#include <stdio.h>
#include <stdlib.h>
#include "list.h"

static void printCToolsMessage(char *location, char *message) {
	if ((location != NULL) && (message != NULL)) {
		fprintf(stderr, "[CTools] %s: %s\n", location, message);
	}
}

/* Creates a new linked list structure */
linkedlist* createLinkedList() {
	linkedlist *list = NULL;
	struct _list_node_ *dummynode = NULL;

	list = malloc(sizeof(linkedlist));
	if (list == NULL) {
		printCToolsMessage("createLinkedList", "Not enough memory");
		return NULL;
	}

	dummynode = malloc(sizeof(struct _list_node_));
	if (dummynode == NULL) {
		printCToolsMessage("createLinkedList", "Not enough memory");
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
		printCToolsMessage("freeLinkedList", "List is NULL");
		return;
	} else if (list->head == NULL) {
		printCToolsMessage("freeLinkedList", "List HEAD is NULL");
		return;
	} else if (list->destructor == NULL) {
		printCToolsMessage("freeLinkedList", "Destructor function is NULL");
		return;
	}

	nextnode = list->head->next;
	currentnode = NULL;

	while (nextnode != NULL) {
		currentnode = nextnode;
		nextnode = currentnode->next;
		if (currentnode->value == NULL) {
			printCToolsMessage("freeLinkedList", "Contained value is NULL");
		} else {
			list->destructor(currentnode->value);
		}
		free(currentnode);
	}

	free(list->head);
	free(list);
}

/* Adds a new value to the beginning of the list */
void addNode(linkedlist *list, void *value) {
	struct _list_node_ *newnode = NULL;

	if (value == NULL) {
		printCToolsMessage("addNode","Value is NULL");
		return;
	} else if (list == NULL) {
		printCToolsMessage("addNode","List is NULL");
		return;
	} else if (list->head == NULL) {
		printCToolsMessage("addNode","List HEAD is NULL");
		return;
	} else if (list->constructor == NULL) {
		printCToolsMessage("addNode","Constructor function is NULL");
		return;
	}

	newnode = malloc(sizeof(struct _list_node_));
	if (newnode == NULL) {
		printCToolsMessage("addNode","Not enough memory");
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
		printCToolsMessage("removeNode", "Value is NULL");
		return;
	} else if (list == NULL) {
		printCToolsMessage("removeNode","List is NULL");
		return;
	} else if (list->head == NULL) {
		printCToolsMessage("removeNode","List HEAD is NULL");
		return;
	} else if (list->equals == NULL) {
		printCToolsMessage("removeNode","Equals function is NULL");
		return;
	} else if (list->destructor == NULL) {
		printCToolsMessage("removeNode","Destructor function is NULL");
		return;
	}

	previousnode = list->head;
	currentnode = list->head->next;

	while (currentnode != NULL) {
		if (currentnode->value == NULL) {
			printCToolsMessage("removeNode", "Contained value is NULL");
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
		printCToolsMessage("containsValue", "Value is NULL");
		return 0;
	} else if (list == NULL) {
		printCToolsMessage("containsValue", "List is NULL");
		return 0;
	} else if (list->head == NULL) {
		printCToolsMessage("containsValue", "List HEAD is NULL");
		return 0;
	} else if (list->equals == NULL) {
		printCToolsMessage("containsValue", "Equals function is NULL");
		return 0;
	}

	node = list->head->next;

	while (node != NULL) {
		if (node->value == NULL) {
			printCToolsMessage("containsValue", "Contained value is NULL");
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
		printCToolsMessage("mapLinkedList", "Function is NULL");
		return;
	} else if (list == NULL) {
		printCToolsMessage("mapLinkedList", "List is NULL");
		return;
	} else if (list->head == NULL) {
		printCToolsMessage("mapLinkedList", "List HEAD is NULL");
		return;
	}

	node = list->head->next;

	while (node != NULL) {
		if (node->value == NULL) {
			printCToolsMessage("mapLinkedList", "Contained value is NULL");
		} else {
			(*funcp)(node->value);
		}
		node = node->next;
	}
}
