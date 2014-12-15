#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct _euromillions_draw_ {
	unsigned int star1, star2;
} draw;

/* Required functions */
void printElement(void *val) {
	printf("%d, %d\n", ((draw*) val)->star1, ((draw*) val)->star2);
}

int equalsDraw(void *a, void *b) {
	return ((draw*) a)->star1 == ((draw*) b)->star1 &&
			((draw*) a)->star2 == ((draw*) b)->star2;
}

void copyDraw(void *destination, void *origin) {
	*(draw*) destination = *(draw*) origin;
}

void freeDraw(void *element) {
	free(element);
}

int main() {
	FILE* fp;
	draw tmp;
	draw *next;
	unsigned int sum, count;
	double average, average9;
	listiterator *iterator;
	linkedlist *newlist;
	linkedlist *list;
	
	list = createList();
	list->elemsize = sizeof(draw);
	list->equals = &equalsDraw;
	list->copy = &copyDraw;
	list->free = &freeDraw;
	
	/* Read input file */
	fp = fopen("integration-test-input.txt", "r");
	while (fscanf(fp, "%u %u", &tmp.star1, &tmp.star2) == 2) {
		addValue(list, &tmp);
	}
	
	newlist = copyList(list);
	
	/* Remove draws without stars over 9 */
	iterator = createIterator(newlist);
	while (hasNext(iterator)) {
		next = getNext(iterator);
		if (next->star1 <= 9 && next->star2 <= 9) {
			removeCurrent(iterator);
		}
	}
	freeIterator(iterator);
	
	/* Calculate star averages for list with stars above 9 */
	sum = 0;
	count = 0;
	iterator = createIterator(newlist);
	while (hasNext(iterator)) {
		next = getNext(iterator);
		sum += next->star1 + next->star2;
		count++;
	}
	freeIterator(iterator);
	average9 = (double) sum / (double) (count * 2);
	
	/* Calculate star averages for all elements */
	sum = 0;
	count = 0;
	iterator = createIterator(list);
	while (hasNext(iterator)) {
		next = getNext(iterator);
		sum += next->star1 + next->star2;
		count++;
	}
	freeIterator(iterator);
	average = (double) sum / (double) (count * 2);
	
	printf("Above 9 List (average %f):\n", average9);
	mapList(newlist, &printElement);
	freeList(newlist);
	
	printf("Original List (average %f):\n", average);
	mapList(list, &printElement);
	freeList(list);
	
	return 0;
}
