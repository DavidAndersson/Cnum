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


	auto t = Cnum::Array<int>({ 6,3,6,4,6,6,3,2,4,6,8,5,4,2,5,7 }); 
	t.Transpose();

	auto out = t.argSort(); 

	out.Print();

 }