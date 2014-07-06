#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

/* Required functions */
void printElement(void *val) {
	assert(val != NULL);
	printf("- %d\n", *(int*) val);
}

int equals(void *a, void *b) {
	assert(a != NULL);
	assert(b != NULL);
	if (*(int*) a == *(int*) b) {
		return 1;
	}
	return 0;
}

void* constructor(void *data) {
	int *newdata = NULL;
	assert(data != NULL);
	newdata = malloc(sizeof(int));
	*newdata = *(int*) data;
	return (void*) newdata;
}

void destructor(void *element) {
	assert(element != NULL);
	free(element);
}

/* Automatic tests */
void testCreateAndFree() {
	linkedlist *list = NULL;

	printf("\n# Start testCreateAndFree()\n");
	list = createList();

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

	freeList(list, DEEP);

	printf("Passed\n");
}

void testBasicOperations() {
	int tmp = 0;
	linkedlist *list = createList();
	list->equals = &equals;
	list->constructor = &constructor;
	list->destructor = &destructor;

	printf("\n# Start testBasicOperations()\n");

	tmp = 1;
	removeValue(list, (void*) &tmp);

	if (containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 1) on list {}; shouldn't contain value\n");
		exit(-1);
	}

	addValue(list, (void*) &tmp);
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
	removeValue(list, (void*) &tmp);
	if (containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 1) after removeValue(list, 1); shouldn't contain value\n");
		exit(-1);
	}

	freeList(list, DEEP);

	printf("Passed\n");
}

void testSeveralItems() {
	int tmp = 0;
	linkedlist *list = createList();
	list->equals = &equals;
	list->constructor = &constructor;
	list->destructor = &destructor;

	printf("\n# Start testSeveralItems()\n");

	tmp = 1; addValue(list, (void*) &tmp);
	tmp = 2; addValue(list, (void*) &tmp);
	tmp = 3; addValue(list, (void*) &tmp);
	tmp = 4; addValue(list, (void*) &tmp);
	tmp = 5; addValue(list, (void*) &tmp);
	tmp = 6; addValue(list, (void*) &tmp);

	tmp = 0;
	if (!containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 0) on list {1, 2, 3, 4, 5, 6}; shouldn't contain value\n");
		exit(-1);
	}

	removeValue(list, (void*) &tmp);

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
	removeValue(list, (void*) &tmp);
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
	removeValue(list, (void*) &tmp);
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
	removeValue(list, (void*) &tmp);
	if (containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 4) on list {2, 3, 5}; shouldn't contain value\n");
		exit(-1);
	}
	tmp = 3;
	if (!containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 3) on list {2, 3, 5}; should contain value\n");
		exit(-1);
	}

	freeList(list, DEEP);

	printf("Passed\n");
}

void testRepeatItems() {
	int tmp = 0;
	linkedlist *list = createList();
	list->equals = &equals;
	list->constructor = &constructor;
	list->destructor = &destructor;

	printf("\n# Start testRepeatItems()\n");

	tmp = 1; addValue(list, (void*) &tmp);
	tmp = 2; addValue(list, (void*) &tmp);
	tmp = 3; addValue(list, (void*) &tmp);
	tmp = 1; addValue(list, (void*) &tmp);
	tmp = 2; addValue(list, (void*) &tmp);
	tmp = 3; addValue(list, (void*) &tmp);

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
	removeValue(list, (void*) &tmp);
	if (containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 1) on list {1, 3, 1, 3}; shouldn't contain value\n");
		exit(-1);
	}

	tmp = 1;
	removeValue(list, (void*) &tmp);
	if (containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 1) on list {3, 3}; shouldn't contain value\n");
		exit(-1);
	}

	tmp = 3;
	removeValue(list, (void*) &tmp);
	if (containsValue(list, (void*) &tmp)) {
		printf("Error in containsValue(list, 3) on list {}; shouldn't contain value\n");
		exit(-1);
	}

	freeList(list, DEEP);

	printf("Passed\n");
}

