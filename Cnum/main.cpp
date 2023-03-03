#include "kdTree.h"
#include "Rect.h"
#include "DynamicArray.h"
#include "Cnum.h"
#include "Rect.h"

// Make the priority scheme for Rect overlapping more explicit


int main() {


	auto test = Cnum::GetRect(Cnum::Array<float>({0,0}), Cnum::Array<float>({1, 1}));

	auto other = Cnum::GetRect(Cnum::Array<float>({0.5, 0.5}), Cnum::Array<float>({1, 1}));

	if (other.isOverlappingWith(test)) {
		std::cout << "Hello" << std::endl;
	}


	iArray bla = Cnum::Array<int>({ 1,2,3,4,5,6 }, { 2,3 }); 
	iArray gla = bla.getRow(1);


}