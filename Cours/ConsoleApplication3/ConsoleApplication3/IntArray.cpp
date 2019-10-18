#include "pch.h"
#include "IntArray.hpp"

int IntArray::TEST = 66;

IntArray::IntArray(int size, const char * name) {
	this->name = name;
	printf("construite! %s\n", this->name.c_str());

	if (size == 0) size++;
	data = new int[size];
	for (int i = 0; i < size; ++i) data[i] = 0;

	maxSize = size;
}

bool IntArray::ensure(int size) {
	if (size < maxSize) return false;

	int* newdata = new int[size];

	for (int i = 0; i < size; ++i) newdata[i] = 0;
	for (int i = 0; i < curSize; i++)
		newdata[i] = data[i];
	delete(data);
	data = newdata;

	maxSize = size;
	return true;
}

int IntArray::getLength() {
	return curSize;
}


void IntArray::push_back(int elem) {
	ensure(curSize + 1);
	data[curSize] = elem;
	curSize++;
}

void IntArray::push_front(int elem) {
	ensure(curSize + 1);
	for (int i = curSize; i > 0; i--)
		data[i] = data[i - 1];
	data[0] = elem;
	curSize++;
}

void IntArray::insert(int pos, int elem) {
	
	ensure((pos >= curSize) ? (pos + 1) : (curSize + 1));

	for (int i = curSize; i > pos; i--)
		data[i] = data[i - 1];

	data[pos] = elem;

	if (pos >= curSize)
		curSize = pos + 1;
	else
		curSize++;
}