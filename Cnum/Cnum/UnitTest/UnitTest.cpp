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
		
		TEST_METHOD(Test_adjacentDiff) {
			iArray arr = Array::initializedArray<int>({ 0,1,3,2,5,9,4,10,17 }, { 3,3 });
			arr.adjacentDiff(1);
			iArray res1 = Array::initializedArray<int>({ 1,2,3,4,6,7 }, { 3,2 }); 
			Assert::IsTrue(res1.isEqualTo(arr)); 

			iArray arr2 = Array::initializedArray<int>({ 0,1,3,2,5,9,4,10,17 }, { 3,3 });
			arr2.adjacentDiff(0);
			iArray res2 = Array::initializedArray<int>({ 2,4,6,2,5,8 }, { 2,3 });
			Assert::IsTrue(res2.isEqualTo(arr2));


			iArray arr3 = Array::initializedArray<int>({ 1,5,3,6,3,6,2,5,3,3,7,2 }, { 2,2,3 });
			arr3.adjacentDiff(0);
			iArray res3 = Array::initializedArray<int>({ 1,0,0,-3,4,-4 }, { 1,2,3 });
			Assert::IsTrue(res2.isEqualTo(arr2));
		}

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

		TEST_METHOD(Test_erase) {
			iArray arr = Array::initializedArray<int>({ 1,2,3,-1,1,4 }, { 2,3 });
			arr.erase_if(arr < 2 || arr > 3);
			Assert::IsTrue(arr.isEqualTo(iArray{ 2,3 })); 


			iArray arr2{ 1,2,3,4,5,5 };
			arr2.erase_if([](auto val) { return val > 1 && val < 3; });
			Assert::IsTrue(arr2.isEqualTo(iArray{ 1,3,4,5,5 }));
		}

		TEST_METHOD(Test_find) {
			iArray arr = Array::initializedArray<int>({ 2,4,2,2,6,2,6,1,36,2563,6,2,36, 13,2,13 }, { 2,2,4 });
			iArray indices = arr.find(arr > 2);
			iArray res = Array::initializedArray<int>({
				0, 0, 1,
				0, 1, 0,
				0, 1, 2,
				1, 0, 0,
				1, 0, 1,
				1, 0, 2,
				1, 1, 0,
				1, 1, 1,
				1, 1, 3
				}, { 9,3 });

			Assert::IsTrue(indices.isEqualTo(res));

			iArray indices2 = arr.find_if([](auto v) {return v == 2; });
			iArray res2 = Array::initializedArray<int>({
				0, 0, 0,
				0, 0, 2,
				0, 0, 3,
				0, 1, 1,
				1, 0, 3,
				1, 1, 2
				}, { 6,3 });
			Assert::IsTrue(indices2.isEqualTo(res2));
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

		TEST_METHOD(Test_matMul) {

			iArray arr = { {1,2,3,4,5,6,7,8,9}, {3,3} };
			iArray arr2 = { {1,2,3,4,5,6,7,8,9,10,11,12}, {3,4} };

			iArray result = matrixMul(arr, arr2);

			iArray correctResult = Array::initializedArray<int>({
				38,44,50,56,
				83,98,113,128,
				128, 152, 176, 200 }, { 3,4 });

			Assert::IsTrue(result.isEqualTo(correctResult));

		}

		TEST_METHOD(Test_norm) {
			fArray arr{ 1,1 }; 
			float norm = arr.norm(); 
			Assert::IsTrue(norm - std::pow(2, 0.5) < std::pow(10, -6));

			fArray arr2 = Array::initializedArray<float>({ 1,2,3,-1,1,4 }, { 2,3 });
			arr2.norm(0);
			Assert::IsTrue(arr2.isEqualTo(fArray{1.41421, 2.23606, 5}));
		}

		TEST_METHOD(Test_raiseTo) 
		{
			iArray arr{ 1,2,3,4 };
			arr.raiseTo(2);
			iArray result{ 1, 4, 9, 16 };
			Assert::IsTrue(arr.isEqualTo(result));

			dArray arr2{ 1.2, 3.1, 6.5 };
			arr2.raiseTo(3.3);
			dArray result2{ 1.83, 41.83, 481.52 };
			Assert::IsTrue(arr2.isEqualTo(result2, 2));

			dArray arr3{ 1.2, 3.1, 6.5 };
			arr3.raiseTo(-3.3);
			dArray result3{ 0.547901, 0.023906, 0.00207676 };
			Assert::IsTrue(arr3.isEqualTo(result3, 4));
		}

		TEST_METHOD(Test_reduceAlong) {
			iArray arr = Array::initializedArray<int>({ 3,5,3,6,7,5,3,4,7,7,24,5,3,5 }, { 7,2 });
			arr.reduceAlongAxis(1, 0, std::plus<>());
			iArray res = Array::initializedArray<int>({ 8,9,12,7,14,29,8 }, { 7,1 });
			Assert::IsTrue(res.isEqualTo(arr));

			iArray arr2 = Array::initializedArray<int>({ 3,5,3,6,7,5,3,4,7,7,24,5,3,5,2,3 }, { 2,2,4 });
			arr2.reduceAlongAxis(2, 0, std::plus<>());
			iArray res2 = Array::initializedArray<int>({ 17,19,43,13}, { 2,2,1 });
			Assert::IsTrue(res2.isEqualTo(arr2));

			iArray arr3 = Array::initializedArray<int>({ 3,5,3,6,7,5,3,4,7,7,24,5,3,5,2,3 }, { 2,2,4 });
			arr3.reduceAlongAxis(0, 0, std::plus<>());
			iArray res3 = Array::initializedArray<int>({ 10,12,27,11,10,10,5,7 }, { 1,2,4 });
			Assert::IsTrue(res3.isEqualTo(arr3));
		}

		TEST_METHOD(Test_reverse) {
			iArray arr{ 1,2,3,4 }; 
			arr.reverse();
			Assert::IsTrue(arr.isEqualTo({ 4,3,2,1 })); 

			iArray arr2 = Array::initializedArray<int>({ 1,2,3,4,5,6,7,8,9 }, { 3,3 }); 
			arr2.reverse(1); 
			iArray res = Array::initializedArray<int>({ 3,2,1,6,5,4,9,8,7 }, { 3,3 });
			Assert::IsTrue(arr2.isEqualTo(res));

		}

		TEST_METHOD(Test_sort)
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
		
		
	};
}
