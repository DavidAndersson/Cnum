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

*/



int main() 
{

	iArray test = Cnum::Array<int>({ 9,8,7,6,5,4,3,2,1 }, { 3,3 });

	test.Sort(1).Print();
	
}