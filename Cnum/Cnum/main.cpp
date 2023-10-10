#include "Cnum.h"
#include <iostream>
#include "../PhysicsEngine/Core.cpp"; 
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
		Single int indexing in [] seems to be broken, at least for bool. True becomes false as the function returns to the parent stack frame
		Don't return vectors from functions if not necessary

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

}