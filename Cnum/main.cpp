#include "Cnum.h"
#include <iostream>

// Make the priority scheme for Rect overlapping more explicit


/*
	To Add:
		- Find
		- Where
*/


int main() {

	
	iArray test = Cnum::UniformArray({ 5,5,5 }, 10); 
	test[{3, 2, 1}] = 100; 

	test[test > 50].Print();

}