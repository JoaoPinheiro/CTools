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

	if (list == NULL) {
		printf("Failed to create list\n");
		exit(-1);
	} else if (list->head == NULL) {
		printf("Failed to create list head\n");
		exit(-1);
	}

	list->equals = &equals;
	list->constructor = &constructor;
	list->destructor = &destructor;

	freeLinkedList(list, DEEP);

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

	freeLinkedList(list, DEEP);

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

	freeLinkedList(list, DEEP);

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

	freeLinkedList(list, DEEP);

	printf("Passed\n");
}

void testGetValue() {
	int tmp = 0;
	int *value = NULL;
	linkedlist *list = createLinkedList();
	list->equals = &equals;
	list->constructor = &constructor;
	list->destructor = &destructor;

	printf("\n# Start testGetValue()\n");

	tmp = 0;
	value = (int*) getValue(list, &tmp);
	if (!(value == NULL)) {
		printf("Error in getValue(list, 0); on list {}; shouldn't contain value\n");
		exit(-1);
	}

	tmp = 1; addNode(list, (void*) &tmp);
	tmp = 2; addNode(list, (void*) &tmp);
	tmp = 3; addNode(list, (void*) &tmp);

	value = (int*) getValue(list, &tmp);
	if (value == NULL) {
		printf("Error in getValue(list, 3); on list {1, 2, 3}; should contain value\n");
		exit(-1);
	} else if (!(*value == 3)) {
		printf("Error in getValue(list, 3); on list {1, 2, 3}; wrong value\n");
		exit(-1);
	}

	*value = 4;
	tmp = 4;
	if (!containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 4) on list {1, 2, 4}; should contain value\n");
		exit(-1);
	}

	tmp = 3;
	if (containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 3) on list {1, 2, 4}; shouldn't contain value\n");
		exit(-1);
	}

	freeLinkedList(list, DEEP);

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

	freeLinkedList(list, DEEP);
}

void testDeepCopyLinkedList() {
	int tmp = 0;
	int *value = 0;
	linkedlist *newlist = NULL;
	linkedlist *list = createLinkedList();
	list->equals = &equals;
	list->constructor = &constructor;
	list->destructor = &destructor;

	printf("\n# Start testDeepCopyLinkedList()\n");

	newlist = copyLinkedList(list, DEEP);
	if (newlist == NULL) {
		printf("Error in copyLinkedList(list, DEEP); newlist is NULL\n");
		exit(-1);
	}

	printf("Print newlist {}:\n");
	mapLinkedList(newlist, &printElement);

	tmp = 1; addNode(list, (void*) &tmp);
	tmp = 2; addNode(list, (void*) &tmp);
	tmp = 3; addNode(list, (void*) &tmp);

	printf("Print list {1, 2, 3}:\n");
	mapLinkedList(list, &printElement);
	printf("Print new list {}:\n");
	mapLinkedList(newlist, &printElement);
	freeLinkedList(newlist, DEEP);

	newlist = NULL;
	newlist = copyLinkedList(list, DEEP);
	if (newlist == NULL) {
		printf("Error in copyLinkedList(list, DEEP); newlist is NULL\n");
		exit(-1);
	}

	printf("Print list {1, 2, 3}:\n");
	mapLinkedList(list, &printElement);
	printf("Print new list {1, 2, 3}:\n");
	mapLinkedList(newlist, &printElement);

	tmp = 2;
	value = (int*) getValue(list, &tmp);

	*value = 4;
	tmp = 4;
	if (containsValue(newlist, (void*) &tmp)) {
		printf("Error in containsValue(newlist, 4) on list {1, 2, 3}; shouldn't contain value\n");
		exit(-1);
	}

	printf("Print list {1, 4, 3}:\n");
	mapLinkedList(list, &printElement);
	printf("Print new list {1, 2, 3}:\n");
	mapLinkedList(newlist, &printElement);

	freeLinkedList(newlist, DEEP);
	freeLinkedList(list, DEEP);
}

