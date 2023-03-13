#include "Cnum.h"
#include <iostream>

// Make the priority scheme for Rect overlapping more explicit


int main() {

	
	iArray test = Cnum::Array<int>({ 1,2,3,4,5,6,7,8 }, { 2,2,2 }); 
	auto thing = test.ReduceAlongAxis(0); 

	test.Print();
	thing.Print();

	test.Reshape({ 4,2 }).Print();


}