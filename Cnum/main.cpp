#include "Cnum.h"
#include <iostream>

// Make the priority scheme for Rect overlapping more explicit


/*
	To Add:
		- Find
		- Where
*/


int main() {

	iArray test = Cnum::Arange(10); 
	iArray test2 = Cnum::Arange(10, 0, 1);

	auto tst2 = Cnum::BlendIf(test, test2, test > 5);

	tst2.Print();
	test.Print(); 
	test2.Print();

 }