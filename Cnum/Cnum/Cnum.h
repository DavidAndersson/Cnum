#pragma once
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <numeric>
#include <functional>
#include <numbers>
#include "Meta.h"
#include "Exceptions.h"
#include <string_view>
#include <fstream>
#include <format>


template<typename T>
class numeric {
public:
	numeric(T val)
		:m_value{val}{}
	numeric() = delete;
	T operator/(T value) { return m_value / value; }
	T operator*(T value) { return m_value * value; }
	T operator+(T value) { return m_value + value; }
	T operator-(T value) { return m_value - value; }
private:
	T m_value;
};


namespace Cnum {

	template<typename T>
	class DynamicArray;

	template<typename T>
	class Rect;


	typedef DynamicArray<int> iArray;
	typedef DynamicArray<float> fArray;
	typedef DynamicArray<double> dArray;


	namespace Constants {
		static constexpr double pi = std::numbers::pi;
		static constexpr double e = std::numbers::e;
	}


	namespace Rotation {
		enum class Axis {
			X,
			Y,
			Z
		};

		struct Radians : public numeric<double>{};
		struct Degrees : public numeric<double>{};

		Radians toRadians(Degrees angleDegrees) {
			return Radians(angleDegrees / (180.0 / Cnum::Constants::pi));
		}

		Degrees toDegrees(Radians angleRadians) {
			return Degrees(angleRadians / (Cnum::Constants::pi / 180.0));
		}

	}



	template<typename T>
	static Rect<T> getRect(DynamicArray<T> lowPoint, DynamicArray<T> highPoint) {
		return Rect(lowPoint, highPoint);
	}
	

	// Static Creators

	// Overload to only take end
	template<typename T>
	static DynamicArray<T> arange(T start, T end, T stepSize) {

		try {
			if (Exceptions::EnsureInsideRangeSym(start, end, stepSize)) {
				
				bool toBeReversed = false;
				if (start > end) {
					std::swap(start, end);
					toBeReversed = true;
				}
				std::vector<T> initializer;
				for (T i = start; i <= end; i += stepSize) {
					initializer.push_back(i);
				}
				if (toBeReversed) {
					std::reverse(initializer.begin(), initializer.end());
				}
				return DynamicArray<T>{initializer};
			}
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
			exit(0);
		}
		
	}

	template<typename T>
	static DynamicArray<T> arange(T end) {
		return Arange(0, end, 1);
	}

	template<typename T>
	static DynamicArray<T> linspace(T start, T end, int nSteps) {
		T rangeLength = end - start;
		T stepSize = rangeLength / (nSteps - 1);

		std::vector<T> initializer;
		for (int i = 0; i < nSteps; i++) {
			initializer.push_back(start + i * stepSize);
		}

		return DynamicArray<T>{initializer};
	}

	//template<typename T>
	/*static DynamicArray<T> fromFile(std::string_view filePath, char delimiter = ' ') {

		try {
			auto dataFile = openFile(filePath);
			return readDataFromFile(dataFile, delimiter);
		}
		catch (const std::exception& ex) {
			std::cout << "Error in FromFile() -> ";
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	}*/

	template<typename T>
	static DynamicArray<T> uniformArray(const iArrayLike_1d auto& shape, T value) {
		return DynamicArray(shape, value);
	}
	template<typename T>
	static DynamicArray<T> uniformArray(const std::initializer_list<T>& shape, T value) {
		return DynamicArray(shape, value);
	}

