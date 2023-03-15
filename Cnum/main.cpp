#include "Cnum.h"
#include <iostream>

// Make the priority scheme for Rect overlapping more explicit


/*
	To Add:
		- Find
		- Where
*/


int main() {

	
	iArray test = Cnum::Arange(0, 50, 1); 

	auto test2 = test[test > 10 && test < 40]; 

	test2.Print();

 }