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
	DynamicArray(const std::vector<T>& initializer, const std::vector<int>&& shape)
		: DynamicArray(std::move(initializer), std::move(shape))
	{}
	DynamicArray(const std::vector<T>& initializer, const std::vector<int>& shape)
		: DynamicArray(std::move(initializer), std::move(shape))
	{}
	DynamicArray(const std::vector<T>& initializer)
		: DynamicArray(std::move(initializer))
	{}
	DynamicArray(const std::vector<int>&& shape, T initialValue)
		: m_shape{ shape } 
	{
		m_data = std::vector<T>(getNumberOfElements(), initialValue);

		if (m_shape.size() == 1) {
			m_shape = std::vector<int>{ m_shape[0], 1 };
		}
	};
	DynamicArray(const std::vector<int>& shape, T initialValue)
		: DynamicArray(std::move(shape), initialValue)
	{}

	// Copy Constructors
	DynamicArray(const DynamicArray<T>& src)
		: m_data(src.m_data), m_shape(src.m_shape) 
	{
		std::cout << "Made a copy" << std::endl;
	};

	// Move Constructors
	DynamicArray(DynamicArray<T>&& other)noexcept
	{
		std::swap(*this, other); 
	}
	DynamicArray(const std::vector<T>&& other)
		: m_data{ other }, m_shape{ std::vector<int>{ 1, (int)other.size() } }
	{}
	DynamicArray(const std::vector<T>&& other, const std::vector<int>&& shape) 
		: m_data{other}, m_shape{shape}
	{	
		try {
			std::string msg = std::format("Cannot create array of size {} with shape {}", this->size(), toString(shape));
			Exceptions::EnsureEqual(getNumberOfElements(shape), this->size(), msg);
			
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
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
	T& operator[](std::vector<int>&& index)
	{
		try {
			Exceptions::EnsureDim(*this, (int)index.size());
			return m_data.at(flattenIndex(index));
		}
		catch (const std::exception& ex) {
			std::cout << "Error in [] access -> ";
			std::cout << ex.what() << std::endl;
			exit(0);
		}
		
	}
	T& operator[](std::vector<int>& index)
	{
		try {
			Exceptions::EnsureDim(*this, (int)index.size());
			return m_data.at(flattenIndex(index));
		}
		catch (const std::exception& ex) {
			std::cout << "Error in [] access -> ";
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	}
	T& operator[](DynamicArray<int>& index) {
		try {
			Exceptions::EnsureSameSize(index.size(), this->shape());
			return m_data.at(flattenIndex(index.raw()));
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	}
	T& operator[](int index)
	{
		try {
			Exceptions::EnsureDim(*this, 1); 
			return (T&)m_data.at(index);
		}
		catch (const std::exception& ex) {
			std::cout << "Error in [] access -> ";
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	}
	DynamicArray<T> operator[](DynamicArray<bool>&& logicalIndices) {
		try {
			Exceptions::EnsureSameShape(*this, logicalIndices);
			DynamicArray<T> data;
			for (int i = 0; i < logicalIndices.size(); i++) {
				if (logicalIndices.raw()[i] == true)
					data.append(m_data[i]);
			}
			return data;
		}
		catch (const std::exception& ex) {
			std::cout << "Error in [] access -> ";
			std::cout << ex.what() << std::endl;
			exit(0);
		}

		
	}
	DynamicArray<T> operator[](DynamicArray<bool>& logicalIndices) {
		try {
			Exceptions::EnsureSameShape(*this, logicalIndices);
			DynamicArray<T> data;
			for (int i = 0; i < logicalIndices.size(); i++) {
				if (logicalIndices.raw()[i] == true)
					data.append(m_data[i]);
			}
			return data;
		}
		catch (const std::exception& ex) {
			std::cout << "Error in [] access -> ";
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	}

	// Assignment
	DynamicArray<T> operator=(DynamicArray<T>& rhs) { 
		return *this = std::move(rhs);
	}
	DynamicArray<T> operator=( DynamicArray<T>&& rhs) noexcept {
		m_data = rhs.m_data;
		m_shape = rhs.m_shape;
		std::cout << "Made a copy assignment" << std::endl;
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
		catch (const std::exception& ex) {
			std::cout << "Error in array addition -> ";
			std::cout << ex.what() << std::endl;
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
		catch (const std::exception& ex) {
			std::cout << "Error in array subtraction -> ";
			std::cout << ex.what() << std::endl;
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
		catch (const std::exception& ex) {
			std::cout << "Error in array multiplication -> ";
			std::cout << ex.what() << std::endl;
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
		catch (const std::exception& ex) {
			std::cout << "Error in array division -> ";
			std::cout << ex.what() << std::endl;
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
		catch (const std::exception& ex) {
			std::cout << "Error in == operator -> ";
			std::cout << ex.what() << std::endl;
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
		catch (const std::exception& ex) {
			std::cout << "Error in != operator -> ";
			std::cout << ex.what() << std::endl;
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
		catch (const std::exception& ex) {
			std::cout << "Error in < operator -> ";
			std::cout << ex.what() << std::endl;
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
		catch (const std::exception& ex) {
			std::cout << "Error in > operator -> ";
			std::cout << ex.what() << std::endl;
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
		catch (const std::exception& ex) {
			std::cout << "Error in <= operator -> ";
			std::cout << ex.what() << std::endl;
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
		catch (const std::exception& ex) {
			std::cout << "Error in >= operator -> ";
			std::cout << ex.what() << std::endl;
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
		catch (const std::exception& ex) {
			std::cout << "Error in && operator -> ";
			std::cout << ex.what() << std::endl;
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
		catch (const std::exception& ex) {
			std::cout << "Error in || operator -> ";
			std::cout << ex.what() << std::endl;
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
		m_shape = { 1, this->size() };
		return *this;
	}
	DynamicArray<T>& abs()
	{
		std::transform(this->begin(), this->end(), this->begin(), [](T e) {return std::abs(e); });
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
		catch (const std::exception& ex) {
			std::cout << "Error in Reshape() -> ";
			std::cout << ex.what() << std::endl;
			exit(0);
		}	
	}

	DynamicArray<T>& Transpose() {

		if (this->nDims() == 1) {
			std::reverse(m_shape.begin(), m_shape.end()); 
			return *this; 
		}
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
			std::vector<T> newData = std::vector<T>(this->size(), 0);
			std::vector<int> newShape = std::vector<int>(this->nDims(), 0);

			// Update the shape based on the permutation
			for (int j = 0; j < this->nDims(); j++) {
				newShape.at(j) = this->shapeAlong(permutation.at(j));
			}

			// Swap places of the data w.r.t the permutation
			for (int i = 0; i < newData.size(); i++) {
				std::vector<int> indices = reconstructIndex(i);
				std::vector<int> temp = std::vector<int>(this->nDims(), 0);
				for (int j = 0; j < temp.size(); j++) {
					temp.at(j) = indices.at(permutation.at(j));
				}
				newData.at(flattenIndex(temp, newShape)) = m_data.at(i);
			}

			m_data = newData;  m_shape = newShape;
			return *this;
		}
		catch (const std::exception& ex) {
			std::cout << "Error in Transpose() -> ";
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	} 

	DynamicArray<T>& Concatenate(std::vector<T>&& arr, int axis) {
		return this->Join(DynamicArray<T>(arr), axis, -1); 
	}
	DynamicArray<T>& Concatenate(std::vector<T>& arr, int axis) {
		return this->Join(DynamicArray<T>(arr), axis, -1);
	}
	DynamicArray<T>& Concatenate(DynamicArray<T>&& arr, int axis) {
		return this->Join(std::move(arr), axis, -1);
	}
	DynamicArray<T>& Concatenate(DynamicArray<T>& arr, int axis) {
		return this->Join(std::move(arr), axis, -1);
	}
	
	DynamicArray<T>& Insert(std::vector<T>&& arr, int axis, int offset) {
		return this->Join(DynamicArray<T>(arr), axis, offset);
	}
	DynamicArray<T>& Insert(std::vector<T>& arr, int axis, int offset) {
		return this->Join(DynamicArray<T>(arr), axis, offset);
	}
	DynamicArray<T>& Insert(DynamicArray<T>& arr, int axis, int offset) {
		return this->Join(std::move(arr), axis, offset);
	}
	DynamicArray<T>& Insert(DynamicArray<T>&& arr, int axis, int offset) {
		return this->Join(arr, axis, offset);
	}

	DynamicArray<T>& ReplaceAlong(DynamicArray<T>& newData, int axis, std::vector<int>& nonAxisIndices) {
		return this->ReplaceAlong(std::move(newData), axis, std::move(nonAxisIndices)); 
	}
	DynamicArray<T>& ReplaceAlong(DynamicArray<T>& newData, int axis, std::vector<int>&& nonAxisIndices) {
		return this->ReplaceAlong(std::move(newData), axis, std::move(nonAxisIndices));
	}
	DynamicArray<T>& ReplaceAlong(DynamicArray<T>&& newData, int axis, std::vector<int>&& nonAxisIndices) {

		try {
			Exceptions::EnsureSize(nonAxisIndices, this->nDims() - 1); 
			Exceptions::EnsureValidNonAxisIndex(*this, nonAxisIndices, axis); 
			Exceptions::EnsureSize(newData, this->shapeAlong(axis)); 

			int stride = getStride(axis);
			auto start_stop = DetermineStartEndIndexForAxis(axis, nonAxisIndices, 0, -1);
			int j = 0;
			for (int i = start_stop.first; i <= start_stop.second; i += stride) {
				m_data.at(i) = newData[j];
				j++;
			}
			return *this;
		}
		catch (const std::exception& ex) {
			std::cout << "Error in ReplaceAlong() -> ";
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	}

	DynamicArray<T>& Blend(DynamicArray<T>&& arr, DynamicArray<bool>&& condition) {
		
		try {
			Exceptions::EnsureSameShape(*this, condition); 

			for (int i = 0; i < condition.size(); i++) {
				if (condition[i] == true) {
					m_data.at(i) = arr[i];
				}
			}
			return *this;
		}
		catch (const std::exception& ex) {
			std::cout << "Error in Blend() -> ";
			std::cout << ex.what() << std::endl;
			exit(0);
		}	
	}
	DynamicArray<T>& Blend(DynamicArray<T>& arr, DynamicArray<bool>&& condition) {
		return this->Blend(std::move(arr), std::move(condition));
	}

	void append(const T value, int axis = 1) {

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
			m_shape.at(axis)++; 
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
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

			// The axis which the sum is along gets reduced to 1
			std::vector<int> returnShape = this->shape();
			returnShape.at(axis) = 1;
			DynamicArray<T> returnArray(returnShape, 0);

			for (int i = 0; i < getNumberOfElements(returnShape); i++) {
				std::vector<int> nonAxisIndex = getNonAxisIndex(i, axis);
				returnArray[i] = this->ExtractAxis(axis, nonAxisIndex).Reduce(0, std::plus());
			}
			return returnArray;

		}
		catch (const std::exception& ex) {
			std::cout << "Error in ReduceAlongAxis() -> ";
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	}

	// Extractions
	DynamicArray<T> ExtractAxis(int axis, std::vector<int>nonAxisIndex, int start=0, int end=-1)const
	{
		try {
			Exceptions::EnsureLargerDimThan(*this, axis);
			Exceptions::EnsureSize(nonAxisIndex, this->nDims() - 1, std::format("Error in function - ExtractAxis(). Need to specify {} constraint indices, {} given ", this->nDims() - 1, nonAxisIndex.size()));
			Exceptions::EnsureValidNonAxisIndex(*this, nonAxisIndex, axis);

			auto start_stop = DetermineStartEndIndexForAxis(axis, nonAxisIndex, start, end);
			int stride = getStride(axis);
			DynamicArray<T> out;
			for (int i = start_stop.first; i <= start_stop.second; i += stride) {
				out.append(m_data.at(i));
			}
			return out;
		}
		catch (const std::exception& ex) {
			std::cout << "Error in ExtractAxis() -> ";
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	}
	DynamicArray<T> ExtractAxis(int axis, int nonAxisIndex, int start = 0, int end = -1)const {
		return ExtractAxis(axis, std::vector<int>(1, nonAxisIndex), start, end);
	}

	// Searching
	DynamicArray<int> Find(DynamicArray<bool>&& condition)
	{
		try {
			Exceptions::EnsureSameShape(*this, condition); 

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
		catch (const std::exception& ex) {
			std::cout << "Error in Find() -> ";
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	}

	// Sorting
	DynamicArray<int> argSort() 
	{
		try {
			Exceptions::EnsureDim(*this, 1); 
			DynamicArray<int> out(this->shape(), 0);
			std::iota(out.begin(), out.end(), 0);
			std::stable_sort(out.begin(), out.end(), [&](int i1, int i2) { return m_data.at(i1) < m_data.at(i2);  });
			return out;
		}
		catch (const std::exception& ex) {
			std::cout << "Error in argSort() -> ";
			std::cout << ex.what() << std::endl;
			exit(0);
		}

	}
	DynamicArray<int> argSort(int axis) {

		/*
			What are the indices returned? 
				The indices that are returned represent the internal axis-sorting indices. 
				i.e. along the specifed axis, there will be an internal set containing 0-N in 
				the corresponding sorted order

				[[1 0],   would if sorted along axis 0 return [[1 0],
				 [0 2]]									       [0 1]]
		*/

		try {
			Exceptions::EnsureLargerDimThan(*this, axis); 

			DynamicArray<int> axisIdx({ this->shapeAlong(axis) }, 0);
			DynamicArray<int> out(this->shape(), 0);

			int stride = this->getStride(axis);
			int step = stride * this->shapeAlong(axis);
			for (int i = 0; i < this->size(); i += step) {
				DynamicArray<T> data = this->ExtractAxis(axis, getNonAxisIndex(i, axis));
				std::iota(axisIdx.begin(), axisIdx.end(), 0);
				std::stable_sort(axisIdx.begin(), axisIdx.end(), [&](int i1, int i2) { return data[i1] < data[i2];  });
				out.ReplaceAlong(axisIdx, axis, getNonAxisIndex(i, axis));
			}
			return out;
		}
		catch (const std::exception& ex) {
			std::cout << "Error in argSort() -> ";
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	}
	DynamicArray<T>& SortFlat()
	{
		std::sort(this->begin(), this->end()); 
		this->Flatten(); 
		return *this;
	}
	DynamicArray<T>& Sort()
	{
		try {
			Exceptions::EnsureDim(*this, 1);
			std::sort(this->begin(), this->end());
			return *this;
		}
		catch (const std::exception& ex) {
			std::cout << "Error in Sort() -> ";
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	}
	DynamicArray<T>& Sort(int axis) 
	{
		try {
			Exceptions::EnsureLargerDimThan(*this, axis); 

			int stride = this->getStride(axis);
			int step = stride * this->shapeAlong(axis);
			for (int i = 0; i < this->size(); i += step) {
				DynamicArray<T> sortedAxis = this->ExtractAxis(axis, getNonAxisIndex(i, axis)).SortFlat();
				this->ReplaceAlong(sortedAxis, axis, getNonAxisIndex(i, axis));
			}
			return *this;
		}
		catch (const std::exception& ex) {
			std::cout << "Error in Sort() -> ";
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	}


	// Boolean checks
	bool isEqualTo(const DynamicArray<T>& other)const {
		return std::equal(m_data.begin(), m_data.end(), other.m_data.begin());
	}
	
	template<typename S>
	bool sameShapeAs(const DynamicArray<S>& other)const {
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
	const std::vector<int>& shape()const 
	{ 
		return m_shape; 
	}
	std::vector<int> shape()
	{
		return m_shape;
	};
	const int& shapeAlong(int axis)const { 

		try {
			return m_shape.at(axis);
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	}
	int shapeAlong(int axis) {

		try {
			return m_shape.at(axis);
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	}
	std::string sshape()const 
	{
		return toString(this->shape()); 
	};
	int nDims()const {
		int dims = (int)std::count_if(m_shape.begin(), m_shape.end(), [](int dim) {return dim > 1; });
		if (dims == 0 && m_shape.at(0) == 1)
			return 1;
		else
			return dims;
	}
	int size()const 
	{
		return (int)m_data.size();
	};
	int getStride(int axis = 0)const
	{
		/*
			What is a stride?
				- How far you have to move in the 1d vector to get to the next element along the specified axis
		*/

		try {
			Exceptions::EnsureLargerDimThan(*this, axis); 
			return std::accumulate(m_shape.begin() + 1 + axis, m_shape.end(), 1, std::multiplies<>());
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
			exit(0);
		}
		
	}

	// Iterators
	auto begin()const
	{
		return m_data.begin(); 
	}
	auto begin()
	{
		return m_data.begin();
	}
	auto end()const
	{
		return m_data.end();
	}
	auto end()
	{
		return m_data.end();
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

	int flattenIndex(DynamicArray<int>& indices)const
	{
		return this->flattenIndex(m_data, this->shape());
	}
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


		try {

			int index = 0;

			for (int j = 0; j < indices.size() - 1; j++)
				index += indices.at(j) * std::accumulate(shape.begin() + j + 1, shape.end(), 1, std::multiplies<int>());

			index += indices.at(indices.size() - 1);
			return index;
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
			exit(0);
		}
		
	}
	std::vector<int> reconstructIndex(int index)const 
	{
		try {
			if (this->nDims() == 1) {
				return std::vector<int>{index};
			}

			std::vector<int> indices = std::vector<int>(this->nDims(), 0);

			for (int i = 0; i < this->nDims(); i++) {

				int stride = getStride(i);
				if (stride > index)
					continue;

				int j = 0;
				for (; j < m_shape.at(i) - 1 && index - stride >= 0; j++) {
					index -= stride;
				}
				indices.at(i) = j;

				if (index < 0)
					break;
			}
			return indices;
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	}

	int getNumberOfElements()const
	{
		return getNumberOfElements(this->shape());
	}
	int getNonAxisNumberOfElements(int axis)const {
		std::vector<int> shape = this->shape(); 
		shape.erase(shape.begin() + axis); 
		return getNumberOfElements(shape); 
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
				if (j < start + m_shape[m_shape.size() - 1] - 1) {
					std::cout << ", ";
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
		std::transform(arr1.begin(), arr1.end(), arr2.begin(), out.begin(), func);
		return DynamicArray<bool>(out, arr1.shape());
	}
	static DynamicArray<bool> CreateLogicalArray(const DynamicArray<T>& arr1, T value, std::function<bool(T)> func)
	{
		std::vector<bool> out(arr1.size(), false);
		std::transform(arr1.begin(), arr1.end(), out.begin(), func);
		return DynamicArray<bool>(out, arr1.shape());
	}
	
	template<typename Operation>
	static DynamicArray<T> PerformArithmeticOperation(const DynamicArray<T>& arr1, const DynamicArray<T>&& arr2, Operation op)
	{
		std::vector<T> data(arr1.size());
		std::transform(arr1.begin(), arr1.raw().end(), arr2.begin(), data.begin(), op);
		return DynamicArray<T>(data, arr1.shape());
	}

	std::vector<int> getNonAxisIndex(int flatIndex, int axis)const {

		std::vector<int> index = reconstructIndex(flatIndex); 
		index.erase(index.begin() + axis); 
		return index;

	}

	// Deprecated
	void reConfigureData(DynamicArray<int>& indices) {
		auto copy = m_data; 
		for (int i = 0; i < this->size(); i++) {
			m_data[i] = copy[indices[i]];
		}
	}

	int getDominantAxis_1d()
	{
		try {
			Exceptions::EnsureDim(*this, 1);
			return (int)(std::max_element(m_shape.begin(), m_shape.end()) - m_shape.begin());
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
			exit(0);
		}
		
	}
	std::pair<int, int> DetermineStartEndIndexForAxis(int axis, std::vector<int> nonAxisIndex, int start, int end)const
	{ 
		// Determine starting index in the array
		auto startIndex = nonAxisIndex;
		for (int i = 0; i <= startIndex.size(); i++) {
			if (i == axis) {
				startIndex.insert(startIndex.begin() + i, start);
			}
		}

		auto endIndex = startIndex;
		endIndex.at(axis) = (end < 0) ? m_shape.at(axis) + end : end;

		return std::make_pair(flattenIndex(startIndex), flattenIndex(endIndex)); 
	}

	DynamicArray<T>& Join(DynamicArray<T>&& arr, int axis, int offset) {

		// If the array is uninitialized i.e. empty, the join will simply act as assignment
		if (m_data.empty()) {
			*this = arr;
			return *this;
		}
		try {
			Exceptions::EnsureSameNonAxisShape(*this, arr, axis); 

			int stride = this->getStride(axis);

			if (axis == 0) {
				auto startPoint = (offset == -1) ? this->end() : this->begin() + offset * stride;
				m_data.insert(startPoint, arr.begin(), arr.end());
				m_shape[axis] += arr.shapeAlong(axis);
			}
			else {

				int newNumberOfElements = this->size() + arr.size();
				int startIndex = (offset == -1) ? this->shapeAlong(axis) : offset;

				// Update the shape first so that the stepLength can be computed correctly
				m_shape[axis] += arr.shapeAlong(axis);
				int stepLength = stride * this->shapeAlong(axis);
				int j = 0;

				for (int i = startIndex; i < newNumberOfElements; i += stepLength) {
					int offset = j;
					for (int k = 0; k < arr.shapeAlong(axis); k++) {
						m_data.insert(this->begin() + i + k, arr[arr.reconstructIndex(j)]);
						j++;
					}
				}
			}
			return *this;
		}
		catch (const std::exception& ex) {
			std::cout << "Error when trying to join two arrays -> ";
			std::cout << ex.what() << std::endl;
			exit(0);
		}
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

