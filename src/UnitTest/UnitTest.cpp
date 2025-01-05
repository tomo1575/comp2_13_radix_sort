#include "pch.h"
#include <chrono>
#include <random>
#include "CppUnitTest.h"
#include "../include/lib_func.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTest
{
	TEST_CLASS(基本確認)
	{
	public:
		_CrtMemState sStartMemState;
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}
		TEST_METHOD_CLEANUP(Cleanup)
		{
#ifdef _DEBUG
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

	public:

		TEST_METHOD(一要素はそのまま)
		{
			item items[1] = { {1, "Hello world!"} };
			Assert::IsTrue(radix_sort(items, items + sizeof(items) / sizeof(item), 10));
			Assert::AreEqual("Hello world!", items[0].value);

		}

		TEST_METHOD(２要素でそのまま)
		{
			item items[2] = {
				{2, "Hello"},
				{4, "world!"},
			};
			Assert::IsTrue(radix_sort(items, items + sizeof(items) / sizeof(item), 10));
			Assert::AreEqual("Hello", items[0].value);
			Assert::AreEqual("world!", items[1].value);
		}

		TEST_METHOD(２要素で入れ替え)
		{
			item items[2] = {
				{9, "world!"},
				{7, "Hello"},
			};
			Assert::IsTrue(radix_sort(items, items + sizeof(items) / sizeof(item), 10));
			Assert::AreEqual("Hello", items[0].value);
			Assert::AreEqual("world!", items[1].value);
		}
	};

	TEST_CLASS(品質確認)
	{
	public:
		_CrtMemState sStartMemState;
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}
		TEST_METHOD_CLEANUP(Cleanup)
		{
#ifdef _DEBUG
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		static void swap(item* a, item* b)
		{
			item t = *a;
			*a = *b;
			*b = t;
		}

		static item* search_front(unsigned int key, item* begin, const item* end)
		{
			while (begin->key <= key && begin < end) begin++;
			return begin;
		}

		static item* search_rear(unsigned int key, const item* begin, item* end)
		{
			while (key <= end->key && begin < end) end--;
			return end;
		}

		item* partition(item* begin, const item* end)
		{
			int key = begin->key;
			item* head = search_front(key, begin + 1, end);
			item* tail = search_rear(key, begin, begin + (end - begin) - 1);

			while (head < tail) {
				swap(head, tail);
				head = search_front(key, head + 1, end);
				tail = search_rear(key, begin, tail - 1);
			}

			swap(tail, begin);
			return tail;
		}

		bool quick_sort(item* begin, const item* end)
		{
			if (begin == NULL || end == NULL || end < begin) return false;
			if (end - begin <= 1) return true;

			item* pivot = partition(begin, end);
			quick_sort(begin, pivot);
			quick_sort(pivot + 1, end);

			return true;
		}

	public:

		TEST_METHOD(NULLは帰り値がfalse)
		{
			item items[1]={{0,""}};
			Assert::IsFalse(radix_sort(NULL, items, 10));
			Assert::IsFalse(radix_sort(items, NULL, 10));
		}

		TEST_METHOD(endのアドレスが小さいのはダメ)
		{
			item items[1]={{0,""}};
			Assert::IsFalse(radix_sort(items + sizeof(items) / sizeof(item), items, 10));
		}

		TEST_METHOD(小さな基数はダメ)
		{
			item items[1]={{0,""}};
			Assert::IsFalse(radix_sort(items, items+1, 0));
			Assert::IsFalse(radix_sort(items, items+1, -1));
		}

		TEST_METHOD(たくさんの要素を入れ替え)
		{
			const int NUM = 10000;
			item* items = (item*)malloc(sizeof(item) * NUM);
			if (NULL == items) { Assert::Fail(); return; }

			for (int i = 0; i < NUM; i++) {
				items[i].key = NUM - i;
				sprintf_s(items[i].value, 256, "%d", i);
			}

			Assert::IsTrue(radix_sort(items, items + NUM, 10));

			for (int i = 0; i < NUM; i++) {
				char buf[256];
				sprintf_s(buf, 256, "%d", NUM - i - 1);
				Assert::AreEqual(buf, items[i].value);
			}

			free(items);
		}

		TEST_METHOD(クイックソートとの比較)
		{
			const int NUM = 100000;

			std::random_device rnd;
			item* items = (item*)malloc(sizeof(item) * NUM);
			Assert::IsNotNull(items);
			for (int i = 0; i < NUM; i++) {
				items[i].key = i;
				sprintf_s(items[i].value, 256, "%d", i);
			}
			for (int i = 0; i < NUM; i++) {
				int r = rnd() % NUM;
				unsigned int k = items[i].key;
				items[i].key = items[r].key;
				items[r].key = k;
			}

			item* item_b = (item*)malloc(sizeof(item) * NUM);
			Assert::IsNotNull(item_b);
			memcpy_s(item_b, sizeof(item) * NUM, items, sizeof(item) * NUM);

			std::chrono::system_clock::time_point timeStt, timeEnd;
			timeStt = std::chrono::system_clock::now();
			quick_sort(item_b, item_b + NUM);
			timeEnd = std::chrono::system_clock::now();
			uint64_t  sortTime_quick = std::chrono::duration_cast<std::chrono::microseconds>(timeEnd - timeStt).count();

			timeStt = std::chrono::system_clock::now();
			radix_sort(items, items + NUM, 1000);
			timeEnd = std::chrono::system_clock::now();
			uint64_t  sortTime_radix = std::chrono::duration_cast<std::chrono::microseconds>(timeEnd - timeStt).count();

			for (int i = 0; i < NUM; i++) {
				Assert::AreEqual(item_b[i].value, items[i].value);
			}

			Assert::IsTrue(sortTime_radix < sortTime_quick);// クイックソートよりも早くできるかな

			free(item_b);
			free(items);
		}
	};
}
