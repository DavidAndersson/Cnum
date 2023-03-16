#pragma once
#include <vector>
#include <numeric>
#include <iostream>
#include <string>
#include <fstream>
#include <format>
#include <algorithm>
#include <iterator>
#include <functional>
#include <math.h>
#include "Utils.h"
#include "Exceptions.h"
#include <string_view>

template<typename T>
class DynamicArray
{
public:

	//--------------------------
	// Constructors
	// -------------------------

	DynamicArray() = default;
	DynamicArray(std::vector<T>& initializer, std::vector<int>&& shape)
		: DynamicArray(std::move(initializer), std::move(shape))
	{}
	DynamicArray(std::vector<T>& initializer, std::vector<int>& shape)
		: DynamicArray(std::move(initializer), std::move(shape))
	{}
	DynamicArray(std::vector<T>& initializer)
		: DynamicArray(std::move(initializer))
	{}
	DynamicArray(std::vector<int>&& shape, T initialValue)
		: m_shape{ shape } 
	{
		m_data = std::vector<T>(getNumberOfElements(), initialValue);

		if (m_shape.size() == 1) {
			m_shape = std::vector<int>{ m_shape[0], 1 };
		}
	};
	DynamicArray(std::vector<int>& shape, T initialValue)
		: DynamicArray(std::move(shape), initialValue)
	{}

	// Copy Constructors
	DynamicArray(const DynamicArray<T>& src)
		: m_data(src.m_data), m_shape(src.m_shape) {
		std::cout << "Made a copy" << std::endl;
	};

