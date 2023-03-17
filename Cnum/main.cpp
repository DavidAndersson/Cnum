#include "Cnum.h"
#include <iostream>


/*
	TODOs: 
		Sorting
		Unit tests
		Print in which function an exception is thrown
		Mathemathical functions mapped to all elements
		Random
		Shape - which Rect, Circ etc could inherit from
		Fix kdTree

*/



int main() {

	iArray test = Cnum::UniformArray<int>({ 3,3 }, 0); 
	iArray in = Cnum::Array<int>({ 1,2,3 }).Transpose();

	test.Insert(in, 1, 1); 
	test.Print();
	auto res = test.Sort_args(1); 

	res.first.Print();
	res.second.Print();


 }