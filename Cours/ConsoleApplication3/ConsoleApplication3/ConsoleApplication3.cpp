#include "pch.h"
#include <iostream>
#include "IntArray.hpp"
#include <time.h>
#include "Util.hpp"
#include "List.hpp"
#include "ConsoleApplication3.h"

void StrCat(char * dst, const char * src) {
	if (*dst == 0) {
		if (*src == 0)
			return;
		else {
			*dst = *src;
			*(dst + 1) = 0;
			StrCat(dst + 1, src + 1);
		}
	}
	else
		StrCat(dst + 1, src);
}

int StrCmp(const char * s0, const char * s1) {
	if (*s0 == 0 && *s1 == 0) return 0;
	else if (*s0 < *s1) return 1;
	else if (*s0 > *s1) return -1;
	return StrCmp(s0 + 1, s1 + 1);
}

int _StrChr(const char * s0, char s1, const char * _start) {
	if (*s0 == 0 && s1 != 0)
		return -1;
	else if (*s0 == s1)
		return s0 - _start;
	else
		return _StrChr(s0 + 1, s1, _start);
}

int StrChr(const char * s0, char s1) {
	return _StrChr(s0, s1, s0);
}

int StrChr2(const char * s0, char s1, const char*_orig = nullptr) {
	if (_orig == nullptr)
		return StrChr2(s0, s1, s0);

	if (*s0 == 0 && s1 != 0)
		return -1;
	else if (*s0 == s1)
		return s0 - _orig;
	else
		return StrChr2(s0 + 1, s1, _orig);
}



bool contains(const char * s0, const char * s1) {
	int len = strlen(s0);
	return s1 >= s0 && s1 <= s0 + len;
}

bool startsWith(const char * s0, const char * s1) {
	if (*s0 == 0 && *s1 != 0)
		return false;
	if (*s1 == 0)
		return true;
	if (*s0 != *s1)
		return false;
	else
		return startsWith(s0 + 1, s1 + 1);
}

const char * StrStr(const char * s0, const char * s1) {
	if (startsWith(s0, s1))
		return s0;
	else
		if (*s0 == 0) {
			if (*s1 == 0)
				return s0;
			else
				return nullptr;
		}
		else
			return StrStr(s0 + 1, s1);
}

void assert(bool b) {
	if (!b) throw std::exception();
}

void testInList()
{
	IntList l(8);

	int toto = 0;
}

int main()
{
	const int len = 4;
	int sizes[len] = { 4, 8, 16, 32 };
	double prev = 0.000000000001;
	for (int k = 0; k < len; k++)
	{
		double t0 = TimeUtils::getTimestamp();
		IntArray toto(16);
		toto.fillWithRandom(256 * sizes[k]);
		toto.insertSort();
		for (int i = 0; i < toto.getLength() - 1; i++)
			assert(toto[i] <= toto[i + 1]);
		double t1 = TimeUtils::getTimestamp();
		printf("temps de calcul: %f s for %d\n", (t1 - t0), toto.getLength());
		int j = 0;
		auto cur = t1 - t0;
		printf("rapport:%f\n", cur / prev);
		prev = t1 - t0;
	}


	{
		IntArray test;
		test[0] = 66;
		test[1] = 69;
		test.swap(0, 1);
		assert(test[0] == 69);
	}

	int k = 0;

	{
		IntArray test(1000);
		for (int i = 0; i < 255; i++) {
			test[i] = i * 10;
		}
		assert(test.binarySearch(10) >= 0);
		assert(test.binarySearch(10) < test.getLength());
	}

	{
		IntArray test(1000);
		assert(test.binarySearch(10) == 0);
	}
	{
		IntArray test(10);
		int pos = 0;
		test[pos++] = 5;
		test[pos++] = 10;
		test[pos++] = 15;
		test[pos++] = 50;

		assert(test.binarySearch(4) == 0);
		assert(test.binarySearch(5) == 0);
		assert(test.binarySearch(6) == 1);
		assert(test.binarySearch(50) == 3);
		assert(test.binarySearch(60) == 4);
	}

	{
		IntArray test(10);
		int pos = 0;
		test[pos++] = 1;
		test[pos++] = 3;
		test[pos++] = 8;
		test[pos++] = 10;
		test[pos++] = 15;
		test[pos++] = 17;
		test[pos++] = 20;

		for (int i = 0; i < 10; ++i) {
			printf("i:%d  b:%d lin:%d \n", i, test.binarySearch(i), test.searchPosition(i));
			assert(test.binarySearch(i) == test.searchPosition(i));
		}
		{
			int bpos = test.binarySearch(2);
			int here = 0;
		}
		{
			printf("----------------------\n");
			int bpos = test.binarySearch(12);
			int here = 0;
		}
	}

	int l = 0;
}
