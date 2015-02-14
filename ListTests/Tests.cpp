#include "stdafx.h"
#include "CppUnitTest.h"
#include "../FileSystem/List.hpp"
#include "../FileSystem/HashTable.hpp"
#include "../FileSystem/BST.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DataStructuresTests
{		
	TEST_CLASS(ListTests)
	{
	public:
		
		TEST_METHOD(LIST_EmptyListSize) {
			List<int> object;
			size_t size = object.Size();
			Assert::AreEqual(0U, size);
		}

		TEST_METHOD(LIST_OrderCheck) {
			List<size_t> object;
			size_t numbersCount = 10;
			for (size_t i = 0; i < numbersCount; i++) {
				object.PushBack(i);
			}

			for (size_t i = 0; i < numbersCount; i++) {
				Assert::AreEqual(i, object.PopFront());
			}
		}

		TEST_METHOD(LIST_PopOnEmptyList) {
			List<size_t> *object = new List<size_t>;
			auto f1 = [object] { return object->PopBack(); };
			auto f2 = [object] { return object->PopFront(); };
			Assert::ExpectException<const char*>(f1);
			Assert::ExpectException<const char*>(f2);
		}

		TEST_METHOD(LIST_PopElements) {
			List<size_t> object;
			for (size_t i = 0; i < 10; i++) {
				object.PushFront(i);
			}
			Assert::AreEqual(0U, object.PopBack());
			Assert::AreEqual(9U, object.PopFront());
			Assert::AreEqual(4U, object.PopAt(4));
		}

		TEST_METHOD(LIST_IteratorCorrect) {
			List<size_t> object;
			for (size_t i = 0; i < 10; i++) {
				object.PushBack(i);
			}

			size_t c = 0;
			for (List<size_t>::Iterator i = object.Begin(); i != object.End(); ++i) {
				Assert::AreEqual(c, (*i));
				++c;
			}
		}

		TEST_METHOD(LIST_TestPop) {
			List<int> list;
			list.PushBack(1);
			list.PushBack(2);
			Assert::AreEqual(2, list.PopBack());
		}

	};

	TEST_CLASS(HashTableTests) {
private:
	struct TestStruct {
		int size;
		double dSize;
		char a[10];
	};
	static size_t hashString(std::string a) {
		return (size_t)a[0]; // First letter ascii code
	}

	static size_t hashInt(int a) {
		return (size_t)((a * 10) % 25);
	}

	static size_t hashStruct(TestStruct a) {
		return (size_t)(((int)a.dSize + a.size) % 1256);
	}

	public:
		TEST_METHOD(TABLE_EmptyTable) {
			HashTable<int, int> table = HashTable<int, int>(hashInt);
			Assert::IsFalse(table.contains(3));
			bool exThrown = false;
			try {
				table.get(17);
			}
			catch (const char*) {
				exThrown = true;
			}
			Assert::IsTrue(exThrown);
		}

		TEST_METHOD(TABLE_SizeCheck) {
			HashTable<std::string, std::string> table(hashString);
			Assert::AreEqual(0U, table.size());
			table.put("string1", "value1");
			table.put("asd", "dsa");
			table.put("key", "value");
			Assert::AreEqual(3U, table.size());

		}

		TEST_METHOD(TABLE_Get) {
			HashTable<std::string, std::string> table(hashString);
			table.put("string1", "value1");
			table.put("asd", "dsa");
			table.put("key", "value");

			Assert::AreEqual(std::string("dsa"), table.get("asd"));
			Assert::AreEqual(std::string("value"), table.get("key"));

		}
	
		TEST_METHOD(TABLE_Remove) {
			HashTable<std::string, std::string> table(hashString);
			table.put("string1", "value1");
			table.put("asd", "dsa");
			table.put("key", "value");

			Assert::AreEqual(std::string("dsa"), table.remove("asd"));

			bool exThrown = false;
			try {
				table.get("asd");
			}
			catch (const char*) {
				exThrown = true;
			}
			Assert::IsTrue(exThrown);
		}

		TEST_METHOD(TABLE_Copy) {
			HashTable<std::string, std::string> table(hashString);
			table.put("string1", "value1");
			table.put("asd", "dsa");
			table.put("key", "value");

			HashTable<std::string, std::string> newTable(table);
			table.remove("asd");
			table.remove("key");

			Assert::AreEqual(std::string("dsa"), newTable.get("asd"));
		}

	};

	TEST_CLASS(BST_Tests) {
		TEST_METHOD(BST_Insert) {
			BST<std::string, int> tree;
			Assert::AreEqual(0U, tree.size());
			tree.insert("asd", 5);
			Assert::AreEqual(1U, tree.size());

		}

		TEST_METHOD(BST_GET) {
			BST<std::string, int> tree;
			tree.insert("asd", 5);
			int num = tree.get("asd");

			Assert::AreEqual(5, num);
		}

		TEST_METHOD(BST_GETMultiple) {
			BST<std::string, int> tree;
			std::string s;
			for (size_t i = 0; i < 20; i++) {
				s = (char)(i + 65);
				tree.insert(s, i);
			}
			s = (char)(70);
			Assert::AreEqual(5, tree.get(s));
		}

		TEST_METHOD(BST_Remove) {
			BST<std::string, int> tree;
			std::string s;
			for (size_t i = 0; i < 20; i++) {
				s = (char)(i + 65);
				tree.insert(s, i);
			}
			
			s = (char)(70);
			tree.remove(s);
			Assert::AreEqual(19U, tree.size());
			bool exception = false;
			try {
				tree.get(s);
			}
			catch (const char* ex) {
				exception = true;
			}
			Assert::IsTrue(exception);
		}

		TEST_METHOD(BST_Iterator) {
			BST<int, int> tree;
			for (size_t i = 0; i < 20; i++) {

				tree.insert(i, (i + 5)%10);
			}

			for (BST<int, int>::Iterator i = tree.begin(); i != tree.end(); ++i) {
				std::cout << *i;
			}
			// Makes sure no exceptions are thrown, nothing more;
		}
	};
}