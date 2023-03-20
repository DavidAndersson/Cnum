#pragma once
#include <vector>
#include <format>
#include <stdexcept>
#include "Utils.h"

template<typename T>
class DynamicArray;


//-------------------------------------
// Exception Finding Functions
// ------------------------------------


class Exceptions 
{
public: 

	Exceptions() = delete;

	template<typename T, typename S>
	static bool EnsureSameShape(const DynamicArray<T>& arr1, const DynamicArray<S>& arr2)
	{
		if (arr1.sameShapeAs(arr2) == false) {
			throw std::invalid_argument(std::format("Shape {} and {} do not match", arr1.sshape(), arr2.sshape()));
		}
		return true;
	}

	template<typename T>
	static bool EnsureSameSize(const std::vector<T>& arr1, const std::vector<T>& arr2)
	{
		if (multiplyElements(arr1) != multiplyElements(arr2)) {
			throw std::invalid_argument(std::format("{} is not of the same size as {}", toString(arr1), toString(arr2)));
		}
		return true;
	}

	template<typename T>
	static bool EnsureSize(const std::vector<T>& arr, int size, const std::string_view msg)
	{
		if (arr.size() != size) {
			throw std::invalid_argument(msg.data());
		}
		return true;
	}

	template<typename T>
	static bool EnsureDim(const DynamicArray<T>& arr, int dim)
	{
		if (arr.nDims() != dim) {
			throw std::invalid_argument(std::format("{} dimension array is incompatible with {} dimension", arr.nDims(), dim));
		}
		return true;
	}

	template<typename T>
	static void EnsureSameSizeAlongAxis(const DynamicArray<T>& arr1, const DynamicArray<T>& arr2, int axis, std::string_view msg)
	{
		if (arr1.shapeAlong(axis) != arr2.shapeAlong(axis)) {
			throw std::invalid_argument(msg.data());
		}
		//return true;
	}

	template<typename T>
	static bool EnsureNoZeros(const std::vector<T>& arr)
	{
		if (std::find(arr.begin(), arr.end(), 0) != arr.end()) {
			throw std::invalid_argument("Division by zero encountered");
		}
		return true;
	}

	template<typename T>
	static bool EnsurePermutation(const DynamicArray<T>& arr, std::vector<int>& permutation)
	{
		std::vector<int> permValues = std::vector<int>(arr.nDims());
		std::iota(permValues.begin(), permValues.end(), 0);
		if (std::is_permutation(permutation.begin(), permutation.end(), permValues.begin(), permValues.end()) == false) {
			throw std::invalid_argument(std::format("{} is not a correct permutation in {} dimensions", toString(permutation), arr.nDims()));
		}
		return true;
	}

	template<typename T>
	static bool EnsureLargerDimThan(const DynamicArray<T>& arr, int dim)
	{
		if (arr.nDims() < dim) {
			throw std::invalid_argument(std::format("The dimension of the array must be larger than {} using dimension {}", arr.sshape(), dim));
		}
		return true;
	}

	template<typename T>
	static bool EnsureValidNonAxisIndex(const DynamicArray<T>& arr, std::vector<int>& nonAxisIndex, int axis)
	{
		int idx = 0;
		for (int i = 0; i < arr.nDims(); i++) {
			if (i != axis) {
				if (nonAxisIndex[idx] > arr.shapeAlong(i)) {
					throw std::invalid_argument(std::format("Error! {} is out of range for axis {} in shape {}", nonAxisIndex[idx], i, arr.sshape()));
				}
				idx++;
			}
		}
		return true;
	}

	template<typename T>
	static bool EnsureSameNonAxisShape(const DynamicArray<T>& arr1, const DynamicArray<T>& arr2, int axis)
	{
		for (int i = 0; i < arr1.nDims(); i++) {
			if (i != axis) {
				if (arr1.shapeAlong(i) != arr2.shapeAlong(i)) {
					throw std::invalid_argument(std::format("The shape must be the same along axis {}", i));
				}
			}
		}
		return true;
	}



	static bool EnsureEqual(int val_1, int val_2, const std::string& msg)
	{
		if (val_1 != val_2)
			throw std::invalid_argument(msg.data());
		return true;
	}

	template<typename T>
	static bool EnsureInsideRange(T value, T low, T high) {

		if ((high - low) < value) {
			throw std::invalid_argument(std::format("Value {} not inside range ({}, {})", value, low, high));
			exit(0);
		}
		return true;
	}
	template<typename T>
	static bool EnsureInsideRangeSym(T low, T high, T value) {

		if (std::abs(high - low) < value) {
			throw std::invalid_argument(std::format("Value {} not inside range +-({}, {})", value, low, high));
			exit(0);
		}
		return true;
	}

};



