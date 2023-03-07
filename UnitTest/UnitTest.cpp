#include "pch.h"
#include "CppUnitTest.h"
#include "../Cnum/DynamicArray.h"
#include "../Cnum/Cnum.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(Test_Transpose)
		{
			dArray arr = Cnum::Array<double>({ 0.47, 0.40, 0.08, 0.12, 0.21, 0.72, 0.62, 0.85 }, { 2,2,2 });
			arr.Transpose();
			dArray answer = Cnum::Array<double>({ 0.47, 0.21, 0.08, 0.62, 0.4, 0.72, 0.12, 0.85}, { 2,2,2 });
			bool isSame = arr == answer;
			Assert::IsTrue(isSame);

		}
	};
}
