#include <iostream>
#include <stdlib.h>

#include "../include/lib_func.h"

int main()
{
	const int NUM = 100;
	item items[NUM];

	for (int i = 0; i < NUM; i++) {
		items[i].key = rand();
	}

	radix_sort(items, items + NUM, 256);

	for (int i = 0; i < NUM; i++) {
		printf_s("%d\n", items[i].key);
	}
}
