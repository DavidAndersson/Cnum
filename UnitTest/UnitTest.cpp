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
			{
				dArray arr = Cnum::Array<double>({ 0.47, 0.40, 0.08, 0.12, 0.21, 0.72, 0.62, 0.85 }, { 2,2,2 });
				arr.Transpose();
				dArray answer = Cnum::Array<double>({ 0.47, 0.21, 0.08, 0.62, 0.4, 0.72, 0.12, 0.85 }, { 2,2,2 });
				Assert::IsTrue(arr == answer);
			}
			
			{
				iArray arr = Cnum::Array<int>({ 1,2,3,4,5,6,7,8,9,10,11,12 }, { 2,2,3 });
				arr.Transpose({ 0,2,1 });
				iArray answer = Cnum::Array<int>({ 1,4,2,5,3,6,7,10,8,11,9,12 }, { 2,3,2 });
				Assert::IsTrue(arr == answer);
			}

			{
				dArray arr = Cnum::Array<double>({ 0.64, 0.17, 0.74, 0.6, 0.76, 0.87, 0.02, 0.29, 0.69, 0.44, 0.31, 0.34, 0.08, 0.04, 0.89, 0.49, 0.28, 0.45, 0.23, 0.60 }, { 4,5 });
				arr.Transpose();
				dArray answer = Cnum::Array<double>({ 0.64, 0.87, 0.31, 0.49,0.17, 0.02, 0.34, 0.28, 0.74, 0.29, 0.08, 0.45, 0.60, 0.69, 0.04, 0.23, 0.76, 0.44, 0.89, 0.60 }, { 5,4 });
				Assert::IsTrue(arr == answer);
			}
			




		}
	};
}
