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
		Single int indexing in [] seems to be broken, at least for bool. True becomes false as the function returns to the parent stack frame

		Add extractIndicesAlongAxis(axis, nonAxisIndices, condition) alt. extractIndicesAlongAxis(axis, nonAxisIndices, indices)

*/



int main() 
{

	//iArray test = Cnum::Array<int>({ 1,2,3,4,5,6,7,8 }, { 2,2,2 });

	//test.Sort(1).Print();

	/*auto table = Cnum::GetBinaryTable<int>(3); 

	iArray test = table.ExtractAxis(0, 1); 

	auto t = test.Find(test == 1); 

	t.Print();*/

	DynamicArray<bool> test = Cnum::Array<bool>({ true, false, true, false });
	DynamicArray<float> test2 = Cnum::Array<float>({ 1,0,1,0 });



	test2[1] = 1;

	test2.Print();

	auto t = 1 && 1;

	std::cout << t << std::endl;

	
}