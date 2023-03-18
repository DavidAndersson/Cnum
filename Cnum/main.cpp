#include "Cnum.h"
#include <iostream>


/*
	TODOs: 
		Unit tests
		Print in which function an exception is thrown
		Reverse method
		Mathemathical functions mapped to all elements
		Random
		Shape - which Rect, Circ etc could inherit from
		Fix kdTree

*/



int main() 
{

	iArray arr = Cnum::UniformArray<int>({ 3,3 }, 0);
	iArray in = Cnum::Array<int>({ 1,2,3,4,5,6 }, {3,2});
	
	// axis 0
	arr.Concatenate(in, 1);
	arr.Print();
	
 }