	template<typename T>
	static DynamicArray<T> array(const ArrayLike_1d auto& init, const iArrayLike_1d auto& shape) {
		try {
			return DynamicArray<T>(init, shape);
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	}
	template<typename T>
	static DynamicArray<T> array(const std::initializer_list<T>& init, const std::initializer_list<int> shape) {
		try {
			return DynamicArray<T>(init, shape);
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	}
	template<typename T>
	static DynamicArray<T> array(const ArrayLike_1d auto& init) {
		return DynamicArray(init);
	}
	template<typename T>
	static DynamicArray<T> array(const std::initializer_list<T>& init) {
		return DynamicArray(init);
	}


	// Static Actions

	template<typename T>
	static T dot(DynamicArray<T>& arr1, DynamicArray<T>& arr2) {

		try {
			Exceptions::EnsureSameSize(arr1, arr2);
			Exceptions::EnsureDim(arr1, 1);
			Exceptions::EnsureDim(arr2, 1);

			return std::inner_product(arr1.begin(), arr1.end(), arr2.begin(), 0);
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	}

	template<typename T>
	static DynamicArray<T> matrixMul(DynamicArray<T>& arr1, DynamicArray<T>& arr2)
	{
		// arr1 and 2 must both be 2d
		// width of arr1 must be equal to the height of arr2

		DynamicArray<T> output = DynamicArray({ arr1.shapeAlong(0), arr2.shapeAlong(1) }, 0);

		for (int i = 0; i < arr1.shapeAlong(0); i++) {
			for (int j = 0; j < arr2.shapeAlong(1); j++) {
				T sum = 0;
				for (int k = 0; k < arr1.shapeAlong(0); k++) {
					sum += arr1.at({ i,k }) * arr2.at({ k,j });
				}
				output.at({ i,j }) = sum;
			}
		}
		return output;
	}


	template<typename T>
	static DynamicArray<T> concatenate(DynamicArray<T> arr1, DynamicArray<T> arr2, int axis) {
		return concatenate(arr1, arr2, axis);
	}
	template<typename T>
	static DynamicArray<T> concatenate(DynamicArray<T>&& arr1, DynamicArray<T>&& arr2, int axis) {
		return arr1.concatenate(arr2, axis);
	}

	template<typename T>
	static DynamicArray<T> transpose(DynamicArray<T> arr) {
		return arr.transpose();
	}
	template<typename T>
	static DynamicArray<T> transpose(DynamicArray<T> arr, std::vector<int>&& permutation) {
		return arr.transpose(permutation);
	}

	template<typename T>
	static DynamicArray<T> flatten(DynamicArray<T> arr) {
		return arr.flatten();
	}

	template<typename T>
	static DynamicArray<T>& abs(DynamicArray<T>&& arr) {
		return arr.abs();
	}

	template<typename T>
	static DynamicArray<T> abs(DynamicArray<T> arr) {
		return arr.abs();
	}

	template<typename T>
	static DynamicArray<T> minimumOf(DynamicArray<T>& arr1, DynamicArray<T>& arr2) 
	{
		try {
			Exceptions::EnsureSameShape(arr1, arr2); 

			DynamicArray<T> out = arr1;
			std::transform(arr1.begin(), arr1.end(), arr2.begin(), out.begin(), [](T v1, T v2) {return std::min(v1, v2); });
			return out;
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
			exit(0);
		}
		

	}

	template<typename T>
	static DynamicArray<T> maximumOf(DynamicArray<T>& arr1, DynamicArray<T>& arr2) {

		try {
			Exceptions::EnsureSameShape(arr1, arr2);
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
			exit(0);
		}
		DynamicArray<T> out = arr1;
		std::transform(arr1.begin(), arr1.end(), arr2.begin(), out.begin(), [](T v1, T v2) {return std::max(v1, v2); });
		return out;

	}

	template<typename T>
	static T max(DynamicArray<T>& arr) {
		return arr.max();
	}
	template<typename T>
	static T min(DynamicArray<T>& arr) {
		return arr.min();
	}


	// Searching
	template<typename T>
	static DynamicArray<int> find(DynamicArray<T>&& arr, DynamicArray<bool>&& condition) {
		return find(arr, condition);
	}
	template<typename T>
	static DynamicArray<int> find(DynamicArray<T>& arr, DynamicArray<bool>&& condition)
	{
		return arr.find(condition);
	}
	template<typename T>
	static DynamicArray<int> find_if(DynamicArray<T>& arr, std::function<bool(T)>&& condition)
	{
		return arr.find_if(condition);
	}



	// Sorting
	template<typename T>
	DynamicArray<T> sortFlat(DynamicArray<T> arr)
	{
		return arr.sortFlat();
	}
	//template<typename T>
	//DynamicArray<T> sort();


	template<typename T>
	static DynamicArray<T> blend_if(DynamicArray<T> trueConditionArray, DynamicArray<T>& falseConditionArray, DynamicArray<bool>&& condition) {
		return trueConditionArray.blend_if(falseConditionArray, condition);
	}
	template<typename T>
	static DynamicArray<T> blend_if(DynamicArray<T> trueConditionArray, DynamicArray<T>& falseConditionArray, DynamicArray<bool>& condition) {
		return trueConditionArray.blend_if(falseConditionArray, condition);
	}
	template<typename T>
	static DynamicArray<T> blend_if(DynamicArray<T> trueConditionArray, DynamicArray<T>&& falseConditionArray, DynamicArray<bool>&& condition) {
		return trueConditionArray.blend_if(falseConditionArray, condition);
	}
	template<typename T>
	static DynamicArray<T> blend_if(DynamicArray<T> trueConditionArray, DynamicArray<T>&& falseConditionArray, bool condition) {
		return trueConditionArray.blend_if(falseConditionArray, DynamicArray<bool>(trueConditionArray.shape(), condition));
	}
	template<typename T>
	static DynamicArray<T> blend_if(DynamicArray<T> trueConditionArray, DynamicArray<T>& falseConditionArray, bool condition) {
		return trueConditionArray.Blend(falseConditionArray, DynamicArray<bool>(trueConditionArray.shape(), condition));
	}

	template<typename T>
	static DynamicArray<T> getBinaryTable(int nDims) {
		
		int nRows = (int)std::pow(2, nDims);
		int nCols = nDims;
		int stride = nRows / 2;
		DynamicArray<T> table;

		// Loop column for column. The first column has first half 0 and second 1. Second column has first quarter 0 second 1 etc etc...

		for (int col = 0; col < nDims; col++) {

			DynamicArray<T> column = Cnum::uniformArray<T>({ nRows, 1 }, 0);

			int value = 0;
			for (int i = stride; i < nRows; i+=2*stride) {
				for (int j = i; j < i + stride; j++) {
					column[j] = 1;
				}
			}
			if (col == 0)
				table = column;
			else
				table.concatenate(column, 1);
			stride /= 2;
		}
		return table;
	}

	template<typename T>
	static void toFile(std::string_view filename, DynamicArray<T>& data, char writeMode = 'w', char delimiter = ' ')
	{
		// Problems:
		//	Two different delimiters can be used if the write mode is append

		if (data.nDims() > 2)
			throw std::runtime_error("Cannot save data in higher dimension than 2");

		if (writeMode != 'w' && writeMode != 'a')
			throw std::invalid_argument("Write mode must be either 'w' or 'a'");

		auto mode = (writeMode == 'w') ? std::ios::out : std::ios::app;
		std::ofstream dataFile(filename.data(), mode);

		int stride = data.getStride();

		for (int i = 0; i < data.size(); i++) {

			std::string s = std::to_string(data.raw()[i]);
			dataFile << s;

			if (fmod(i + 1, stride) == 0)
				dataFile << "\n";
			else
				dataFile << delimiter;
		}
	}


	//--------------------------
	// Private Member Functions
	// -------------------------

	static std::fstream openFile(std::string_view filePath) {
		std::fstream dataFile(filePath.data(), std::ios::in);

		if (dataFile.is_open() == false) {
			throw std::runtime_error(std::format( "Could not open file: {}", filePath));
		}
		else {
			return dataFile;
		}
	}

	template<typename T>
	static DynamicArray<T> readDataFromFile(std::fstream& dataFile, char delimiter = ' ') {

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




};

