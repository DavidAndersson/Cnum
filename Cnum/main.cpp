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

	iArray tet = Cnum::Array<int>({1,2,4,5,6, 7}, {3,2});
	auto t = tet[tet == 2];
	t.Print();
	
 }