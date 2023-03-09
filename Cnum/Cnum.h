#pragma once
#include "DynamicArray.h"
#include <algorithm>
#include <iterator>
#include <vector>
#include <numeric>
#include <functional>

class Cnum
{
public:

	template<typename T>
	class Rect;


	template<typename T>
	static Cnum::Rect<T> GetRect(DynamicArray<T> lowPoint, DynamicArray<T> highPoint) {
		return Rect(lowPoint, highPoint);
	}



	Cnum() = delete;
	
	// Static Creators
	template<typename T>
	static DynamicArray<T> Arange(T start, T end, T stepSize) {
		try {
			return DynamicArray<T>::Arange(start, end, stepSize);
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
		}
	}

	template<typename T>
	static DynamicArray<T> Linspace(T start, T end, int nSteps) {
		return DynamicArray<T>::Linspace(start, end, nSteps);
	}

	template<typename T>
	static DynamicArray<T> FromFile(std::string_view filePath, char delimiter = ' ') {
		try {
			return DynamicArray<T>::FromFile(filePath, delimiter);
		}
		catch (const std::runtime_error& err) {
			std::cout << err.what() << std::endl;
		}
		exit(0);
	}

	template<typename T>
	static DynamicArray<T> UniformArray(std::vector<int> shape, T value) {
		return DynamicArray(shape, value);
	}

	template<typename T>
	static DynamicArray<T> Array(std::vector<T> initiallizer, std::vector<int> shape) {
		try {
			return DynamicArray<T>(initiallizer, shape);
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	}

	template<typename T>
	static DynamicArray<T> Array(std::vector<T> initiallizer) {
		return DynamicArray(initiallizer);
	}


	// Static Actions
	template<typename T>
	static void ToFile(std::string_view filename, DynamicArray<T>& data, char writeMode = 'w', char delimiter = ' ') {
		try {
			DynamicArray<T>::ToFile(filename, data, writeMode, delimiter);
		}
		catch (const std::runtime_error& err) {
			std::cout << err.what() << std::endl;
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
		}
	}

	template<typename T>
	static T Dot(DynamicArray<T>& arr1, DynamicArray<T>& arr2) {
		if (arr1.shape().size() > 1 || arr2.shape().size() > 1) {
			std::cerr << "Arrays must be one dimensional for scalar product" << std::endl;
			exit(0);
		}
		else if (arr1.shape().size() != arr2.shape().size()) {
			std::cerr << "Arrays must be of the same size" << std::endl;
			exit(0);
		}

		auto arr1Data = arr1.raw();
		return std::inner_product(arr1Data.begin(), arr1Data.end(), arr2.raw().begin(), 0);
	}

	template<typename T>
	static DynamicArray<T> Concatenate(DynamicArray<T> arr1, DynamicArray<T> arr2, int axis=0, int offset=-1) {
		
		try {
			arr1.Concatenate(arr2, axis, offset);
			return arr1;
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	}

	template<typename T>
	static DynamicArray<T> Transpose(DynamicArray<T>& arr) {
		auto copy = arr; 
		copy.Transpose();
		return copy;
	}

	template<typename T>
	static DynamicArray<T> Transpose(DynamicArray<T> arr, DynamicArray<int> permutation) {
		arr.Transpose(permutation);
		return arr;
	}

	template<typename T>
	static DynamicArray<T> Flatten(DynamicArray<T> arr) {
		arr.Flatten();
		return arr;
	}

	template<typename T>
	static DynamicArray<T> abs(DynamicArray<T> arr) {
		arr.abs();
		return arr;
	}


	template<typename T>
	static DynamicArray<T> GetBinaryTable(int nDims) {
		
		int nRows = (int)std::pow(2, nDims);
		int nCols = nDims;
		int stride = nRows / 2;
		DynamicArray<T> table;

		// Loop column for column. The first column has first half 0 and second 1. Second column has first quarter 0 second 1 etc etc...

		for (int col = 0; col < nDims; col++) {

			DynamicArray<T> column = Cnum::UniformArray<T>({ nRows, 1 }, 0);

			int value = 0;
			for (int i = stride; i < nRows; i+=2*stride) {
				for (int j = i; j < i + stride; j++) {
					column[j] = 1;
				}
			}
			if (col == 0)
				table = column;
			else
				table.Concatenate(column, 1);
			stride /= 2;
		}
		return table;
	}

};