	// Move Constructors
	DynamicArray(const DynamicArray<T>&& other)noexcept
		: m_data{other.m_data}, m_shape{other.m_shape}
	{}
	DynamicArray(const std::vector<T>&& other)
		: m_data{other}
	{
		m_shape = std::vector<int>{ 1, (int)other.size() };
	}
	DynamicArray(const std::vector<T>&& other, const std::vector<int>&& shape) 
		: m_data{other}, m_shape{shape}
	{	
		try {
			std::string_view msg(std::string_view(std::format("Cannot create array of size {} with shape {}", this->size(), toString(shape))));
			Exceptions::EnsureEqual(getNumberOfElements(shape), this->size(), msg);
			
			if (this->size() == 1) {
				m_shape = std::vector<int>{ m_shape[0], 1 };
			}
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	}
	DynamicArray(const std::vector<T>&& other, const std::vector<int>& shape)
		: DynamicArray(std::move(other), std::move(shape))
	{}


	//--------------------------
	// Operators
	// -------------------------

	// Indexing
	T& operator[](std::vector<int>& index)
	{
		return m_data.at(flattenIndex(index));
	}
	T& operator[](DynamicArray<int>& index) {
		try {
			Exceptions::EnsureSameSize(index.size(), this->shape());
			return m_data.at(flattenIndex(index.raw()));
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	}
	T operator[](int index)
	{
		return m_data[index];
	}
	DynamicArray<T> operator[](DynamicArray<bool>&& logicalIndices) {
		DynamicArray<T> data;
		for (int i = 0; i < logicalIndices.size(); i++) {
 			if (logicalIndices.raw()[i] == true)
				data.append(m_data[i]);
		}
		return data;
	}
	DynamicArray<T> operator[](DynamicArray<bool>& logicalIndices) {
		DynamicArray<T> data;
		for (int i = 0; i < logicalIndices.size(); i++) {
			if (logicalIndices.raw()[i] == true)
				data.append(m_data[i]);
		}
		return data;
	}

	// Assignment
	DynamicArray<T> operator=(DynamicArray<T>& rhs) { 
		return *this = std::move(rhs);
	}
	DynamicArray<T> operator=( DynamicArray<T>&& rhs) noexcept {
		m_data = rhs.m_data;
		m_shape = rhs.m_shape;
		return *this;
	}
	DynamicArray<T> operator=(std::vector<T>&& rhs)const {
		return DynamicArray<T>(rhs);
	}
	DynamicArray<T> operator=(const std::vector<T>& rhs)const {
		return DynamicArray<T>(rhs);
	}

	// Addition
	DynamicArray<T> operator+(const DynamicArray& rhs)const
	{
		return *this + std::move(rhs);
	}
	DynamicArray<T> operator+(const DynamicArray&& rhs)const
	{
		try {
			Exceptions::EnsureSameShape(*this, rhs);
			return PerformArithmeticOperation(*this, std::move(rhs), std::plus<>());
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	}
	DynamicArray<T> operator+(const T value)const
	{
		return *this + DynamicArray<T>(this->shape(), value);
	}
	void operator+=(const DynamicArray& rhs)
	{
		*this = *this + rhs;
	}
	void operator+=(const DynamicArray&& rhs)
	{
		*this = *this + rhs;
	}
	void operator+=(const T rhs) {
		*this = *this + rhs;
	}

	// Subtraction
	DynamicArray<T> operator-(const DynamicArray& rhs)const
	{
		return *this - std::move(rhs); 
	}
	DynamicArray<T> operator-(const DynamicArray&& rhs)const
	{
		try {
			Exceptions::EnsureSameShape(*this, rhs);
			return PerformArithmeticOperation(*this, std::move(rhs), std::minus<>());
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	}
	DynamicArray<T> operator-(const T value)const
	{
		 return *this - DynamicArray<T>(this->shape(), value);
	}
	void operator-=(const DynamicArray& rhs)
	{
		*this = *this - rhs;
	}
	void operator-=(const DynamicArray&& rhs)
	{
		*this = *this - rhs;
	}
	void operator-=(const T rhs) {
		*this = *this - rhs;
	}

	// Multiplication
	DynamicArray<T> operator*(const DynamicArray& rhs)const
	{
		return *this * std::move(rhs); 
	}
	DynamicArray<T> operator*(const DynamicArray&& rhs)const
	{
		try {
			Exceptions::EnsureSameShape(*this, rhs);
			return PerformArithmeticOperation(*this, std::move(rhs), std::multiplies<>());
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	}
	DynamicArray<T> operator*(const T value)const
	{
		return *this * DynamicArray<T>(this->shape(), value);
	}
	void operator*=(const DynamicArray& rhs)
	{
		*this = *this * rhs;
	}
	void operator*=(const DynamicArray&& rhs)
	{
		*this = *this * rhs;
	}
	void operator*=(const T rhs) {
		*this = *this * rhs;
	}

	// Division
	DynamicArray<T> operator/(const DynamicArray& rhs)const
	{
		return *this / std::move(rhs);
	}
	DynamicArray<T> operator/(const DynamicArray&& rhs)const
	{
		try {
			Exceptions::EnsureSameShape(*this, rhs);
			Exceptions::EnsureNoZeros(this->raw());
			return PerformArithmeticOperation(*this, std::move(rhs), std::divides<>());
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	}
	DynamicArray<T> operator/(const T value)const
	{
		return *this / DynamicArray<T>(this->shape(), value);
	}
	void operator/=(const DynamicArray& rhs)
	{
		*this = *this / rhs;
	}
	void operator/=(const DynamicArray&& rhs)
	{
		*this = *this / rhs;
	}
	void operator/=(const T rhs) {
		*this = *this / rhs;
	}

	// Equailty
	DynamicArray<bool> operator==(const DynamicArray& rhs)const
	{
		return *this == std::move(rhs);
	}
	DynamicArray<bool> operator==(const DynamicArray&& rhs)const
	{
		try {
			Exceptions::EnsureSameShape(*this, rhs);
			return CreateLogicalArray(*this, std::move(rhs), [](T v1, T v2) {return v1 == v2; });
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	}
	DynamicArray<bool> operator==(const T rhs)const {
		return CreateLogicalArray(*this, rhs, [&](T v) {return v == rhs; });
	}

	// Anti-Equality
	DynamicArray<bool> operator!=(const DynamicArray& rhs)const
	{
		return *this != std::move(rhs);
	}
	DynamicArray<bool> operator!=(const DynamicArray&& rhs)const
	{
		try {
			Exceptions::EnsureSameShape(*this, rhs);
			return CreateLogicalArray(*this, std::move(rhs), [](T v1, T v2) {return v1 != v2; });
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	}
	DynamicArray<bool> operator!=(const T rhs)const {
		return CreateLogicalArray(*this, rhs, [&](T v) {return v != rhs; });
	}

	// Less than
	DynamicArray<bool> operator < (T rhs)const {
		return *this < DynamicArray<T>(this->shape(), rhs);
	}
	DynamicArray<bool> operator < (const DynamicArray& rhs)const
	{
		return *this < std::move(rhs);
	}
	DynamicArray<bool> operator < (const DynamicArray&& rhs)const 
	{
		try {
			Exceptions::EnsureSameShape(*this, rhs);
			return CreateLogicalArray(*this, std::move(rhs), [](T v1, T v2) {return v1 < v2; });
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	}

	// Larger than
	DynamicArray<bool> operator > (T rhs)const {
		return *this > DynamicArray<T>(this->shape(), rhs); 
	}
	DynamicArray<bool> operator > (const DynamicArray& rhs)const
	{
		return *this > std::move(rhs);
	}
	DynamicArray<bool> operator > (const DynamicArray&& rhs)const
	{
		try {
			Exceptions::EnsureSameShape(*this, rhs);
			return CreateLogicalArray(*this, std::move(rhs), [](T v1, T v2) {return v1 > v2; });
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	}

	// Less or equal than
	DynamicArray<bool> operator <= (T rhs)const {
		return *this <= DynamicArray<T>(this->shape(), rhs);
	}
	DynamicArray<bool> operator <= (const DynamicArray& rhs)const
	{
		return *this <= std::move(rhs);
	}
	DynamicArray<bool> operator <= (const DynamicArray&& rhs)const
	{
		try {
			Exceptions::EnsureSameShape(*this, rhs);
			return CreateLogicalArray(*this, std::move(rhs), [](T v1, T v2) {return v1 <= v2; });
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	}

	// Larger or equal than
	DynamicArray<bool> operator >= (T rhs)const {
		return *this >= DynamicArray<T>(this->shape(), rhs);
	}
	DynamicArray<bool> operator >= (const DynamicArray& rhs)const
	{
		return *this >= std::move(rhs);
	}
	DynamicArray<bool> operator >= (const DynamicArray&& rhs)const
	{
		try {
			Exceptions::EnsureSameShape(*this, rhs);
			return CreateLogicalArray(*this, std::move(rhs), [](T v1, T v2) {return v1 >= v2; });
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	}

	// Logical AND operator
	DynamicArray<bool> operator && (const DynamicArray<bool>& rhs)const
	{
		return *this && std::move(rhs);
	}
	DynamicArray<bool> operator && (const DynamicArray<bool>&& rhs)const 
	{
		try {
			Exceptions::EnsureSameShape(*this, rhs); 
			return CreateLogicalArray(*this, rhs, [](bool v1, bool v2) {return v1 && v2; });
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	}

	// Logical OR operator
	DynamicArray<bool> operator || (const DynamicArray<bool>& rhs)const
	{
		return *this || std::move(rhs);
	}
	DynamicArray<bool> operator || (const DynamicArray<bool>&& rhs)const
	{
		try {
			Exceptions::EnsureSameShape(*this, rhs);
			return CreateLogicalArray(*this, rhs, [](bool v1, bool v2) {return v1 || v2; });
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	}

	// Streaming
	friend std::ostream& operator << (std::ostream& stream, DynamicArray& arr) {
		return stream << std::move(arr);
	}
	friend std::ostream& operator << (std::ostream& stream, DynamicArray&& arr) {
		arr.Print();
		return stream;
	}

public:

	//--------------------------
	// Public Interface
	// -------------------------

	// Mutating methods
	DynamicArray<T>& Flatten()
	{
		m_shape = { this->size() };
		return *this;
	}
	DynamicArray<T>& abs()
	{
		std::transform(m_data.begin(), m_data.end(), m_data.begin(), [](T e) {return std::abs(e); });
		return *this;
	}
	DynamicArray<T>& Reshape(const std::vector<int>& newShape)
	{
		try {
			// The new shape must have the same number of elements as the previous had
			Exceptions::EnsureSameSize(this->shape(), newShape);
			m_shape = newShape;
			return *this;
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}	
	}

	DynamicArray<T>& Transpose() {
		std::vector<int> permutation = std::vector<int>(this->nDims());
		std::iota(permutation.begin(), permutation.end(), 0);
		std::reverse(permutation.begin(), permutation.end());
		return this->Transpose(permutation);

	}
	DynamicArray<T>& Transpose(DynamicArray<int>&& permutation) {
		return this->Transpose(std::move((permutation.raw())));
	}
	DynamicArray<T>& Transpose(DynamicArray<int>& permutation) {
		return this->Transpose(std::move(permutation));
	}
	DynamicArray<T>& Transpose(std::vector<int>& permutation) {
		return this->Transpose(std::move(permutation));
	}
	DynamicArray<T>& Transpose(std::vector<int>&& permutation)
	{
		/*
			What is the meaning of a permutation in this context? 
				A permutation, same size as the shape, tells which axis should move to where. 
				If the original shape is (2,3,5) and the permutation is (0, 2, 1), then the new shape is (2, 5, 3)
				
				The default permutation is to reverse the shape
		*/

		try {
			Exceptions::EnsurePermutation(*this, permutation);
			Exceptions::EnsureLargerDimThan(*this, 1);
			std::vector<T> newData = std::vector<T>(m_data.size(), 0);
			std::vector<int> newShape = std::vector<int>(this->shape().size(), 0);

			// Update the shape based on the permutation
			for (int j = 0; j < this->shape().size(); j++) {
				newShape[j] = this->shapeAlong(permutation[j]);
			}

			// Swap places of the data w.r.t the permutation
			for (int i = 0; i < newData.size(); i++) {
				std::vector<int> indices = reconstructIndex(i);
				std::vector<int> temp = std::vector<int>(this->shape().size(), 0);
				for (int j = 0; j < temp.size(); j++) {
					temp[j] = indices[permutation[j]];
				}
				newData[flattenIndex(temp, newShape)] = m_data[i];
			}

			m_data = newData;  m_shape = newShape;
			return *this;
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	} 

	DynamicArray<T>& Concatenate(std::vector<T>&& arr, int axis = 0, int offset = -1) {
		return this->Concatenate(DynamicArray<T>(arr), axis, offset); 
	}
	DynamicArray<T>& Concatenate(std::vector<T>& arr, int axis = 0, int offset = -1) {
		return this->Concatenate(DynamicArray<T>(arr), axis, offset);
	}
	DynamicArray<T>& Concatenate(DynamicArray<T>& arr, int axis = 0, int offset = -1) {
		return this->Concatenate(std::move(arr), axis, offset);
	}
	DynamicArray<T>& Concatenate(DynamicArray<T>&& arr, int axis = 0, int offset = -1) {

		std::vector<T> result = this->raw();
		std::vector<int> result_shape = this->shape();
		auto sourceData = arr.raw();

		// If the array is uninitialized i.e. empty, the concatenation will simply act as assignment
		if (result.empty()) {
			*this = arr; 
			return *this;
		}


		// The off-axis dimensions must all be the same for a valid concatenation
		for (int i = 0; i < this->shape().size(); i++) {
			if (i == axis)
				continue;
			if (this->shapeAlong(i) != arr.shapeAlong(i)) {
				throw std::invalid_argument(std::format("Arrays are not of equal length in axis {}", axis));
			}
		}

		int stride = this->getStride(axis);


		if (axis == 0) {
			auto startPoint = (offset == -1) ? result.end() : result.begin() + offset * stride;
			result.insert(startPoint, sourceData.begin(), sourceData.end());
			result_shape[axis] += arr.shapeAlong(axis);
		}

		else {

			int newNumberOfElements = (int)result.size() + arr.size();

			// Update the shape first so that the stepLength can be computed correctly
			result_shape[axis] += arr.shapeAlong(axis);
			int stepLength = stride * this->shapeAlong(axis);

			int startIndex = (offset == -1) ? stepLength : offset;  // Most likely wrong. Maybe stride*offset

			int j = 0;
			for (int i = startIndex; i < newNumberOfElements; i += stepLength) {
				result.insert(result.begin() + i, sourceData[j]);
				j++;
			}
		}

		m_data = result; m_shape = result_shape; 
		return *this;
	}

	DynamicArray<T>& Blend(DynamicArray<T>&& arr, DynamicArray<bool>&& condition) {
		for (int i = 0; i < condition.size(); i++) {
			if (condition[i] == true) {
				m_data[i] = arr[i];
			}
		}
		return *this;
	}
	DynamicArray<T>& Blend(DynamicArray<T>& arr, DynamicArray<bool>&& condition) {
		return this->Blend(std::move(arr), std::move(condition));
	}

	void append(const T value) {

		try {
			if (m_shape.empty()) {
				m_data.push_back(value);
				m_shape = std::vector<int>{ 1,1 };
				return;
			}
			else {
				Exceptions::EnsureDim(*this, 1);
			}
			m_data.push_back(value);
			if (m_shape[0] == 1)
				m_shape[1]++;
			else
				m_shape[0]++;
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}
	}

	// Reductions
	template<typename Operation>
	T Reduce(T initVal, Operation op)const {
		return std::accumulate(m_data.begin(), m_data.end(), initVal, op);
	}
	DynamicArray<T> ReduceAlongAxis(int axis)const
	{

		try {
			Exceptions::EnsureLargerDimThan(*this, axis + 1);
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}

		// The axis which the sum is along gets reduced to 1
		std::vector<int> returnShape = this->shape();
		returnShape[axis] = 1;
		DynamicArray<T> returnArray(returnShape, 0);

		for (int i = 0; i < getNumberOfElements(returnShape); i++) {
			std::vector<int> nonAxisIndex = getNonAxisIndex(i, axis); 
			returnArray[i] = this->ExtractAxis(axis, nonAxisIndex).Reduce(0, std::plus());
		}
		return returnArray;
	}

	// Extractions
	DynamicArray<T> ExtractAxis(int axis, std::vector<int>nonAxisIndex, int start=0, int end=-1)const
	{
		try {
			Exceptions::EnsureLargerDimThan(*this, axis);
			Exceptions::EnsureSize(nonAxisIndex, this->nDims() - 1, std::format("Error in function - ExtractAxis(). Need to specify {} constraint indices, {} given ", this->nDims() - 1, nonAxisIndex.size()));
			Exceptions::EnsureValidNonAxisIndex(*this, nonAxisIndex, axis);
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}

		// If negative end - wrap around
		if (end < 0)
			end = m_shape[axis] + end;

		// Determine starting index in the array
		auto startIndex = nonAxisIndex;
		for (int i = 0; i <= startIndex.size(); i++) {
			if (i == axis) {
				startIndex.insert(startIndex.begin() + i, start);
			}
		}

		auto endIndex = startIndex;
		endIndex[axis] = end;

		int stride = getStride(axis);
		std::vector<T> initializer;
		for (int i = flattenIndex(startIndex); i <= flattenIndex(endIndex); i += stride) {
			initializer.push_back(m_data[i]);
		}

		return DynamicArray<T>(initializer);
	}
	DynamicArray<T> ExtractAxis(int axis, int nonAxisIndex, int start = 0, int end = -1)const {
		return ExtractAxis(axis, std::vector<int>(1, nonAxisIndex), start, end);
	}

	// Searching
	DynamicArray<int> Find(DynamicArray<bool>&& condition)
	{
		int axis = (this->nDims() > 1) ? 0 : 1; 
		DynamicArray<int> outIndices;
		for (int i = 0; i < condition.size(); i++) {
			if (condition[i] == true) {
				auto idx = this->reconstructIndex(i);
				outIndices.Concatenate(idx, axis);
			}
		}

		return outIndices;
	}

	// Boolean checks
	bool isEqualTo(const DynamicArray<T>& other)const {
		return std::equal(m_data.begin(), m_data.end(), other.m_data.begin());
	}
	bool sameShapeAs(const DynamicArray<T>& other)const {
		return std::equal(m_shape.begin(), m_shape.end(), other.shape().begin());
	}

	// Prints
	void Print()const {
		auto startIndex = std::vector<int>(this->nDims(), 0);
		std::cout << "Cnum::Array(";
		PrintDim(startIndex, 0);
		std::cout << ")" << std::endl;
	}

	// Getters
	std::vector<int> shape()const 
	{ 
		return m_shape; 
	};
	int shapeAlong(int axis)const { 

		try {
			Exceptions::EnsureLargerDimThan(*this, axis); 
			return m_shape[axis];
		}
		catch (const std::invalid_argument& err) {
			std::cout << err.what() << std::endl;
			exit(0);
		}

	}
	std::string sshape()const 
	{
		return toString(this->shape()); 
	};
	int nDims()const {
		int dims = (int)std::count_if(m_shape.begin(), m_shape.end(), [](int dim) {return dim > 1; });
		if (dims == 0 && m_shape[0] == 1)
			return 1;
		else
			return dims;
	}
	int size()const 
	{
		return getNumberOfElements();
	};
	int getStride(int axis = 0)const
	{
		/*
			What is a stride?
				- How far you have to move in the 1d vector to get to the next element along the specified axis
		*/
		return std::accumulate(m_shape.begin() + 1 + axis, m_shape.end(), 1, std::multiplies<>());
	}

	const std::vector<T>& raw()const
	{ 
		return this->m_data;
	};
	std::vector<T> raw() 
	{
		return this->m_data;
	}

	T min()const
	{ 
		return *std::min_element(m_data.begin(), m_data.end());
	}
	T max()const
	{ 
		return *std::max_element(m_data.begin(), m_data.end());
	}

	DynamicArray<T> argMin()const {
		size_t flatIdx = std::min_element(m_data.begin(), m_data.end()) - m_data.begin();
		return reconstructIndex((int)flatIdx);
	}
	DynamicArray<T> argMax()const {
		size_t flatIdx = std::max_element(m_data.begin(), m_data.end()) - m_data.begin();
		return reconstructIndex((int)flatIdx);
	}

private:

	//--------------------------
	// Private Interface
	// -------------------------

	int flattenIndex(std::vector<int>& indices)const
	{
		return flattenIndex(indices, this->shape());
	}
	int flattenIndex(std::vector<int>& indices, std::vector<int> shape)const 
	{
		
		/*
			General Formula:
				The 1D index is generalized into
				x_1 * (nx_2 * ... nx_n - 1) + ... x_n-1 * nx_n-1  + x_n

				where x_1 ... x_n are the indices specified by the user, and nx_1 ... nx_n are the dimension-size for the n-th dimension

				Example: 4D index (1,2,1,0) where the shape is (4,4,4,4).

				1 * 4*4*4 + 2 * 4*4 + 1 * 4 + 0	= 64 + 32 + 4 = 100
			*/

		int index = 0;

		for (int j = 0; j < indices.size() - 1; j++)
			index += indices[j] * std::accumulate(shape.begin() + j + 1, shape.end(), 1, std::multiplies<int>());

		index += indices[indices.size() - 1];
		return index;
	}
	std::vector<int> reconstructIndex(int index)const {

		if (this->nDims() == 1) {
			return std::vector<int>{index};
		}

		std::vector<int> indices = std::vector<int>(this->nDims(), 0);

		for (int i = 0; i < this->nDims(); i++) {

			int stride = getStride(i);
			if (stride > index)
				continue;

			int j = 0;
			for (; j < m_shape[i] - 1 && index - stride >= 0; j++) {
				index -= stride;
			}
			indices[i] = j;

			if (index < 0)
				break;
		}
		return indices;
	}

	int getNumberOfElements()const
	{
		return getNumberOfElements(this->shape());
	}

	static int getNumberOfElements(const std::vector<int>& shape)
	{
		return std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int>());
	}

	void PrintDim(std::vector<int>& index, int dim)const
	{
		// In the lowest recursion (max dim) level - do the print
		if (dim == m_shape.size()-1) {
			int start = flattenIndex(index); 
			std::cout << "[";
			for (int j = start; j < start + m_shape[m_shape.size()-1]; j++) {
				std::cout << m_data[j];
				if (j < start + m_shape[1] - 1) {
					std::cout << " ";
				}
			}
			std::cout << "]";
			if (index[dim - 1] != m_shape[dim - 1] - 1) {
				std::cout << ",";
			}
			return;
		}
		// For the higher ranks, recursively call up wards in rank
		std::cout << "[";
		for (int i = 0; i < m_shape[dim]; i++) {
			index[dim] = i; 
			if (index[dim] != 0) { std::cout << "\t"; PrintSpaces(5 + dim); }
			PrintDim(index, dim + 1);
			if (i == m_shape[dim] - 1) {
				std::cout << "]";
				if (dim > 0 && index[dim - 1] != m_shape[dim - 1] - 1) { std::cout << "," << std::endl; }
			}
			else
				std::cout << std::endl;
		}
	}

	static DynamicArray<bool> CreateLogicalArray(const DynamicArray<T>& arr1, const DynamicArray<T>& arr2, std::function<bool(T, T)> func)
	{
		return CreateLogicalArray(arr1, std::move(arr2), func);
	}
	static DynamicArray<bool> CreateLogicalArray(const DynamicArray<T>& arr1, const DynamicArray<T>&& arr2, std::function<bool(T,T)> func)
	{
		std::vector<bool> out(arr1.size(), false);
		std::transform(arr1.raw().begin(), arr1.raw().end(), arr2.raw().begin(), out.begin(), func);
		return DynamicArray<bool>(out, arr1.shape());
	}
	static DynamicArray<bool> CreateLogicalArray(const DynamicArray<T>& arr1, T value, std::function<bool(T)> func)
	{
		std::vector<bool> out(arr1.size(), false);
		std::transform(arr1.raw().begin(), arr1.raw().end(), out.begin(), func);
		return DynamicArray<bool>(out, arr1.shape());
	}
	
	template<typename Operation>
	static DynamicArray<T> PerformArithmeticOperation(const DynamicArray<T>& arr1, const DynamicArray<T>&& arr2, Operation op)
	{
		std::vector<T> data(arr1.size());
		std::transform(arr1.raw().begin(), arr1.raw().end(), arr2.raw().begin(), data.begin(), op);
		return DynamicArray<T>(data, arr1.shape());
	}

	std::vector<int> getNonAxisIndex(int flatIndex, int axis)const {

		std::vector<int> index = reconstructIndex(flatIndex); 
		index.erase(index.begin() + axis); 
		return index;

	}


private:

	//--------------------------
	// Member variables
	// -------------------------

	std::vector<T> m_data; 
	std::vector<int> m_shape; 

};


typedef DynamicArray<int> iArray;
typedef DynamicArray<float> fArray;
typedef DynamicArray<double> dArray;

