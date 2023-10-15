#pragma once
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <numeric>
#include <functional>
#include <numbers>
#include "Meta.h"
#include <string_view>
#include <fstream>
#include <format>
#include <assert.h>
#include <string_view>

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
	static Rect<T> getRect(DynamicArray<T>& lowPoint, DynamicArray<T>& highPoint) {
		return Rect(lowPoint, highPoint);
	}
	

	// Static Creators

	namespace ndArray {

		// Creators 

		template<typename T>
		static DynamicArray<T> arange(T start, T end, T stepSize) {
			bool toBeReversed = false;
			if (start > end) {
				std::swap(start, end);
				toBeReversed = true;
			}
			DynamicArray<T> arr;
			for (T i = start; i < end; i += stepSize) {
				arr.append(i);
			}
			if (toBeReversed) {
				arr.reverse();
			}
			return arr;
		}

		template<typename T>
		static DynamicArray<T> arange(T end) {
			return arange(0, end, 1);
		}

		template<typename T>
		static DynamicArray<T> linspace(T start, T end, int nSteps) {
			T rangeLength = end - start;
			T stepSize = rangeLength / (nSteps - 1);

			DynamicArray<T> arr;
			for (int i = 0; i < nSteps; i++) {
				arr.append(start + i * stepSize);
			}
			return arr;
		}

		template<typename T>
		static DynamicArray<T> uniformArray(const iArray& shape, T value) {
			return DynamicArray<T>(shape, value);
		}

		template<typename T>
		static DynamicArray<T> uniformArray(const int size, T value){
			return DynamicArray<T>(iArray{size}, value);
		}

		template<typename T>
		static DynamicArray<T> initializedArray(const DynamicArray<T>&& init, const iArray& shape) {
			return DynamicArray<T>(init, shape);
		}


		// Actions

		template<typename T>
		static DynamicArray<T> abs(DynamicArray<T> arr) {
			return arr.abs();
		}

		template<typename T>
		static DynamicArray<T> blend_if(DynamicArray<T> base, DynamicArray<T>& blendArray, iArray&& condition) {
			return base.blend_if(std::move(blendArray), std::move(condition));
		}

		template<typename T>
		static DynamicArray<T> concatenate(DynamicArray<T> arr1, const DynamicArray<T>& arr2, int axis) {
			return arr1.concatenate(arr2, axis);
		}

		template<typename T>
		static DynamicArray<T> erase(DynamicArray<T> arr, int index) {
			return arr.erase(index);
		}

		template<typename T>
		static iArray find(DynamicArray<T> arr, iArray& condition)
		{
			return arr.find(condition);
		}

		template<typename T>
		static iArray find_if(DynamicArray<T>& arr, std::function<bool(T)>&& condition)
		{
			return arr.find_if(condition);
		}

		template<typename T>
		static DynamicArray<T> flatten(DynamicArray<T> arr) {
			return arr.flatten();
		}

		template<typename T, typename iter>
		static DynamicArray<T> insert(DynamicArray<T> arr, iter it, T value) {
			return arr.insert(it, value);
		}

		template<typename T>
		static DynamicArray<T> insert(DynamicArray<T> base, const DynamicArray<T>& insertion, int axis, int offset) {
			return base.insert(insertion, axis, offset);
		}

		template<typename T>
		static DynamicArray<T> minimumOf(DynamicArray<T> arr1, DynamicArray<T> arr2) {
			return arr1.blend_if(std::move(arr2), arr2 < arr1);
		}

		template<typename T>
		static DynamicArray<T> maximumOf(DynamicArray<T> arr1, DynamicArray<T> arr2) {
			return arr1.blend_if(std::move(arr2), arr2 > arr1);
		}

		template<typename T>
		static DynamicArray<T> normalize(DynamicArray<T> arr) {
			return arr.normalize();
		}

		template<typename T>
		static DynamicArray<T> raiseTo(DynamicArray<T> arr, T exponent){
			return arr.raiseTo(exponent);
		}

		template<typename T, typename Operation>
		static DynamicArray<T> reduce(DynamicArray<T>& arr, T initVal, Operation op) {
			return arr.reduce(initVal, op);
		}

		template<typename T, typename Operation>
		DynamicArray<T> reduceAlongAxis(DynamicArray<T>& arr, int axis, T initValue, Operation op) {
			arr.reduceAlongAxis(axis, initValue, op);
		}

		template<typename T>
		static DynamicArray<T> replace_if(DynamicArray<T> arr, std::function<bool(T)>&& condition, T replacement) {
			arr.replace_if(condition, replacement);
		}

		template<typename T>
		static DynamicArray<T> reshape(DynamicArray<T> arr, iArray&& newShape) {
			return arr.reshape(newShape);
		}

		template<typename T>
		static DynamicArray<T> reverse(DynamicArray<T> arr) {
			return arr.reverse();
		}

		template<typename T>
		static DynamicArray<T> reverse(DynamicArray<T> arr, int axis) {
			return arr.reverse(axis);
		}

		template<typename T>
		static DynamicArray<T> roll(DynamicArray<T> arr, int shift, int axis) {
			return arr.roll(shift, axis);
		}

		template<typename T>
		DynamicArray<T> sort(DynamicArray<T> arr) {
			return arr.sort();
		}

		template<typename T>
		DynamicArray<T> sort(DynamicArray<T> arr, int axis) {
			return arr.sort(axis);
		}

		template<typename T>
		DynamicArray<T> sortFlat(DynamicArray<T> arr)
		{
			return arr.sortFlat();
		}

		template<typename T>
		static DynamicArray<T> transpose(DynamicArray<T> arr) {
			return arr.transpose();
		}

		template<typename T>
		static DynamicArray<T> transpose(DynamicArray<T> arr, iArray&& permutation) {
			return arr.transpose(permutation);
		}

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





	// Static Actions

	template<typename T>
	static T dot(DynamicArray<T>& arr1, DynamicArray<T>& arr2) {

		assert(arr1.size() == arr2.size); 
		assert(arr1->nDims() == 1); 
		assert(arr2->nDims() == 1);
		return std::inner_product(arr1.begin(), arr1.end(), arr2.begin(), 0);
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
	static DynamicArray<T> getBinaryTable(int nDims) {
		
		int nRows = (int)std::pow(2, nDims);
		int nCols = nDims;
		int stride = nRows / 2;
		DynamicArray<T> table;

		// Loop column for column. The first column has first half 0 and second 1. Second column has first quarter 0 second 1 etc etc...

		for (int col = 0; col < nDims; col++) {

			DynamicArray<T> column = Cnum::ndArray::uniformArray<T>({ nRows, 1 }, 0);

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

