#pragma once
#include <vector>
#include "Cnum.h"
#include "DynamicArray.h"

template<typename T>
class Cnum::Rect
{
public: 
	Rect(DynamicArray<T> lowPoint, DynamicArray<T> highPoint)
		:m_dim{ lowPoint.shapeAlong(1) }
	{
		if (lowPoint.nDims() != > 1 || highPoint.nDims() > 1) {
			throw std::invalid_argument("Invalid coordinates. Must be 1 dimensional");
			exit(1);
		}
		if (lowPoint.shapeAlong(0) > 1) {
			lowPoint.Transpose();
		}	
		if (highPoint.shapeAlong(0) > 1) {
			highPoint.Transpose();
		}
			
		m_coordinates = Cnum::Concatenate(lowPoint, highPoint);
	}

	bool isOverlappingWith(Cnum::Rect<T>& other)const {

		for (int i = 0; i < m_coordinates.shape().size(); i++) {
			auto coords = m_coordinates.ExtractAxis(1, i);
			if (other.m_coordinates.Contains(coords)) {
				return true;
			}
		}
		return false;
	}

	DynamicArray<T> Center()const {
		return DynamicArray<T>( m_coordinates.ReduceAlongAxis(0) / (T)2 );
	}

	std::vector<Rect> Subdivide(){

		int nChildren = (int)std::pow(2, m_dim);
		std::vector<Rect> children;

		// Place center coords in between the two other coords
		DynamicArray<T> coords = Cnum::Concatenate(m_coordinates, this->Center(), 0, 1);

		auto table = Cnum::GetBinaryTable<T>(m_dim);

		for (int i = 0; i < nChildren; i++) {
			auto indices = table.ExtractAxis(1, i); 

			std::vector<T> low; 
			std::vector<T> high;

			for (int j = 0; j < indices.size(); j++) {
				low.push_back(coords[{ (int)indices[j], j }]);
				high.push_back(coords[{(int)indices[j]+1, j }]);
			}

			Cnum::Rect<T> child = Rect<T>(DynamicArray<T>(low), DynamicArray<T>(high));
			children.push_back(child);
		}

		return children;
	}

private:
	DynamicArray<T> m_coordinates;
	int m_dim; 
};

