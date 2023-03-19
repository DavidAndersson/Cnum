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
	auto arr = Cnum::Array<int>({ 1,2,-5, 1 });
	auto test = Cnum::Abs(arr);

	test.Print();
	arr.Print();
	
}