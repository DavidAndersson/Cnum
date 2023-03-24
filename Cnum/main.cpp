#include "Cnum.h"
#include <iostream>


/*
	TODOs: 
		Unit tests
		Print in which function an exception is thrown
		Reverse, roll, delta methods
		Mathemathical functions mapped to all elements
		Random
		Shape - which Rect, Circ etc could inherit from
		Fix kdTree
		Single int indexing in [] seems to be broken, at least for bool. True becomes false as the function returns to the parent stack frame

		Add extractIndicesAlongAxis(axis, nonAxisIndices, condition) alt. extractIndicesAlongAxis(axis, nonAxisIndices, indices)

*/



int main() 
{

	iArray test = Cnum::Array<int>({ 12,11,10,9,8,7,6,5,4,3,2,1 }, { 2,2,3 });

	test.Print();

	test.Sort(2).Print();




	
}