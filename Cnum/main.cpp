#include "Cnum.h"
#include <iostream>


/*
	TODOs: 
		Sorting
		Unit tests
		Mathemathical functions mapped to all elements
		Random
		Shape - which Rect, Circ etc could inherit from
		Fix kdTree

*/



int main() {

	iArray test = Cnum::Arange(10); 
	iArray test2 = Cnum::Arange(10, 0, 1);

	auto tst2 = Cnum::BlendIf(test, test2, test > 5);

	auto out = tst2.SortAndFlatten();

	out.second.Print();

 }