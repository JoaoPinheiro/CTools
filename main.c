#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* Required functions */
void printElement(void *val) {
	printf("- %d\n", *(int*) val);
}

int equals(void *a, void *b) {
	if (*(int*) a == *(int*) b) {
		return 1;
	}
	return 0;
}

void* constructor(void *data) {
	int *newdata = malloc(sizeof(int));
	*newdata = *(int*) data;
	return (void*) newdata;
}

void destructor(void *element) {
	free(element);
}

/* Automatic tests */
void testCreateAndFree() {
	linkedlist *list = NULL;

	printf("\n# Start testCreateAndFree()\n");
	list = createLinkedList();
	list->equals = &equals;
	list->constructor = &constructor;
	list->destructor = &destructor;

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
	int tmp = 0;
	linkedlist *list = createLinkedList();
	list->equals = &equals;
	list->constructor = &constructor;
	list->destructor = &destructor;

	printf("\n# Start testBasicOperations()\n");

	tmp = 1;
	removeNode(list, (void*) &tmp);

	if (containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 1) on list {}; shouldn't contain value\n");
		exit(-1);
	}

	addNode(list, (void*) &tmp);
	if (!containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 1) on list {1}; should contain value\n");
		exit(-1);
	}
	tmp = 9;
	if (containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 9) on list {1}; shouldn't contain value\n");
		exit(-1);
	}

	tmp = 1;
	removeNode(list, (void*) &tmp);
	if (containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 1) after removeNode(list, 1); shouldn't contain value\n");
		exit(-1);
	}

	freeLinkedList(list);

	printf("Passed\n");
}

void testSeveralItems() {
	int tmp = 0;
	linkedlist *list = createLinkedList();
	list->equals = &equals;
	list->constructor = &constructor;
	list->destructor = &destructor;

	printf("\n# Start testSeveralItems()\n");

	tmp = 1; addNode(list, (void*) &tmp);
	tmp = 2; addNode(list, (void*) &tmp);
	tmp = 3; addNode(list, (void*) &tmp);
	tmp = 4; addNode(list, (void*) &tmp);
	tmp = 5; addNode(list, (void*) &tmp);
	tmp = 6; addNode(list, (void*) &tmp);

	tmp = 0;
	if (!containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 0) on list {1, 2, 3, 4, 5, 6}; shouldn't contain value\n");
		exit(-1);
	}

	removeNode(list, (void*) &tmp);

	tmp = 1;
	if (!containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 1) on list {1, 2, 3, 4, 5, 6}; should contain value\n");
		exit(-1);
	}
	tmp = 6;
	if (!containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 6) on list {1, 2, 3, 4, 5, 6}; should contain value\n");
		exit(-1);
	}
	tmp = 4;
	if (!containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 4) on list {1, 2, 3, 4, 5, 6}; should contain value\n");
		exit(-1);
	}

	tmp = 1;
	removeNode(list, (void*) &tmp);
	if (containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 1) on list {2, 3, 4, 5, 6}; shouldn't contain value\n");
		exit(-1);
	}
	tmp = 2;
	if (!containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 2) on list {2, 3, 4, 5, 6}; should contain value\n");
		exit(-1);
	}

	tmp = 6;
	removeNode(list, (void*) &tmp);
	if (containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 6) on list {2, 3, 4, 5}; shouldn't contain value\n");
		exit(-1);
	}
	tmp = 5;
	if (!containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 5) on list {2, 3, 4, 5}; should contain value\n");
		exit(-1);
	}

	tmp = 4;
	removeNode(list, (void*) &tmp);
	if (containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 4) on list {2, 3, 5}; shouldn't contain value\n");
		exit(-1);
	}
	tmp = 3;
	if (!containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 3) on list {2, 3, 5}; should contain value\n");
		exit(-1);
	}

	freeLinkedList(list);

	printf("Passed\n");
}

void testRepeatItems() {
	int tmp = 0;
	linkedlist *list = createLinkedList();
	list->equals = &equals;
	list->constructor = &constructor;
	list->destructor = &destructor;

	printf("\n# Start testRepeatItems()\n");

	tmp = 1; addNode(list, (void*) &tmp);
	tmp = 2; addNode(list, (void*) &tmp);
	tmp = 3; addNode(list, (void*) &tmp);
	tmp = 1; addNode(list, (void*) &tmp);
	tmp = 2; addNode(list, (void*) &tmp);
	tmp = 3; addNode(list, (void*) &tmp);

	tmp = 0;
	if (containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 0) on list {1, 2, 3, 1, 2, 3}; shouldn't contain value\n");
		exit(-1);
	}
	tmp = 1;
	if (!containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 1) on list {1, 2, 3, 1, 2, 3}; should contain value\n");
		exit(-1);
	}
	tmp = 2;
	if (!containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 2) on list {1, 2, 3, 1, 2, 3}; should contain value\n");
		exit(-1);
	}
	tmp = 3;
	if (!containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 3) on list {1, 2, 3, 1, 2, 3}; should contain value\n");
		exit(-1);
	}

	tmp = 2;
	removeNode(list, (void*) &tmp);
	if (containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 1) on list {1, 3, 1, 3}; shouldn't contain value\n");
		exit(-1);
	}

	tmp = 1;
	removeNode(list, (void*) &tmp);
	if (containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 1) on list {3, 3}; shouldn't contain value\n");
		exit(-1);
	}

	tmp = 3;
	removeNode(list, (void*) &tmp);
	if (containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 3) on list {}; shouldn't contain value\n");
		exit(-1);
	}

	freeLinkedList(list);

	printf("Passed\n");
}