void testgetItem() {
	int tmp = 0;
	int *value = NULL;
	linkedlist *list = createList();
	list->equals = &equals;
	list->constructor = &constructor;
	list->destructor = &destructor;

	printf("\n# Start testgetItem()\n");

	tmp = 0;
	value = (int*) getItem(list, &tmp);
	if (!(value == NULL)) {
		printf("Error in getItem(list, 0); on list {}; shouldn't contain value\n");
		exit(-1);
	}

	tmp = 1; addValue(list, (void*) &tmp);
	tmp = 2; addValue(list, (void*) &tmp);
	tmp = 3; addValue(list, (void*) &tmp);

	value = (int*) getItem(list, &tmp);
	if (value == NULL) {
		printf("Error in getItem(list, 3); on list {1, 2, 3}; should contain value\n");
		exit(-1);
	} else if (!(*value == 3)) {
		printf("Error in getItem(list, 3); on list {1, 2, 3}; wrong value\n");
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

	freeList(list, DEEP);

	printf("Passed\n");
}

/* Non-automatic tests */
void testMapList() {
	int tmp = 0;
	linkedlist *list = createList();
	list->equals = &equals;
	list->constructor = &constructor;
	list->destructor = &destructor;

	printf("\n# Start testMapList()\n");
	printf("Print list {}:\n");
	mapList(list, &printElement);

	tmp = 1;
	addValue(list, (void*) &tmp);
	printf("Print list {1}:\n");
	mapList(list, &printElement);

	tmp = 2; addValue(list, (void*) &tmp);
	tmp = 3; addValue(list, (void*) &tmp);
	printf("Print list {1, 2, 3}:\n");
	mapList(list, &printElement);

	freeList(list, DEEP);
}

void testDeepCopyList() {
	int tmp = 0;
	int *value = 0;
	linkedlist *newlist = NULL;
	linkedlist *list = createList();
	list->equals = &equals;
	list->constructor = &constructor;
	list->destructor = &destructor;

	printf("\n# Start testDeepCopyList()\n");

	newlist = copyList(list, DEEP);
	if (newlist == NULL) {
		printf("Error in copyList(list, DEEP); newlist is NULL\n");
		exit(-1);
	}

	printf("Print newlist {}:\n");
	mapList(newlist, &printElement);

	tmp = 1; addValue(list, (void*) &tmp);
	tmp = 2; addValue(list, (void*) &tmp);
	tmp = 3; addValue(list, (void*) &tmp);

	printf("Print list {1, 2, 3}:\n");
	mapList(list, &printElement);
	printf("Print new list {}:\n");
	mapList(newlist, &printElement);
	freeList(newlist, DEEP);

	newlist = NULL;
	newlist = copyList(list, DEEP);
	if (newlist == NULL) {
		printf("Error in copyList(list, DEEP); newlist is NULL\n");
		exit(-1);
	}

	printf("Print list {1, 2, 3}:\n");
	mapList(list, &printElement);
	printf("Print new list {1, 2, 3}:\n");
	mapList(newlist, &printElement);

	tmp = 2;
	value = (int*) getItem(list, &tmp);

	*value = 4;
	tmp = 4;
	if (containsValue(newlist, (void*) &tmp)) {
		printf("Error in containsValue(newlist, 4) on list {1, 2, 3}; shouldn't contain value\n");
		exit(-1);
	}

	printf("Print list {1, 4, 3}:\n");
	mapList(list, &printElement);
	printf("Print new list {1, 2, 3}:\n");
	mapList(newlist, &printElement);

	freeList(newlist, DEEP);
	freeList(list, DEEP);
}

void testShallowCopyList() {
	int tmp = 0;
	int *value = 0;
	linkedlist *newlist = NULL;
	linkedlist *list = createList();
	list->equals = &equals;
	list->constructor = &constructor;
	list->destructor = &destructor;

	printf("\n# Start testShallowCopyList()\n");

	newlist = copyList(list, SHALLOW);
	if (newlist == NULL) {
		printf("Error in copyList(list, SHALLOW); newlist is NULL\n");
		exit(-1);
	}

	printf("Print newlist {}:\n");
	mapList(newlist, &printElement);

	tmp = 1; addValue(list, (void*) &tmp);
	tmp = 2; addValue(list, (void*) &tmp);
	tmp = 3; addValue(list, (void*) &tmp);

	printf("Print list {1, 2, 3}:\n");
	mapList(list, &printElement);
	printf("Print new list {}:\n");
	mapList(newlist, &printElement);
	freeList(newlist, SHALLOW);

	newlist = NULL;
	newlist = copyList(list, SHALLOW);
	if (newlist == NULL) {
		printf("Error in copyList(list, SHALLOW); newlist is NULL\n");
		exit(-1);
	}

	printf("Print list {1, 2, 3}:\n");
	mapList(list, &printElement);
	printf("Print new list {1, 2, 3}:\n");
	mapList(newlist, &printElement);

	tmp = 2;
	value = (int*) getItem(list, &tmp);

	*value = 4;
	tmp = 4;
	if (!containsValue(newlist, (void*) &tmp)) {
		printf("Error in containsValue(newlist, 4) on list {1, 4, 3}; should contain value\n");
		exit(-1);
	}

	printf("Print list {1, 4, 3}:\n");
	mapList(list, &printElement);
	printf("Print new list {1, 4, 3}:\n");
	mapList(newlist, &printElement);

	freeList(newlist, SHALLOW);
	freeList(list, DEEP);
}

void testListToArray() {
	int tmp = 0;
	int i = 0;
	int elements = 0;
	int size = 0;
	int **array = NULL;
	linkedlist *list = createList();
	list->equals = &equals;
	list->constructor = &constructor;
	list->destructor = &destructor;

	printf("\n# Start testListToArray()\n");

	array = (int**) listToArray(list, &size);
	if (size != 0) {
		printf("Error in listToArray(list, size) for list {}; wrong number of elements\n");
		exit(-1);
	} else if (array == NULL) {
		printf("Error in listToArray(list, size) for list {}; array is NULL\n");
		exit(-1);
	}
	free(array);

	array = NULL;
	array = (int**) listToArray(list, &size);
	elements = 5;
	for (i = 0; i < elements; i++) {
		tmp = i + 1;
		addValue(list, (void*) &tmp);
	}

	printf("Print list:\n");
	mapList(list, &printElement);

	array = (int**) listToArray(list, &size);
	if (size != elements) {
		printf("Error in listToArray(list, size); wrong number of elements\n");
		exit(-1);
	} else if (array == NULL) {
		printf("Error in listToArray(list, size); array is NULL\n");
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
	freeList(list, DEEP);
}

int main() {
	testCreateAndFree();
	getc(stdin);
	testBasicOperations();
	getc(stdin);
	testRepeatItems();
	getc(stdin);
	testgetItem();
	getc(stdin);
	testMapList();
	getc(stdin);
	testDeepCopyList();
	getc(stdin);
	testShallowCopyList();
	getc(stdin);
	testListToArray();

	return 0;
}
