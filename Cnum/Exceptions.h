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

	template<typename T>
	static bool EnsureSameShape(DynamicArray<T>& arr1, DynamicArray<T>& arr2)
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
			throw std::invalid_argument(std::format("Invalid shapes, {} is not of the same size as {}", toString(arr1), toString(arr2)));
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
			throw std::invalid_argument(std::format("Array must be of size {} but {} was supplied", dim, arr.nDims()));
		}
	}

	template<typename T>
	static bool EnsureSameSizeAlongAxis(const DynamicArray<T>& arr1, const DynamicArray<T>& arr2, int axis, const std::string_view msg)
	{
		if (arr1.shapeAlong(axis) != arr2.shapeAlong(axis)) {
			throw std::invalid_argument(msg);
		}
		return true;
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
			throw std::invalid_argument("Incorrectly specifed permutation");
		}
		return true;
	}

	template<typename T>
	static bool EnsureLargerDimThan(const DynamicArray<T>& arr, int dim)
	{
		if (arr.nDims() < dim) {
			throw std::invalid_argument(std::format("The dimension of the array must be larger than {} to perform this operation", arr.sshape()));
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
					std::cerr << std::format("Error! {} is out of range for axis {} in shape {}", nonAxisIndex[idx], i, arr.sshape()) << std::endl;
					exit(0);
				}
				idx++;
			}
		}
	}



};


