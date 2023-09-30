#include "Cnum.h"
#include <iostream>

/*
	TODOs: 
		Unit tests
		Print in which function an exception is thrown
		Mathemathical functions mapped to all elements
		Random
		Shape - which Rect, Circ etc could inherit from
		Fix kdTree
		Single int indexing in [] seems to be broken, at least for bool. True becomes false as the function returns to the parent stack frame
		Don't return vectors from functions if not necessary

		Add extractIndicesAlongAxis(axis, nonAxisIndices, condition) alt. extractIndicesAlongAxis(axis, nonAxisIndices, indices)

		Custom Exception class which uses std::location described in "Professional C++"

		Add concepts and make many functions follow the input arrayLike

		Use ranges for algorithms such as sort etc


		PRIO. 
		1. Replace exceptions with assertions

*/


int main() 
{

	iArray test = { {1,2,3,4,5,6,7,8,9}, {3,3} };
	iArray test2 = { {1,2,3,4,5,6,7,8,9}, {3,3} };

	iArray test3(0, 10); 

	test3.replaceAlong({ 10,9,8,7,6,5,4,3,2,1 }, 0, { 0 }); 

	test.reshape({3,3});

	test[test > 5].print();

	test[test == 2].print();

	auto res = test / test2; 


}