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

*/



int main() 
{

	auto test = Cnum::Array<int>({ 1, 2, 4, 7, 0, 1 }, {3,2});

	auto out =  test.AdjacentDiff( false); 

	out.Print();

	
}