#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab9.2/lab9.2.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
            const int N = 4;
            Student students[N] = {
                {"Dio", 1, CS, 80, 85, {90}},
                {"Jojo", 2, IT, 75, 80, {85}},
                {"Jotaro", 3, ME, 90, 95, {100}},
                {"Kakyoin", 2, CS, 85, 90, {87}}
            };

            SortForBinarySearch(students, N);

            Assert::IsTrue(BinarySearch(students, N, "Dio", 1, 90));
            Assert::IsFalse(BinarySearch(students, N, "Giorno", 1, 88));
            Assert::IsFalse(BinarySearch(students, N, "Dio", 1, 88));
            Assert::IsFalse(BinarySearch(students, N, "Dio", 2, 90));
		}
	};
}