/* Non-automatic tests */
void testMapLinkedList() {
	int tmp = 0;
	linkedlist *list = createLinkedList();
	list->equals = &equals;
	list->constructor = &constructor;
	list->destructor = &destructor;

	printf("\n# Start testMapLinkedList()\n");
	printf("Print list {}:\n");
	mapLinkedList(list, &printElement);

	tmp = 1;
	addNode(list, (void*) &tmp);
	printf("Print list {1}:\n");
	mapLinkedList(list, &printElement);

	tmp = 2; addNode(list, (void*) &tmp);
	tmp = 3; addNode(list, (void*) &tmp);
	printf("Print list {1, 2, 3}:\n");
	mapLinkedList(list, &printElement);

	freeLinkedList(list);
}

void testInvalidArguments() {
	int tmp = 0;
	linkedlist *list = createLinkedList();
	list->equals = &equals;
	list->constructor = &constructor;
	list->destructor = &destructor;

	printf("\n# Start testInvalidArguments()\n");

	printf("freeLinkedList(NULL):\n");
	freeLinkedList(NULL);

	printf("addNode(NULL, 1):\n");
	tmp = 1;
	addNode(NULL, (void*) &tmp);

	printf("removeNode(NULL, 1):\n");
	removeNode(NULL, (void*) &tmp);

	printf("containsValue(NULL, 1):\n");
	containsValue(NULL, (void*) &tmp);

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

void testWithoutConstructor() {
	int tmp = 0;
	linkedlist *list = createLinkedList();
	list->equals = &equals;
	list->destructor = &destructor;

	printf("\n# Start testWithoutConstructor()\n");

	printf("addNode(list, {1, 2, 3}):\n");
	tmp = 1; addNode(list, (void*) &tmp);
	tmp = 2; addNode(list, (void*) &tmp);
	tmp = 3; addNode(list, (void*) &tmp);

	tmp = 1;
	printf("containsValue(list, 1):\n");
	containsValue(list, (void*) &tmp);

	printf("mapLinkedList(list, &printElement):\n");
	mapLinkedList(list, &printElement);

	printf("removeNode(list, 1):\n");
	removeNode(list, (void*) &tmp);

	printf("freeLinkedList(list):\n");
	freeLinkedList(list);
}

void testWithoutDestructor() {
	int tmp = 0;
	linkedlist *list = createLinkedList();
	list->equals = &equals;
	list->constructor = &constructor;

	printf("\n# Start testWithoutDestructor()\n");

	printf("addNode(list, {1, 2, 3}):\n");
	tmp = 1; addNode(list, (void*) &tmp);
	tmp = 2; addNode(list, (void*) &tmp);
	tmp = 3; addNode(list, (void*) &tmp);

	tmp = 1;
	printf("containsValue(list, 1):\n");
	containsValue(list, (void*) &tmp);

	printf("mapLinkedList(list, &printElement):\n");
	mapLinkedList(list, &printElement);

	printf("removeNode(list, 1):\n");
	removeNode(list, (void*) &tmp);

	printf("freeLinkedList(list):\n");
	freeLinkedList(list);
}

void testWithoutEquals() {
	int tmp = 0;
	linkedlist *list = createLinkedList();
	list->constructor = &constructor;
	list->destructor = &destructor;

	printf("\n# Start testWithoutEquals()\n");

	printf("addNode(list, {1, 2, 3}):\n");
	tmp = 1; addNode(list, (void*) &tmp);
	tmp = 2; addNode(list, (void*) &tmp);
	tmp = 3; addNode(list, (void*) &tmp);

	tmp = 1;
	printf("containsValue(list, 1):\n");
	containsValue(list, (void*) &tmp);

	printf("mapLinkedList(list, &printElement):\n");
	mapLinkedList(list, &printElement);

	printf("removeNode(list, 1):\n");
	removeNode(list, (void*) &tmp);

	printf("freeLinkedList(list):\n");
	freeLinkedList(list);
}

void* damagedConstructor(void *data) {
	return NULL;
}

void testDamagedList() {
	int tmp = 0;
	linkedlist *list = createLinkedList();
	list->equals = &equals;
	list->constructor = &damagedConstructor;
	list->destructor = &destructor;

	printf("\n# Start testDamagedList()\n");

	printf("addNode(list, {1, 2, 3}):\n");
	tmp = 1; addNode(list, (void*) &tmp);
	tmp = 2; addNode(list, (void*) &tmp);
	tmp = 3; addNode(list, (void*) &tmp);

	tmp = 1;
	printf("containsValue(list, 1):\n");
	containsValue(list, (void*) &tmp);

	printf("mapLinkedList(list, &printElement):\n");
	mapLinkedList(list, &printElement);

	printf("removeNode(list, 1):\n");
	removeNode(list, (void*) &tmp);

	printf("freeLinkedList(list):\n");
	freeLinkedList(list);
}

int main() {
	testCreateAndFree();
	testBasicOperations();
	testRepeatItems();
	testMapLinkedList();
	testInvalidArguments();
	testWithoutConstructor();
	testWithoutDestructor();
	testWithoutEquals();
	testDamagedList();

	return 0;
}