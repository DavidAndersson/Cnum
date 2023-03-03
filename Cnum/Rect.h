#pragma once
#include <vector>
#include "Cnum.h"
#include "DynamicArray.h"

template<typename T>
class Cnum::Rect
{
public: 
	Rect(DynamicArray<T>& lowPoint, DynamicArray<T>& highPoint)
		:m_dim{ lowPoint.shape()[1] }, m_coordinates{ Cnum::Concatenate(lowPoint, highPoint) }
	{}


	bool isOverlappingWith(Cnum::Rect<T>& other)const {

		for (int i = 0; i < m_coordinates.shape().size(); i++) {
			auto coords = m_coordinates.ExtractAxis(1, i);
			if (other.m_coordinates.Contains(coords)) {
				return true;
			}
		}
		return false;
	}

	// Not tested
	DynamicArray<T> Center() {
		return DynamicArray<T>((m_coordinates.getRow(0) + m_coordinates.getRow(1)) / 2);
	}

	std::vector<Rect> Subdivide() const{

		int nChildren = std::pow(2, m_dim);
		int x, y, z = 0;
		std::vector<Rect> children;
		DynamicArray<T> coords = m_coordinates.getRow(0).Concatenate(this->Center()).Concatenate(m_coordinates.getRow(0));

		for (int i = 0; i < nChildren; i++) {

			if (i % 4)
				x++;
			if (i % 3)
				y++;
			if (i % 2)
				z++;

			

		}

	}

private:
	DynamicArray<T> m_coordinates;
	int m_dim; 
};

