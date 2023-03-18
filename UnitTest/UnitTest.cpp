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
		
		TEST_METHOD(Test_Concatenation)
		{
			iArray arr = Cnum::UniformArray<int>({ 3,3 }, 0);
			iArray in = Cnum::Array<int>({ 1,2,3 });
			{
				// axis 0
				arr.Concatenate(in, 0);
				auto result = Cnum::Array<int>({ 0,0,0,0,0,0,0,0,0,1,2,3 }, { 4,3 });
				Assert::IsTrue(arr.isEqualTo(result));
			}
			{
				// axis 1
				iArray arr = Cnum::UniformArray<int>({ 3,3 }, 0);
				arr.Concatenate(in.Transpose(), 1);
				auto result = Cnum::Array<int>({ 0,0,0,1,0,0,0,2,0,0,0,3 }, { 3,4 });
				Assert::IsTrue(arr.isEqualTo(result));

			}
			{
				arr = Cnum::UniformArray<int>({ 2,2,2 }, 0);
				in = Cnum::Array<int>({ 1,2,3,4,5,6,7,8,9,10,11,12 }, { 2,2,3 });

				// axis 2
				arr.Concatenate(in, 2);
				auto result = Cnum::Array<int>({ 0,0,1,2,3,0,0,4,5,6,0,0,7,8,9,0,0,10,11,12 }, { 2,2,5 });
				Assert::IsTrue(arr.isEqualTo(result));
			}
		}

		TEST_METHOD(Test_Insert)
		{
			
			{
				// axis 0
				iArray arr = Cnum::UniformArray<int>({ 2,2 }, 0);
				iArray in = Cnum::Array<int>({ 1,2,3,4,5,6 }, { 3,2 });
				arr.Insert(in, 0, 1);
				auto result = Cnum::Array<int>({ 0,0,1,2,3,4,5,6,0,0,0,0 }, { 5,2 });
				Assert::IsTrue(arr.isEqualTo(result));
			}
			{
				// axis 1
				iArray arr = Cnum::UniformArray<int>({ 2,2 }, 0);
				iArray in = Cnum::Array<int>({ 1,2,3,4,5,6 }, { 2,3 });
				arr.Insert(in, 1, 0);
				auto result = Cnum::Array<int>({ 1,2,3,0,0,4,5,6,0,0 }, { 2,5 });
				Assert::IsTrue(arr.isEqualTo(result));

			}
			{
				// axis 2
				iArray arr = Cnum::UniformArray<int>({ 2,2,2 }, 0);
				iArray in = Cnum::Array<int>({ 1,2,3,4,5,6,7,8,9,10,11,12 }, { 2,2,3 });
				arr.Insert(in, 2, 1);
				auto result = Cnum::Array<int>({ 0,1,2,3,0,0,4,5,6,0,0,7,8,9,0,0,10,11,12,0 }, { 2,2,5 });
				Assert::IsTrue(arr.isEqualTo(result));
			}
		}
		TEST_METHOD(Test_Transpose)
		{			
			{
				// 3D
				iArray arr = Cnum::Array<int>({ 1,2,3,4,5,6,7,8,9,10,11,12 }, { 2,2,3 });
				arr.Transpose({ 0,2,1 });
				iArray answer = Cnum::Array<int>({ 1,4,2,5,3,6,7,10,8,11,9,12 }, { 2,3,2 });
				Assert::IsTrue(arr.isEqualTo(answer));
			}
			{
				// 2D
				dArray arr = Cnum::Array<double>({ 0.64, 0.17, 0.74, 0.6, 0.76, 0.87, 0.02, 0.29, 0.69, 0.44, 0.31, 0.34, 0.08, 0.04, 0.89, 0.49, 0.28, 0.45, 0.23, 0.60 }, { 4,5 });
				arr.Transpose();
				dArray answer = Cnum::Array<double>({ 0.64, 0.87, 0.31, 0.49,0.17, 0.02, 0.34, 0.28, 0.74, 0.29, 0.08, 0.45, 0.60, 0.69, 0.04, 0.23, 0.76, 0.44, 0.89, 0.60 }, { 5,4 });
				Assert::IsTrue(arr.isEqualTo(answer));
			}
			
		}

		
	};
}
