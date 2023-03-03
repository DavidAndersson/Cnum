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
	class Patch;


	template<typename T>
	static Cnum::Patch<T> GetPatch(DynamicArray<T> lowPoint, DynamicArray<T> highPoint) {
		return Patch(lowPoint, highPoint);
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
	static DynamicArray<T> abs(DynamicArray<T>& arr) {
		auto copy = arr; 
		return copy.abs(); 
	}

	template<typename T>
	static DynamicArray<T> Concatenate(DynamicArray<T>& arr1, DynamicArray<T>& arr2, int axis=0) {
		
		try {
			auto copy = arr1;
			copy.Concatenate(arr2, axis);
			return copy;
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	}

};

