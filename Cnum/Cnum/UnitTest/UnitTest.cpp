#include "pch.h"
#include "CppUnitTest.h"
#include "../Cnum.h"
#include "../ndArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTest
{
	using namespace Cnum;

	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(Test_concatenation)
		{
			iArray arr = Array::uniformArray({ 3,3 }, 0);
			iArray in{ 1,2,3 };
			{
				// axis 0
				arr.concatenate(in, 0);
				iArray result = Array::initializedArray<int>({ 0,0,0,0,0,0,0,0,0,1,2,3 }, { 4,3 });
				Assert::IsTrue(arr.isEqualTo(result));
			}
			{
				// axis 1
				iArray arr = Array::uniformArray({ 3,3 }, 0);
				arr.concatenate(in.transpose(), 1);
				auto result = Array::initializedArray<int>({ 0,0,0,1,0,0,0,2,0,0,0,3 }, { 3,4 });
				Assert::IsTrue(arr.isEqualTo(result));

			}
			{
				iArray arr = Array::uniformArray<int>({ 2,2,2 }, 0);
				iArray in = Array::initializedArray<int>({ 1,2,3,4,5,6,7,8,9,10,11,12 }, { 2,2,3 });

				// axis 2
				arr.concatenate(in, 2);
				auto result = Array::initializedArray<int>({ 0,0,1,2,3,0,0,4,5,6,0,0,7,8,9,0,0,10,11,12 }, { 2,2,5 });
				Assert::IsTrue(arr.isEqualTo(result));
			}
		}
		TEST_METHOD(Test_insert)
		{

			{
				// axis 0
				iArray arr = Array::uniformArray<int>({ 2,2 }, 0);
				iArray in = Array::initializedArray<int>({ 1,2,3,4,5,6 }, { 3,2 });
				arr.insert(in, 0, 1);
				auto result = Array::initializedArray<int>({ 0,0,1,2,3,4,5,6,0,0 }, { 5,2 });
				Assert::IsTrue(arr.isEqualTo(result));
			}
			{
				// axis 1
				iArray arr = Array::uniformArray<int>({ 2,2 }, 0);
				iArray in = Array::initializedArray<int>({ 1,2,3,4,5,6 }, { 2,3 });
				arr.insert(in, 1, 0);
				auto result = Array::initializedArray<int>({ 1,2,3,0,0,4,5,6,0,0 }, { 2,5 });
				Assert::IsTrue(arr.isEqualTo(result));

			}
			{
				// axis 2
				iArray arr = Array::uniformArray<int>({ 2,2,2 }, 0);
				iArray in = Array::initializedArray<int>({ 1,2,3,4,5,6,7,8,9,10,11,12 }, { 2,2,3 });
				arr.insert(in, 2, 1);
				auto result = Array::initializedArray<int>({ 0,1,2,3,0,0,4,5,6,0,0,7,8,9,0,0,10,11,12,0 }, { 2,2,5 });
				Assert::IsTrue(arr.isEqualTo(result));
			}
		}
		TEST_METHOD(Test_transpose)
		{
			{
				// 3D
				iArray arr = Array::initializedArray<int>({ 1,2,3,4,5,6,7,8,9,10,11,12 }, { 2,2,3 });
				arr.transpose({ 0,2,1 });
				iArray answer = Array::initializedArray<int>({ 1,4,2,5,3,6,7,10,8,11,9,12 }, { 2,3,2 });
				Assert::IsTrue(arr.isEqualTo(answer));
			}
			{
				// 2D
				dArray arr = Array::initializedArray<double>({ 0.64, 0.17, 0.74, 0.6, 0.76, 0.87, 0.02, 0.29, 0.69, 0.44, 0.31, 0.34, 0.08, 0.04, 0.89, 0.49, 0.28, 0.45, 0.23, 0.60 }, { 4,5 });
				arr.transpose();
				dArray answer = Array::initializedArray<double>({ 0.64, 0.87, 0.31, 0.49,0.17, 0.02, 0.34, 0.28, 0.74, 0.29, 0.08, 0.45, 0.60, 0.69, 0.04, 0.23, 0.76, 0.44, 0.89, 0.60 }, { 5,4 });
				Assert::IsTrue(arr.isEqualTo(answer));
			}

		}
		TEST_METHOD(Test_Sort)
		{

			dArray arr = Array::initializedArray<double>({
				0.22, 0.08, 0.83,
				0.84, 0.79, 0.51,
				0.75, 0.07, 0.06,
				0.74, 0.27, 0.35,
				0.67, 0.83, 0.74,
				0.13, 0.81, 0.76,
				0.43, 0.88, 0.93,
				0.64, 0.93, 0.24,
				0.61, 0.76, 0.99 },
				{ 3,3,3 });


			// Axis 0
			{
				auto arr_0 = arr;
				auto sorted = arr_0.sort(0);
				auto result = Array::initializedArray<double>({
				0.22, 0.08, 0.35,
				0.64, 0.79, 0.24,
				0.13, 0.07, 0.06,

				0.43, 0.27, 0.83,
				0.67, 0.83, 0.51,
				0.61, 0.76, 0.76,

				0.74, 0.88, 0.93,
				0.84, 0.93, 0.74,
				0.75, 0.81, 0.99, }, { 3,3,3 });

				Assert::IsTrue(sorted.isEqualTo(result));
			}

			// Axis 1
			{
				auto arr_1 = arr;
				auto sorted = arr_1.sort(1);
				auto result = Array::initializedArray<double>({
					0.22, 0.07, 0.06,
					0.75, 0.08, 0.51,
					0.84, 0.79, 0.83,

					0.13, 0.27, 0.35,
					0.67, 0.81, 0.74,
					0.74, 0.83, 0.76,

					0.43, 0.76, 0.24,
					0.61, 0.88, 0.93,
					0.64, 0.93, 0.99 }, { 3, 3, 3 });

				Assert::IsTrue(sorted.isEqualTo(result));
			}

			// Axis 2
			{
				auto arr_2 = arr;
				auto sorted = arr_2.sort(2);
				auto result = Array::initializedArray<double>({
					0.08, 0.22, 0.83,
					0.51, 0.79, 0.84,
					0.06, 0.07, 0.75,

					0.27, 0.35, 0.74,
					0.67, 0.74, 0.83,
					0.13, 0.76, 0.81,

					0.43, 0.88, 0.93,
					0.24, 0.64, 0.93,
					0.61, 0.76, 0.99 }, { 3, 3, 3 });

				Assert::IsTrue(sorted.isEqualTo(result));
			}


		}
		TEST_METHOD(Test_MatMul) {

			iArray arr = { {1,2,3,4,5,6,7,8,9}, {3,3} };
			iArray arr2 = { {1,2,3,4,5,6,7,8,9,10,11,12}, {3,4} };

			iArray result = matrixMul(arr, arr2);

			iArray correctResult = Array::initializedArray<int>({
				38,44,50,56,
				83,98,113,128,
				128, 152, 176, 200 }, { 3,4 });

			Assert::IsTrue(result.isEqualTo(correctResult));

		}
	};
}
