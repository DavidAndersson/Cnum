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

	//--------------------------
	// Constants
	// -------------------------

	static constexpr double pi = 3.14159265359;
	static constexpr double e = 2.71828182846;

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
			if (isInsideRangeSym(start, end, stepSize)) {
				if (start > end)
					stepSize *= -1;

				std::vector<T> initializer;
				for (T i = start; i <= end; i += stepSize) {
					initializer.push_back(i);
				}

				return DynamicArray<T>{initializer};
			}
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
		
	}

	template<typename T>
	static DynamicArray<T> Linspace(T start, T end, int nSteps) {
		T rangeLength = end - start;
		T stepSize = rangeLength / (nSteps - 1);

		std::vector<T> initializer;
		for (int i = 0; i < nSteps; i++) {
			initializer.push_back(start + i * stepSize);
		}

		return DynamicArray<T>{initializer};
	}

	template<typename T>
	static DynamicArray<T> FromFile(std::string_view filePath, char delimiter = ' ') {

		try {
			auto dataFile = OpenFile(filePath);
			return ReadDataFromFile(dataFile, delimiter);
		}
		catch (const std::runtime_error& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	}

	template<typename T>
	static DynamicArray<T> UniformArray(std::vector<int> shape, T value) {
		return DynamicArray(shape, value);
	}

	template<typename T>
	static DynamicArray<T> Array(std::vector<T>&& initiallizer, std::vector<int> shape) {
		try {
			return DynamicArray<T>(initiallizer, shape);
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	}

	template<typename T>
	static DynamicArray<T> Array(std::vector<T>&& initiallizer) {
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
	static DynamicArray<T> Concatenate(DynamicArray<T>& arr1, DynamicArray<T>& arr2, int axis = 0, int offset = -1) {
		return Concatenate(std::move(arr1), std::move(arr2), axis, offset);
	}
	template<typename T>
	static DynamicArray<T> Concatenate(DynamicArray<T>&& arr1, DynamicArray<T>&& arr2, int axis=0, int offset=-1) {
		
		/*try {
			auto copy = arr1;
			copy.Concatenate(arr2, axis, offset);
			return copy;
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}*/

		std::vector<T> result = arr1.raw();
		std::vector<int> result_shape = arr1.shape();
		auto arr2_data = arr2.raw();

		// If the array is uninitialized i.e. empty, the concatenation will simply act as assignment
		if (result.empty()) {
			return arr2;
		}

		// The off-axis dimensions must all be the same for a valid concatenation
		for (int i = 0; i < arr1.shape().size(); i++) {
			if (i == axis)
				continue;
			if (arr1.shapeAlong(i) != arr2.shapeAlong(i)) {
				throw std::invalid_argument(std::format("Arrays are not of equal length in axis {}", axis));
			}
		}

		int stride = arr1.getStride(axis);


		if (axis == 0) {
			auto startPoint = (offset == -1) ? result.end() : result.begin() + offset * stride;
			result.insert(startPoint, arr2_data.begin(), arr2_data.end());
			result_shape[axis] += arr2.shapeAlong(axis);
		}

		else {

			int newNumberOfElements = (int)result.size() + arr2.size();

			// Update the shape first so that the stepLength can be computed correctly
			result_shape[axis] += arr2.shapeAlong(axis);
			int stepLength = stride * arr1.shapeAlong(axis);

			int startIndex = (offset == -1) ? stepLength - 1 : offset;  // Most likely wrong. Maybe stride*offset

			int j = 0;
			for (int i = startIndex; i < newNumberOfElements; i += stepLength) {
				result.insert(result.begin() + i, arr2_data[j]);
				j++;
			}
		}

		return DynamicArray<T>(result, result_shape);
	}

	template<typename T>
	static DynamicArray<T> Transpose(DynamicArray<T>& arr) {
		auto copy = arr; 
		copy.Transpose();
		return copy;
	}
	template<typename T>
	static DynamicArray<T> Transpose(DynamicArray<T>& arr, DynamicArray<int>&& permutation) {
		auto copy = arr;
		copy.Transpose(std::move(permutation));
		return copy;
	}
	template<typename T>
	static DynamicArray<T> Transpose(DynamicArray<T>& arr, std::vector<int>&& permutation) {
		auto copy = arr;
		copy.Transpose(permutation);
		return copy;
	}

	template<typename T>
	static DynamicArray<T> Flatten(DynamicArray<T>& arr) {
		auto copy = arr;
		copy.Flatten();
		return copy;
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


private:

	//--------------------------
	// Private Member Functions
	// -------------------------

	static std::fstream OpenFile(std::string_view filePath) {
		std::fstream dataFile(filePath.data(), std::ios::in);

		if (dataFile.is_open() == false) {
			throw std::runtime_error(std::format( "Could not open file: {}", filePath));
		}
		else {
			return dataFile;
		}
	}

	template<typename T>
	static DynamicArray<T> ReadDataFromFile(std::fstream dataFile, char delimiter = ' ') {

		int rowCount = 0, colCount = 0, finalColCount = 0;
		std::vector<T> initializer;

		for (std::string line; std::getline(dataFile, line);) {
			colCount = 0;
			std::stringstream ss(line);
			for (std::string element; std::getline(ss, element, delimiter);) {
				initializer.push_back((T)stod(element));
				colCount++;

				if (rowCount > 0 && colCount > finalColCount) {
					throw std::runtime_error("Non consistent column count in file");
				}
			}
			finalColCount = colCount;
			rowCount++;
		}
		return DynamicArray<T>(initializer, std::vector<int>{rowCount, finalColCount});
	}

	template<typename T>
	static bool isInsideRange(T low, T high, T value) {

		if ((high - low) < value) {
			throw std::invalid_argument(std::format("Value {} not inside range ({}, {})", value, low, high));
			exit(0);
		}
		return true;
	}
	template<typename T>
	static bool isInsideRangeSym(T low, T high, T value) {

		if (std::abs(high - low) < value) {
			throw std::invalid_argument(std::format("Value {} not inside range +-({}, {})", value, low, high));
			exit(0);
		}
		return true;
	}


};

