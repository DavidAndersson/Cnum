#include "Cnum.h"
#include <iostream>

// Make the priority scheme for Rect overlapping more explicit


/*
	To Add:
		- Find
		- Where
*/


int main() {

	
	iArray test = Cnum::UniformArray<int>({ 5,5 }, 0); 
	iArray test2 = Cnum::UniformArray<int>({ 5,5 }, 1);

	auto test3 = Cnum::MinimumOf(test, test2); 

	test3.Print();

}