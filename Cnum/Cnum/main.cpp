#include "Cnum.h"
#include <iostream>
#include "Quaternion.h"
#include "DynamicArray.h"

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
	fArray rotated( {0.0f, 1.0f, 0.0f });

	rotated.rotate(Rotation::Axis::X, Rotation::Degrees(90));
	rotated.print();

	rotated.reverse();
	rotated.print();


	iArray arr = ndArray::uniformArray({3,3}, 1);
	arr.adjacentDiff(1, false);
	arr.print();

	
	iArray arr2 = ndArray::initializedArray<int>({ 1,2,3,4 }, {1,4});
	iArray arr3{ 5,0,-1,2 }; 

	iArray res = ndArray::blend_if(arr2, arr3, arr2>arr3); 
	res.print();

	res.raiseTo(2); 
	res.print();

	res.append(2);
	res.print();

	iArray test; 
	test.append(1); 
	test.append(1);
	test.reshape({ 2,1 });
	test.append(1);
	test.print();


}