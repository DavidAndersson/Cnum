#include "Cnum.h"
#include <iostream>
#include <concepts>

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

*/



void test(const ArrayLike_1d auto& arr)
{
	for (int i = 0; i < arr.size(); i++) {
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;
}



int main() 
{

	auto tst = Cnum::Array<int>({ 1,2,3,4,5,6 }, { 2,3 }); 
	tst.Transpose();

	tst.Concatenate(iArray{ {1,1,1}, {3,1} }, 1); 

	tst.Print();

	auto d = tst.Extract_if(0, iArray{1}, [](int t) {return t > 4; });

	d.Print();
	

}