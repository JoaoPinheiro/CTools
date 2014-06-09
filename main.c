#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void printElement(int val) {
	printf("- %d\n", val);
}

void testCreateAndFree() {
	linkedlist *list = NULL;

	printf("# Start testCreateAndFree()\n");
	list = createLinkedList();

	if (list == NULL) {
		printf("Failed to create list\n");
		exit(-1);
	} else if (list->head == NULL) {
		printf("Failed to create list head\n");
		exit(-1);
	}

	freeLinkedList(list);

	printf("Passed\n");
}

void testBasicOperations() {
	linkedlist *list = createLinkedList();

	printf("# Start testBasicOperations()\n");

	removeNode(list, 1);

	if (containsValue(list, 1)) {
		printf("Error in containsValue(list, 1) on list {}; shouldn't contain value\n");
		exit(-1);
	}

	addNode(list, 1);
	if (!containsValue(list, 1)) {
		printf("Error in containsValue(list, 1) on list {1}; should contain value\n");
		exit(-1);
	}
	if (containsValue(list, 9)) {
		printf("Error in containsValue(list, 9) on list {1}; shouldn't contain value\n");
		exit(-1);
	}

	removeNode(list, 1);
	if (containsValue(list, 1)) {
		printf("Error in containsValue(list, 1) after removeNode(list, 1); shouldn't contain value\n");
		exit(-1);
	}

	freeLinkedList(list);

	printf("Passed\n");
}

void testSeveralItems() {
	linkedlist *list = createLinkedList();

	printf("# Start testSeveralItems()\n");

	addNode(list, 1);
	addNode(list, 2);
	addNode(list, 3);
	addNode(list, 4);
	addNode(list, 5);
	addNode(list, 6);

	if (!containsValue(list, 0)) {
		printf("Error in containsValue(list, 0) on list {1, 2, 3, 4, 5, 6}; shouldn't contain value\n");
		exit(-1);
	}

	removeNode(list, 0);

	if (!containsValue(list, 1)) {
		printf("Error in containsValue(list, 1) on list {1, 2, 3, 4, 5, 6}; should contain value\n");
		exit(-1);
	}
	if (!containsValue(list, 6)) {
		printf("Error in containsValue(list, 6) on list {1, 2, 3, 4, 5, 6}; should contain value\n");
		exit(-1);
	}
	if (!containsValue(list, 4)) {
		printf("Error in containsValue(list, 4) on list {1, 2, 3, 4, 5, 6}; should contain value\n");
		exit(-1);
	}

	removeNode(list, 1);
	if (containsValue(list, 1)) {
		printf("Error in containsValue(list, 1) on list {2, 3, 4, 5, 6}; shouldn't contain value\n");
		exit(-1);
	}
	if (!containsValue(list, 2)) {
		printf("Error in containsValue(list, 2) on list {2, 3, 4, 5, 6}; should contain value\n");
		exit(-1);
	}

	removeNode(list, 6);
	if (containsValue(list, 6)) {
		printf("Error in containsValue(list, 6) on list {2, 3, 4, 5}; shouldn't contain value\n");
		exit(-1);
	}
	if (!containsValue(list, 5)) {
		printf("Error in containsValue(list, 5) on list {2, 3, 4, 5}; should contain value\n");
		exit(-1);
	}

	removeNode(list, 4);
	if (containsValue(list, 4)) {
		printf("Error in containsValue(list, 4) on list {2, 3, 5}; shouldn't contain value\n");
		exit(-1);
	}
	if (!containsValue(list, 3)) {
		printf("Error in containsValue(list, 3) on list {2, 3, 5}; should contain value\n");
		exit(-1);
	}

	freeLinkedList(list);

	printf("Passed\n");
}

void testRepeatItems() {
	linkedlist *list = createLinkedList();

	printf("# Start testRepeatItems()\n");

	addNode(list, 1);
	addNode(list, 2);
	addNode(list, 3);
	addNode(list, 1);
	addNode(list, 2);
	addNode(list, 3);


	if (containsValue(list, 0)) {
		printf("Error in containsValue(list, 0) on list {1, 2, 3, 1, 2, 3}; shouldn't contain value\n");
		exit(-1);
	}
	if (!containsValue(list, 1)) {
		printf("Error in containsValue(list, 1) on list {1, 2, 3, 1, 2, 3}; should contain value\n");
		exit(-1);
	}
	if (!containsValue(list, 2)) {
		printf("Error in containsValue(list, 2) on list {1, 2, 3, 1, 2, 3}; should contain value\n");
		exit(-1);
	}
	if (!containsValue(list, 3)) {
		printf("Error in containsValue(list, 3) on list {1, 2, 3, 1, 2, 3}; should contain value\n");
		exit(-1);
	}

	removeNode(list, 2);
	if (containsValue(list, 2)) {
		printf("Error in containsValue(list, 1) on list {1, 3, 1, 3}; shouldn't contain value\n");
		exit(-1);
	}

	removeNode(list, 1);
	if (containsValue(list, 1)) {
		printf("Error in containsValue(list, 1) on list {3, 3}; shouldn't contain value\n");
		exit(-1);
	}

	removeNode(list, 3);
	if (containsValue(list, 3)) {
		printf("Error in containsValue(list, 3) on list {}; shouldn't contain value\n");
		exit(-1);
	}

	freeLinkedList(list);

	printf("Passed\n");
}

void testMapLinkedList() {
	linkedlist *list = createLinkedList();

	printf("# Start testMapLinkedList()\n");
	printf("Print empty list:\n");
	mapLinkedList(list, &printElement);

	addNode(list, 1);
	printf("Print list {1}:\n");
	mapLinkedList(list, &printElement);

	addNode(list, 2);
	addNode(list, 3);
	printf("Print list {1, 2, 3}:\n");
	mapLinkedList(list, &printElement);

	freeLinkedList(list);

	printf("Passed\n");
}

void testInvalidArguments() {
	linkedlist *list = createLinkedList();

	printf("# Start testInvalidArguments()\n");

	printf("freeLinkedList(NULL):\n");
	freeLinkedList(NULL);

	printf("addNode(NULL, 1):\n");
	addNode(NULL, 1);

	printf("removeNode(NULL, 1):\n");
	removeNode(NULL, 1);

	printf("containsValue(NULL, 1):\n");
	containsValue(NULL, 1);

	printf("mapLinkedList(NULL, NULL):\n");
	mapLinkedList(NULL, NULL);

	printf("mapLinkedList(NULL, &printElement):\n");
	mapLinkedList(NULL, &printElement);

	printf("mapLinkedList(list, NULL):\n");
	mapLinkedList(list, NULL);

	printf("freeLinkedList(NULL):\n");
	freeLinkedList(NULL);

	freeLinkedList(list);
}

int main() {
	testCreateAndFree();
	testBasicOperations();
	testRepeatItems();
	testMapLinkedList();
	testInvalidArguments();

	return 0;
}