#pragma once
#include <vector>
#include "Cnum.h"
#include <assert.h>

template<typename T>
class Cnum::Rect
{
public: 
	Rect(ndArray<T> lowPoint, ndArray<T> highPoint)
		:m_dim{ lowPoint.shapeAlong(1) }
	{
		if (lowPoint.nDims() != > 1 || highPoint.nDims() > 1) {
			throw std::invalid_argument("Invalid coordinates. Must be 1 dimensional");
			exit(1);
		}
		if (lowPoint.shapeAlong(0) > 1) {
			lowPoint.transpose();
		}	
		if (highPoint.shapeAlong(0) > 1) {
			highPoint.transpose();
		}
			
		m_coordinates = Cnum::concatenate(lowPoint, highPoint);
	}

	bool isOverlappingWith(Cnum::Rect<T>& other)const {

		for (int i = 0; i < m_coordinates.shape().size(); i++) {
			auto coords = m_coordinates.extract(1, i);
			if (other.m_coordinates.Contains(coords)) {
				return true;
			}
		}
		return false;
	}

	bool Contains(ndArray<T>& point)const
	{
		assert(point->nDims() == 1); 
		assert(m_dim == point.size()); 

		// Goes through the elements of the point, and checks if it is contained by the correct axis in the parent
		// Note that the method is inclusive at the lower end and exclusive at the upper end
		for (int i = 0; i < point.m_data.size(); i++) {
			if ((point[i] >= m_coordinates.extract(0, i).min() && point[i] < m_coordinates.extract(0, i).max()) == false) {
				return false;
			}
		}
		return true;
	}

	ndArray<T> Center()const {
		return ndArray<T>( m_coordinates.reduceAlongAxis(0) / (T)2 );
	}

	std::vector<Rect> Subdivide(){

		int nChildren = (int)std::pow(2, m_dim);
		std::vector<Rect> children;

		// Place center coords in between the two other coords
		ndArray<T> coords = Cnum::concatenate(m_coordinates, this->Center(), 0, 1);

		auto table = Cnum::GetBinaryTable<T>(m_dim);

		for (int i = 0; i < nChildren; i++) {
			auto indices = table.extract(1, i); 


			// Holding the coordinates of the low and high point of the Rect
			ndArray<T> low; 
			ndArray<T> high;

			// Correctly setting the coorindates of the children based on the binary table and the parent coordinates
			for (int j = 0; j < indices.size(); j++) {
				low.append(coords[{ (int)indices[j], j }]);
				high.append(coords[{(int)indices[j]+1, j }]);
			}

			Cnum::Rect<T> child = Rect<T>(low, high);
			children.push_back(child);
		}

		return children;
	}

	

private:
	ndArray<T> m_coordinates;
	int m_dim; 
};

