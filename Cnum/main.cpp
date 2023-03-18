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

	iArray arr = Cnum::UniformArray<int>({ 2,2,2 }, 0);
	iArray in = Cnum::Array<int>({ 1,2,3,4,5,6,7,8,9,10,11,12 }, { 2,2,3 });
	arr.Insert(in, 2, 1);
	auto result = Cnum::Array<int>({ 0,1,2,3,0,0,4,5,6,0,0,7,8,9,0,0,10,11,12,0 }, { 2,2,5 });

	arr.Print(); 
	result.Print();
	
 }