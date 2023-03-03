#include "kdTree.h"
#include "Vec2.h"
#include "Rect.h"
#include "DynamicArray.h"
#include "Cnum.h"
#include "Patch.h"

// Make the priority scheme for Rect overlapping more explicit


int main() {


	auto test = Cnum::GetPatch(Cnum::Array<float>({0,0}), Cnum::Array<float>({1, 1}));

	auto other = Cnum::GetPatch(Cnum::Array<float>({0.5, 0.5}), Cnum::Array<float>({1, 1}));

	if (other.isOverlappingWith(test)) {
		std::cout << "Hello" << std::endl;
	}


}