void testShallowCopyLinkedList() {
	int tmp = 0;
	int *value = 0;
	linkedlist *newlist = NULL;
	linkedlist *list = createLinkedList();
	list->equals = &equals;
	list->constructor = &constructor;
	list->destructor = &destructor;

	printf("\n# Start testShallowCopyLinkedList()\n");

	newlist = copyLinkedList(list, SHALLOW);
	if (newlist == NULL) {
		printf("Error in copyLinkedList(list, SHALLOW); newlist is NULL\n");
		exit(-1);
	}

	printf("Print newlist {}:\n");
	mapLinkedList(newlist, &printElement);

	tmp = 1; addNode(list, (void*) &tmp);
	tmp = 2; addNode(list, (void*) &tmp);
	tmp = 3; addNode(list, (void*) &tmp);

	printf("Print list {1, 2, 3}:\n");
	mapLinkedList(list, &printElement);
	printf("Print new list {}:\n");
	mapLinkedList(newlist, &printElement);
	freeLinkedList(newlist, SHALLOW);

	newlist = NULL;
	newlist = copyLinkedList(list, SHALLOW);
	if (newlist == NULL) {
		printf("Error in copyLinkedList(list, SHALLOW); newlist is NULL\n");
		exit(-1);
	}

	printf("Print list {1, 2, 3}:\n");
	mapLinkedList(list, &printElement);
	printf("Print new list {1, 2, 3}:\n");
	mapLinkedList(newlist, &printElement);

	tmp = 2;
	value = (int*) getValue(list, &tmp);

	*value = 4;
	tmp = 4;
	if (!containsValue(newlist, (void*) &tmp)) {
		printf("Error in containsValue(newlist, 4) on list {1, 4, 3}; should contain value\n");
		exit(-1);
	}

	printf("Print list {1, 4, 3}:\n");
	mapLinkedList(list, &printElement);
	printf("Print new list {1, 4, 3}:\n");
	mapLinkedList(newlist, &printElement);

	freeLinkedList(newlist, SHALLOW);
	freeLinkedList(list, DEEP);
}

void testLinkedListToArray() {
	int tmp = 0;
	int i = 0;
	int elements = 0;
	int size = 0;
	int **array = NULL;
	linkedlist *list = createLinkedList();
	list->equals = &equals;
	list->constructor = &constructor;
	list->destructor = &destructor;

	printf("\n# Start testLinkedListToArray()\n");

	array = (int**) linkedListToArray(list, &size);
	if (size != 0) {
		printf("Error in linkedListToArray(list, size) for list {}; wrong number of elements\n");
		exit(-1);
	} else if (array == NULL) {
		printf("Error in linkedListToArray(list, size) for list {}; array is NULL\n");
		exit(-1);
	}
	free(array);

	array = NULL;
	array = (int**) linkedListToArray(list, &size);
	elements = 5;
	for (i = 0; i < elements; i++) {
		tmp = i + 1;
		addNode(list, (void*) &tmp);
	}

	printf("Print list:\n");
	mapLinkedList(list, &printElement);

	array = (int**) linkedListToArray(list, &size);
	if (size != elements) {
		printf("Error in linkedListToArray(list, size); wrong number of elements\n");
		exit(-1);
	} else if (array == NULL) {
		printf("Error in linkedListToArray(list, size); array is NULL\n");
		exit(-1);
	}

	printf("Print array:\n");
	for (i = 0; i < size; i++) {
		printf("array[%d]: %d\n", i, *(array[i]));
	}

	*(array[0]) = -1;
	tmp = -1;
	if (!containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 3) on list {1, 2, 3, 4, -1}; should contain value\n");
		exit(-1);
	}

	free(array);
	freeLinkedList(list, DEEP);
}

void* constructorNULL(void *data) {
	return NULL;
}

void testListWithNULL() {
	int tmp = 0;
	int size = 0;
	linkedlist *newlist = NULL;
	linkedlist *list = createLinkedList();
	list->equals = &equals;
	list->constructor = &constructorNULL;
	list->destructor = &destructor;

	printf("\n# Start testListWithNULL()\n");

	printf("addNode(list, {1, 2, 3}):\n");
	tmp = 1; addNode(list, (void*) &tmp);
	tmp = 2; addNode(list, (void*) &tmp);
	tmp = 3; addNode(list, (void*) &tmp);

	list->constructor = &constructor;

	tmp = 1;
	printf("containsValue(list, 1):\n");
	containsValue(list, (void*) &tmp);

	printf("getValue(list, 1):\n");
	getValue(list, (void*) &tmp);

	printf("mapLinkedList(list, &printElement):\n");
	mapLinkedList(list, &printElement);

	printf("copyLinkedList(list, DEEP):\n");
	newlist = copyLinkedList(list, DEEP);

	printf("mapLinkedList(newlist, &printElement):\n");
	mapLinkedList(newlist, &printElement);

	printf("linkedListToArray(list, &size):\n");
	linkedListToArray(list, &size);

	printf("removeNode(list, 1):\n");
	removeNode(list, (void*) &tmp);

	printf("freeLinkedList(list, DEEP):\n");
	freeLinkedList(list, DEEP);

	printf("freeLinkedList(newlist, DEEP):\n");
	freeLinkedList(newlist, DEEP);
}

int main() {
	testCreateAndFree();
	getc(stdin);
	testBasicOperations();
	getc(stdin);
	testRepeatItems();
	getc(stdin);
	testGetValue();
	getc(stdin);
	testMapLinkedList();
	getc(stdin);
	testDeepCopyLinkedList();
	getc(stdin);
	testShallowCopyLinkedList();
	getc(stdin);
	testLinkedListToArray();
	getc(stdin);
	testListWithNULL();

	return 0;
}