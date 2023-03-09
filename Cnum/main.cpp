#include "Cnum.h"
#include <iostream>

// Make the priority scheme for Rect overlapping more explicit


int main() {

	{
		dArray arr = Cnum::Array<double>({ 0.64, 0.17, 0.74, 0.6, 0.76, 0.87, 0.02, 0.29, 0.69, 0.44, 0.31, 0.34, 0.08, 0.04, 0.89, 0.49, 0.28, 0.45, 0.23, 0.60 }, { 4,5 });
		auto arrT = Cnum::Transpose(arr, DynamicArray<int>({1,0}));
		dArray answer = Cnum::Array<double>({ 0.64, 0.87, 0.31, 0.49,0.17, 0.02, 0.34, 0.28, 0.74, 0.29, 0.08, 0.45, 0.60, 0.69, 0.04, 0.23, 0.76, 0.44, 0.89, 0.60 }, { 5,4 });
		if (arrT.isEqualTo(answer))
			arrT.Print();

		auto test = arrT == answer;
		DynamicArray<double> tt = Cnum::Concatenate(arrT[arrT == answer], arrT[test]);
		tt.Print();

	}

}