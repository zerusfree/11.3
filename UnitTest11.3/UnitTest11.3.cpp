#include "pch.h"
#include "CppUnitTest.h"
#include "../Source.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest113
{
	TEST_CLASS(UnitTest113)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
            // Створюємо файл студентів з тестовими даними
            string filename = "students.bin";

            fstream f(filename, ios::binary | ios::out);

            Student s1 = { "a", II, In, 5, 4, 5 };
            Student s2 = { "b", III, Kn, 3, 3, 4 };
            Student s3 = { "c", I, PhI, 4, 4, 5 };

            f.write((char*)&s1, sizeof(Student));
            f.write((char*)&s2, sizeof(Student));
            f.write((char*)&s3, sizeof(Student));
            f.close();

            // Запускаємо пошук та перевіряємо результат
            int result = BinSearch(filename, "b", Kn, 3);
            Assert::AreEqual(1, result);
		}
	};
}
