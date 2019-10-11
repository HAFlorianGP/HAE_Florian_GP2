#pragma once

#include <cstdio>
#include <cstdlib>
#include <string>

class IntArray {

public:
	std::string name;
	int * data; 
	int curSize = 0;
	int maxSize = 0;

	static int TEST;

	IntArray(int size = 1, const char * name = "");

	~IntArray() {
		printf("detruite! %s\n", name.c_str());
	}

	bool ensure(int size);

	int getLength();

	int get(int pos) {
		return data[pos];
	}

	void set(int pos, int elem) {
		ensure(pos + 1);
		data[pos] = elem;
		if (pos >= curSize)
			curSize = pos + 1;
	}

	void set_unsafe(int pos, int elem) {
		data[pos] = elem;
		curSize = pos + 1;
	}

	int & operator() (int pos) {
		ensure(pos + 1);
		if (pos >= curSize) curSize = pos + 1;
		return data[pos];
	}

	int & operator[] (int pos) {
		ensure(pos + 1);
		if (pos >= curSize) curSize = pos + 1;
		return data[pos];
	}

	void push_back(int elem);
	void push_front(int elem);
	void insert(int pos, int elem);

	int searchPosition(int element) {
		for (int i = 0; i < getLength(); i++)
			if (element <= data[i])
				return i;
		return getLength();
	}

	void removeAll() {
		curSize = 0;
	}
	bool remove(int valeur) {
		int idx = -1;
		for (int i = 0; i < getLength(); i++) {
			if (data[i] == valeur) {
				idx = i;
				break;
			}
		}
		if (idx == -1) return false;
		for (int i = idx; i < getLength() - 1; i++)
			data[i] = data[i + 1];
		data[curSize - 1] = 0;
		curSize--;
	}

	bool remove2(int valeur) {
		int idx = -1;
		for (int i = 0; i < getLength(); i++) {
			if (data[i] == valeur) {
				for (int j = idx; j < getLength() - 1; j++)
					data[j] = data[j + 1];
				data[curSize - 1] = 0;
				curSize--;
				return true;
			}
		}
		return false;
	}

	void fillWithRandom(int nbElem) {
		ensure(nbElem);
		for (int i = 0; i < nbElem; ++i) {
			int val = std::rand();
			set(i, val);
		}
	}

	void sort() {
		IntArray nuData(1, "nudata");
		for (int i = 0; i < getLength(); i++) {
			int val = get(i);
			printf("val: %d\n", val);

			int pos = nuData.searchPosition(val);
			printf("future pos: %d\n", pos);
			nuData.insert(nuData.searchPosition(val), val);

			printf("[");
			for (int j = 0; j < nuData.getLength(); j++)
				printf("%d ", nuData[j]);
			printf("]\n");

		}

		for (int i = 0; i < getLength(); i++) {
			set(i, nuData.get(i));
		}
	}

};