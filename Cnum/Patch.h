#pragma once
#include <iostream>
#include "Cnum.h"
#include "DynamicArray.h"

template<typename T>
class Cnum::Patch
{
public: 
	Patch(DynamicArray<T>& lowPoint, DynamicArray<T>& highPoint)
		:m_dim{ lowPoint.shape()[1] }, m_coordinates{ Cnum::Concatenate(lowPoint, highPoint) }
	{}


	bool isOverlappingWith(Cnum::Patch<T>& other) {

		for (int i = 0; i < m_coordinates.shape().size(); i++) {
			auto coords = m_coordinates.ExtractAxis(1, i);
			if (other.m_coordinates.Contains(coords)) {
				return true;
			}
		}
		return false;
	}



private:
	DynamicArray<T> m_coordinates;
	int m_dim; 
};

