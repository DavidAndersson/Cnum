#include "Cnum.h"
#include <iostream>

// Make the priority scheme for Rect overlapping more explicit


/*
	To Add:
		- Find
		- Where
*/


int main() {

	fArray test = Cnum::UniformArray<float>({ 3,2,1 }, 5); 

	auto tst2 = Cnum::FindWhere( test > 2.0f);

	tst2.Print();
 }