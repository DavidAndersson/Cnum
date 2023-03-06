#include "Cnum.h"
#include <iostream>

// Make the priority scheme for Rect overlapping more explicit


int main() {


	dArray arr = Cnum::Array<double>({ 0.55, 0.35, 0.87, 0.85, 0.13, 0.54, 0.67, 0.41 }, { 2,2,2 });
	dArray arrT = arr.Transpose({2, 1, 0});
	dArray answer = Cnum::Array<double>({ 0.55, 0.13, 0.87, 0.67, 0.35, 0.54, 0.85, 0.41 }, { 2,2,2 });
	bool isSame = arrT == answer;

	arrT.Print();


}