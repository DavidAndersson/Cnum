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

		Add custom Exception class and use std::location (?) to print line number etc for exceptions

		Add concepts to members so that its possible to use one overload for vectors, dArrays or others. 
			- call the concept arrayLike ?

*/



int main() 
{
	auto arr = Cnum::Array<double>({
		0.22, 0.08, 0.35,
		0.64, 0.79, 0.24,
		0.13, 0.07, 0.06 }, { 3,3 });

	arr.Transpose({ 0,1 });
	arr.Print();


	
}