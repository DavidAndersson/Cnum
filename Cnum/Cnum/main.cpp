#include "Cnum.h"
#include <iostream>
#include "Quaternion.h"
#include "ndArray.h"

/*
	TODOs: 
		Unit tests
		Print in which function an exception is thrown
		Mathemathical functions mapped to all elements
		Random
		Shape - which Rect, Circ etc could inherit from
		Fix kdTree

		Add extractIndicesAlongAxis(axis, nonAxisIndices, condition) alt. extractIndicesAlongAxis(axis, nonAxisIndices, indices)

		Custom Exception class which uses std::location described in "Professional C++"

		Add concepts and make many functions follow the input arrayLike

		Use ranges for algorithms such as sort etc


		PRIO. 
		Add diff_1d and intersect_1d

		

*/


using namespace Cnum;

int main() 
{	

	iArray arr = Array::initializedArray<int>({ 1,2,3,4,5,6,7,8,9,10,11,12 }, { 2,2,3 });
	arr.print();
	arr.transpose({ 0,2,1 });
	arr.print();

}