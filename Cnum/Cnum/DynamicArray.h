#pragma once
#include <vector>
#include <numeric>
#include <iostream>
#include <string>
#include <format>
#include <algorithm>
#include <iterator>
#include <functional>
#include <math.h>
#include "Utils.h"
#include "Meta.h"
#include <assert.h>


namespace Cnum
{

template<typename T>
class DynamicArray
{
public:

	//--------------------------
	// Constructors
	// -------------------------

	DynamicArray() = default;

	// Change all ints - for sizes to size_t

	// Creation by array-like objects
	DynamicArray(const ArrayLike_1d auto& init)
	{
		std::copy(init.begin(), init.end(), std::back_inserter(m_data));
		m_shape = std::vector<int>{ 1, (int)init.size() };
	}
	DynamicArray(const ArrayLike_1d auto& init, const iArrayLike_1d auto& shape)
	{
		try {
			std::string msg = std::format("Cannot create array of size {} with shape {}", init.size(), toString(shape));
			Exceptions::EnsureEqual(getNumberOfElements(shape), (int)init.size(), msg);
			std::copy(init.begin(), init.end(), std::back_inserter(m_data));
			std::copy(shape.begin(), shape.end(), std::back_inserter(m_shape));
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	}
	DynamicArray(const iArrayLike_1d auto& shape, T initialValue)
	{
		std::copy(shape.begin(), shape.end(), std::back_inserter(m_shape));
		if (m_shape.size() == 1) {
			m_shape = std::vector<int>{ 1, m_shape[0] };
		}

		m_data = std::vector<T>(this->getNumberOfElements(), initialValue); 
	}

	// Creation by initializer list
	DynamicArray(const std::initializer_list<T>& init)
		: m_data{std::vector(init)}, m_shape{std::vector<int>{1, (int)init.size()}}
	{}
	DynamicArray(const std::initializer_list<T>& init, const std::initializer_list<int>& shape)
		: m_data{std::vector(init)}, m_shape{std::vector(shape)}
	{}
	DynamicArray(const std::initializer_list<int>& shape, T initialValue)
		: m_data{std::vector(getNumberOfElements(std::vector(shape)), initialValue )}, m_shape{std::vector(shape)}
	{};

	// Creation by size
	DynamicArray(const size_t size)
		: m_shape{ std::vector{1, (int)size} }, m_data{std::vector<T>(size)}
	{}

	
	DynamicArray(const size_t size, const T initialValue)
		: m_shape{ std::vector<int>{1, (int)size} }, m_data{ std::vector<T>(size, initialValue) }
	{}

	// Copy Constructor
	DynamicArray(const DynamicArray<T>& src) = default;

	// Move Constructor
	DynamicArray(DynamicArray&& other) noexcept
		: DynamicArray()
	{
		swap(*this, other);
	}

	// Destructor
	~DynamicArray() = default;

	//--------------------------
	// Operators
	// -------------------------

	T& operator[](const std::initializer_list<int> index)const
	{
		// Make sure that the dimension of the array matches the number of indices passed in
		assert(this->nDims() == index.size());
		return (T&)m_data.at(flattenIndex(DynamicArray{ index }));
 	}
	T& operator[](int index)const
	{
		// Make sure that the array is 1-dimensional
		assert(this->nDims() == 1);
		return (T&)m_data.at(index);
	}
	const DynamicArray<T> operator[](DynamicArray<bool>&& logicalIndices)const {

		assert(this->sameShapeAs(logicalIndices));

		DynamicArray<T> data;
		for (int i = 0; i < logicalIndices.size(); i++) {
			if (logicalIndices.raw()[i] == true)
				data.append(m_data[i]);
		}
		return data;


	}
	const DynamicArray<T> operator[](DynamicArray<bool>& logicalIndices)const {
		
		assert(this->sameShapeAs(logicalIndices)); 
		DynamicArray<T> data;
		for (int i = 0; i < logicalIndices.size(); i++) {
			if (logicalIndices.raw()[i] == true)
				data.append(m_data[i]);
		}
		return data;
		
	}
	T& at(int index)
	{
		assert(this->nDims() == 1); 
		return (T&)m_data.at(index);
	}
	T& at(const DynamicArray<int>& index) 
	{
		assert(this->nDims() == index.size()); 
		return (T&)m_data.at(flattenIndex(index));
	}
	T& at(const std::initializer_list<int>& index)
	{
		assert(this->nDims() == index.size());
		return m_data.at(flattenIndex(std::vector(index)));
	}
	const T& at(int index)const
	{
		assert(this->nDims() == 1);
		return (T&)m_data.at(index);
	}
	const T& at(const DynamicArray<int>& index)const
	{
		assert(this->nDims() == index.size());
		return (T&)m_data.at(flattenIndex(index));
	}
	const T& at(const std::initializer_list<int>& index)const
	{
		assert(this->nDims() == index.size());
		return m_data.at(flattenIndex(std::vector(index)));
	}

	// Assignment
	DynamicArray<T>& operator=(DynamicArray<T> other) 
	{
		// Copy and swap idiom. Copy is made in the parameter list
		swap(*this, other); 
		return *this;
	}

	// Addition
	friend DynamicArray<T> operator+(const DynamicArray& lhs, const DynamicArray& rhs)
	{
		assert(lhs.sameShapeAs(rhs)); 
		return binaryOperation(lhs, rhs, std::plus<>());
	}
	friend DynamicArray<T> operator+(const DynamicArray&& lhs, const DynamicArray&& rhs)
	{
		return lhs + rhs;
	}
	friend DynamicArray<T> operator+(const DynamicArray& lhs, const T value)
	{
		return lhs + DynamicArray<T>(lhs.shape(), value);
	}
	void operator+=(const DynamicArray& rhs)
	{
		*this = *this + rhs;
	}
	void operator+=(const DynamicArray&& rhs)
	{
		*this = *this + rhs;
	}
	void operator+=(const T value) {
		*this = *this + value;
	}

	// Subtraction
	friend DynamicArray<T> operator-(const DynamicArray& lhs, const DynamicArray& rhs)
	{
		assert(lhs.sameShapeAs(rhs));
		return binaryOperation(lhs, rhs, std::minus<>());
	}
	friend DynamicArray<T> operator-(const DynamicArray&& lhs, const DynamicArray&& rhs)
	{
		return lhs - rhs;
	}
	friend DynamicArray<T> operator-(const DynamicArray& lhs, const T value)
	{
		return lhs - DynamicArray<T>(lhs.shape(), value);
	}
	void operator-=(const DynamicArray& rhs)
	{
		*this = *this - rhs;
	}
	void operator-=(const DynamicArray&& rhs)
	{
		*this = *this - rhs;
	}
	void operator-=(const T value) {
		*this = *this - value;
	}
	DynamicArray<T> operator-()
	{
		return unaryOperation(*this, [](T t) {return t *= -1; }); 
	}

	// Multiplication
	friend DynamicArray<T> operator*(const DynamicArray& lhs, const DynamicArray& rhs)
	{
		assert(lhs.sameShapeAs(rhs));
		return binaryOperation(lhs, rhs, std::multiplies<>());
	}
	friend DynamicArray<T> operator*(const DynamicArray&& lhs, const DynamicArray&& rhs)
	{
		return lhs * rhs;
	}
	friend DynamicArray<T> operator*(const T value, const DynamicArray& rhs)
	{
		return rhs * DynamicArray<T>(rhs.shape(), value);
	}
	friend DynamicArray<T> operator*(const DynamicArray& lhs, const T value)
	{
		return value * lhs;
	}
	void operator*=(const DynamicArray& rhs)
	{
		*this = *this * rhs;
	}
	void operator*=(const DynamicArray&& rhs)
	{
		*this = *this * rhs;
	}
	void operator*=(const T value) {
		*this = *this * value;
	}

	// Division
	friend DynamicArray<T> operator/(const DynamicArray& lhs, const DynamicArray& rhs)
	{
		assert(lhs.sameShapeAs(rhs));
		assert(rhs[rhs == 0].size() == 0); 
		return binaryOperation(lhs, rhs, std::divides<>());
	}
	friend DynamicArray<T> operator/(const DynamicArray&& lhs, const DynamicArray&& rhs)
	{
		return lhs / rhs;
	}
	friend DynamicArray<T> operator/(const DynamicArray& rhs, const T value)
	{
		return rhs / DynamicArray<T>(rhs.shape(), value);;
	}
	void operator/=(const DynamicArray& rhs)
	{
		*this = *this / rhs;
	}
	void operator/=(const DynamicArray&& rhs)
	{
		*this = *this / rhs;
	}
	void operator/=(const T value) {
		*this = *this / value;
	}

	// Equailty
	DynamicArray<bool> operator==(const DynamicArray<T>& rhs)const
	{
		return *this == std::move(rhs);
	}
	DynamicArray<bool> operator==(const DynamicArray<T>&& rhs)const
	{
		assert(this->sameShapeAs(rhs)); 
		return createLogicalArray(*this, std::move(rhs), std::equal_to<>());
	}
	DynamicArray<bool> operator==(const T value)const {
		return createLogicalArray(*this, DynamicArray<T>(this->shape(), value), std::equal_to<>());
	}
				 
	// Anti-Equality
	DynamicArray<bool> operator!=(const DynamicArray<T>& rhs)const
	{
		return *this != std::move(rhs);
	}
	DynamicArray<bool> operator!=(const DynamicArray<T>&& rhs)const
	{
		
		return createLogicalArray(*this, std::move(rhs), std::not_equal_to<>());
	}
	DynamicArray<bool> operator!=(const T value)const {
		return createLogicalArray(*this, DynamicArray<T>(this->shape(), value), std::not_equal_to<>());
	}
				 
	// Less than 
	DynamicArray<bool> operator < (T value)const {
		return *this < DynamicArray<T>(this->shape(), value);
	}
	DynamicArray<bool> operator < (const DynamicArray<T>& rhs)const
	{
		return *this < std::move(rhs);
	}
	DynamicArray<bool> operator < (const DynamicArray<T>&& rhs)const
	{
		assert(this->sameShapeAs(rhs));
		return createLogicalArray(*this, std::move(rhs), std::less<>());
	}
				 
	// Larger than
	DynamicArray<bool> operator > (T value)const {
		return *this > DynamicArray<T>(this->shape(), value); 
	}
	DynamicArray<bool> operator > (const DynamicArray<T>& rhs)const
	{
		return *this > std::move(rhs);
	}
	DynamicArray<bool> operator > (const DynamicArray<T>&& rhs)const
	{
		assert(this->sameShapeAs(rhs));
		return createLogicalArray(*this, std::move(rhs), std::greater<>());
	}

	// Less or equal than
	DynamicArray<bool> operator <= (T value)const {
		return *this <= DynamicArray<T>(this->shape(), value);
	}
	DynamicArray<bool> operator <= (const DynamicArray<T>& rhs)const
	{
		return *this <= std::move(rhs);
	}
	DynamicArray<bool> operator <= (const DynamicArray<T>&& rhs)const
	{
		assert(this->sameShapeAs(rhs));
		return createLogicalArray(*this, std::move(rhs), std::less_equal<>());
	}

	// Larger or equal than
	DynamicArray<bool> operator >= (T value)const {
		return *this >= DynamicArray<T>(this->shape(), value);
	}
	DynamicArray<bool> operator >= (const DynamicArray<T>& rhs)const
	{
		return *this >= std::move(rhs);
	}
	DynamicArray<bool> operator >= (const DynamicArray<T>&& rhs)const
	{
		assert(this->sameShapeAs(rhs));
		return createLogicalArray(*this, std::move(rhs), std::greater_equal<>());
	}

	// Logical AND operator
	DynamicArray<bool> operator && (const DynamicArray<bool>& rhs)const
	{
		return *this && std::move(rhs);
	}
	DynamicArray<bool> operator && (const DynamicArray<bool>&& rhs)const
	{
		assert(this->sameShapeAs(rhs));
		return createLogicalArray(*this, rhs, std::logical_and<>());
	}

	// Logical OR operator
	DynamicArray<bool> operator || (const DynamicArray<bool>& rhs)const
	{
		return *this || std::move(rhs);
	}
	DynamicArray<bool> operator || (const DynamicArray<bool>&& rhs)const
	{
		assert(this->sameShapeAs(rhs));
		return createLogicalArray(*this, rhs, std::logical_or<>());
	}

	// Streaming
	friend std::ostream& operator << (std::ostream& stream, const DynamicArray<T>& arr) {
		return stream << std::move(arr);
	}
	friend std::ostream& operator << (std::ostream& stream, const DynamicArray<T>&& arr) {
		arr.print();
		return stream;
	}
	friend std::istream& operator >> (std::istream& stream, DynamicArray<T>& arr)
	{
		assert(this->nDims() == 1); 
		T value;
		stream >> value;
		arr.append(value);
		return stream;
	}

	// Conversion, consider making explicit
	operator std::vector<T>()const
	{
		return m_data;
	}
	operator std::initializer_list<T>()const
	{
		return m_data;
	}

public:

	//--------------------------
	// Public Interface
	// -------------------------

	// Mutating methods
	DynamicArray<T>& flatten()
	{
		m_shape = { 1, (int)this->size() };
		return *this;
	} 
	DynamicArray<T>& abs()
	{
		std::transform(this->begin(), this->end(), this->begin(), [](T e) {return std::abs(e); });
		return *this;
	}
	DynamicArray<T>& reshape(const DynamicArray<int>&&  newShape)
	{
		assert(this->size() == newShape.reduce(1, std::multiplies<>()));
		m_shape.clear();
		std::copy(newShape.begin(), newShape.end(), std::back_inserter(m_shape));
		return *this;	
	}
	DynamicArray<T>& normalize()
	{
		*this /= this->norm();
		return *this;
	}

	DynamicArray<T>& raiseTo(T exponent)
	{
		return unaryOperation(*this, [exponent](T value) {return std::pow(value, exponent); });
	}

	DynamicArray<T>& transpose() {

		if (this->nDims() == 1) {
			std::reverse(m_shape.begin(), m_shape.end()); 
			return *this; 
		}
		std::vector<int> permutation = std::vector<int>(this->nDims());
		std::iota(permutation.begin(), permutation.end(), 0);
		std::reverse(permutation.begin(), permutation.end());
		return this->transpose(permutation);

	}
	DynamicArray<T>& transpose(const DynamicArray<int>&& permutation)
	{
		/*
			What is the meaning of a permutation in this context? 
				A permutation, same size as the shape, tells which axis should move to where. 
				If the original shape is (2,3,5) and the permutation is (0, 2, 1), then the new shape is (2, 5, 3)
				
				The default permutation is to reverse the shape
		*/

		assert(permutation.isPermutation(DynamicArray::arange(0, this->nDims())));
		assert(this->nDims() > 1);

		std::vector<T> newData = std::vector<T>(this->size(), 0);
		std::vector<int> newShape = std::vector<int>(this->nDims(), 0);

		// Update the shape based on the permutation
		for (int j = 0; j < this->nDims(); j++) {
			newShape.at(j) = this->shapeAlong(permutation.at(j));
		}

		// Swap places of the data w.r.t the permutation
		for (int i = 0; i < newData.size(); i++) {
			auto indices = reconstructIndex(i);
			std::vector<int> temp = std::vector<int>(this->nDims(), 0);
			for (int j = 0; j < temp.size(); j++) {
				temp.at(j) = indices.at(permutation.at(j));
			}
			newData.at(flattenIndex(temp, newShape)) = m_data.at(i);
		}

		m_data = newData;  m_shape = newShape;
		return *this;
	} 

	DynamicArray<T>& concatenate(DynamicArray<T>&& arr, int axis) {
		return this->join(std::move(arr), axis, -1);
	}
	DynamicArray<T>& concatenate(DynamicArray<T>& arr, int axis) {
		return this->join(std::move(arr), axis, -1);
	}
	
	template<typename iter>
	DynamicArray<T>& insert(iter it, T value)
	{
		assert(this->nDims() == 1);
		m_data.insert(it, value);
		m_shape[this->getDominantAxis_1d()]++;
		return *this;

	}
	DynamicArray<T>& insert(DynamicArray<T>& arr, int axis, int offset) {
		return this->join(std::move(arr), axis, offset);
	}
	DynamicArray<T>& insert(DynamicArray<T>&& arr, int axis, int offset) {
		return this->join(arr, axis, offset);
	}
	
	DynamicArray<T>& reverse(int axis) 
	{
		assert(this->nDims() > axis);

		auto index = this->reconstructIndex(0);
		for (int i = 0; i < this->getNonAxisNumberOfElements(axis); i++) {
			auto nonAxisIndex = getNonAxisIndex(index, axis);
			DynamicArray<T> extracted = extract(axis, nonAxisIndex);
			std::reverse(extracted.begin(), extracted.end());
			this->replaceAlong(extracted, axis, nonAxisIndex);
			this->incrementExtractionIndex(index, axis, this->nDims() - 1);
		}
		return *this;
	}
	DynamicArray<T>& roll(int shift, int axis)
	{
		assert(this->nDims() > axis); 

		auto index = this->reconstructIndex(0);
		for (int i = 0; i < this->getNonAxisNumberOfElements(axis); i++) {
			auto nonAxisIndex = getNonAxisIndex(index, axis);
			DynamicArray<T> extracted = extract(axis, nonAxisIndex);
			if (shift > 0)
				std::rotate(extracted.rbegin(), extracted.rbegin() + shift, extracted.rend());
			else
				std::rotate(extracted.begin(), extracted.begin() - shift, extracted.end());
			this->replaceAlong(extracted, axis, nonAxisIndex);
			this->incrementExtractionIndex(index, axis, this->nDims() - 1);
		}
		return *this;
	}

	DynamicArray<T>& rotate(Cnum::Rotation::Axis axisOfRotation, Cnum::Rotation::Degrees theta) {
		switch (axisOfRotation) {
		case (Cnum::Rotation::Axis::X):
			return this->rotate({ 1, 0, 0 }, Cnum::Rotation::toRadians(theta));
		case (Cnum::Rotation::Axis::Y):
			return this->rotate({ 0, 1, 0 }, Cnum::Rotation::toRadians(theta));
		case (Cnum::Rotation::Axis::Z):
			return this->rotate({ 0, 0, 1 }, Cnum::Rotation::toRadians(theta));
		}
	}
	DynamicArray<T>& rotate(DynamicArray<T>&& axisOfRotation, Cnum::Rotation::Degrees theta) {
		return this->rotate(axisOfRotation, Cnum::Rotation::toRadians(theta));
	}
	DynamicArray<T>& rotate(Cnum::Rotation::Axis axisOfRotation, Cnum::Rotation::Radians theta) {
		switch (axisOfRotation) {
		case (Cnum::Rotation::Axis::X):
			return this->rotate({ 1, 0, 0 }, theta);
		case (Cnum::Rotation::Axis::Y):
			return this->rotate({ 0, 1, 0 }, theta);
		case (Cnum::Rotation::Axis::Z):
			return this->rotate({ 0, 0, 1 }, theta);
		}
	}
	DynamicArray<T>& rotate(DynamicArray<T>&& axisOfRotation, Cnum::Rotation::Radians theta) {
		assert(this->size() == 3);
		assert(this->nDims() == 1);
		assert(axisOfRotation.size() == 3);
		assert(axisOfRotation.nDims() == 1);

		axisOfRotation.normalize();
		Cnum::Quaternion<T> q1(std::cos(theta / 2), axisOfRotation * std::sin(theta / 2));
		*this = (q1 * (*this) * q1.inverse()).to3D().normalize();
		return *this;
	}

	DynamicArray<T>& blend_if(DynamicArray<T>&& arr, DynamicArray<bool>&& condition) {
		return this->blend_if(arr, condition);
	}
	DynamicArray<T>& blend_if(DynamicArray<T>& arr, DynamicArray<bool>& condition) {
		
		assert(this->sameShapeAs(arr)); 
		assert(this->sameShapeAs(condition));

		for (int i = 0; i < condition.size(); i++) {
			if (condition.at(i) == true) {
				m_data.at(i) = arr[i];
			}
		}
		return *this;	
	}
	DynamicArray<T>& blend_if(DynamicArray<T>&& arr, std::function<bool(T)>&& condition) {
		return blend_if(arr, std::move(condition));
	}
	DynamicArray<T>& blend_if(DynamicArray<T>& arr, std::function<bool(T)>&& condition) {
		
		assert(this->sameShapeAs(arr));

		for (int i = 0; i < arr.size(); i++) {
			if (condition(this->m_data.at(i)) == true) {
				this->m_data.at(i) = arr.m_data.at(i);
			}
		}
		return *this;
	}

	DynamicArray<T>& replace_if(T replacement, std::function<bool(T)>&& condition) {
		return blend_if(DynamicArray(this->shape(), replacement), std::move(condition));
	}

	DynamicArray<T>& erase(int index)
	{
		assert(this->nDims() == 1); 
		m_data.erase(m_data.begin() + index);
		m_shape[getDominantAxis_1d()]--;
		return *this;
	}

	void append(const T value, int axis = 1) {

		if (m_shape.empty()) {
			m_data.push_back(value);
			m_shape = std::vector<int>(std::max(2, axis + 1), 1);
			return;
		}
		assert(this->nDims() == 1); 
		m_data.push_back(value);
		m_shape.at(axis)++; 
	}

	// Reductions
	template<typename Operation>
	T reduce(T initVal, Operation op)const {
		return std::accumulate(m_data.begin(), m_data.end(), initVal, op);
	}
	T norm() {
		auto copy = *this;
		return (T)std::pow(copy.raiseTo(2).reduce(0, std::plus<>()), 0.5);
	}

	template<typename Operation>
	DynamicArray<T> reduceAlongAxis(int axis, Operation op)const
	{
		assert(this->nDims() == axis);

		// The axis which the sum is along gets reduced to 1
		std::vector<int> returnShape = this->shape();
		returnShape.at(axis) = 1;
		DynamicArray<T> returnArray(returnShape, 0);

		for (int i = 0; i < getNumberOfElements(returnShape); i++) {
			std::vector<int> nonAxisIndex = getNonAxisIndex(i, axis);
			returnArray[i] = this->extract(axis, nonAxisIndex).reduce(0, op);
		}
		return returnArray;
	}

	// Extractions
	DynamicArray<T> extract(int start, int end)const {
		assert(this->nDims() == 1); 
		DynamicArray<T> out; 
		end = (end < 0) ? m_shape.at(this->getDominantAxis_1d()) + end + 1: end;
		for (int i = start; i < end; i++) {
			out.append(m_data.at(i)); 
		}
		return out;
	}
	DynamicArray<T> extract(int axis, int nonAxisIndex, int start, int end = -1)const 
	{
		// No need for exception here since they are checked in extract_if()
		return this->extract_if(axis, std::vector<int>(1, nonAxisIndex), [](T t) {return true; }, start, end);
	}
	DynamicArray<T> extract(int axis, std::vector<int>& nonAxisIndex, int start=0, int end=-1)const
	{
		// No need for exception here since they are checked in extract_if()
		return this->extract_if(axis, nonAxisIndex, [](T t) {return true; }, start, end);
	}	
	DynamicArray<T> extract_if(int axis, const iArrayLike_1d auto& nonAxisIndex, std::function<bool(T)>&& pred, int start=0, int end=-1)const
	{
		assert(this->nDims() > axis); 
		assert(nonAxisIndex.size() == this->nDims() - 1);

		auto start_stop = determineStartEndIndexForAxis(axis, nonAxisIndex, start, end);
		int stride = getStride(axis);
		DynamicArray<T> out;
		for (int i = start_stop.first; i < start_stop.second; i += stride) {
			if (pred(m_data.at(i)))
				out.append(m_data.at(i), axis);
		}
		return out;
	}

	DynamicArray<T> adjacentDiff(bool forwardDiff = true)
	{
		assert(this->nDims() == 1); 

		DynamicArray<T> out(this->size(), 0);
		if (forwardDiff)
			std::adjacent_difference(m_data.begin(), m_data.end(), out.begin(), [](int a, int b) {return a - b; });
		else
			std::adjacent_difference(m_data.begin(), m_data.end(), out.begin(), [](int a, int b) {return b - a; });

		out.erase(0);
		return out;		
	}
	DynamicArray<T> adjacentDiff(int axis, bool forwardDiff = true)
	{
		//assert(this->nDims() > 1); 
		assert(this->nDims() > axis); 

		DynamicArray<T> out;
		DynamicArray<int> index = this->reconstructIndex(0);

		for (int i = 0; i < this->getNonAxisNumberOfElements(axis); i++) {
			auto nonAxisIndex = getNonAxisIndex(index, axis);
			DynamicArray<T> extracted = extract(axis, nonAxisIndex);

			if (forwardDiff)
				std::adjacent_difference(extracted.begin(), extracted.end(), extracted.begin(), [](int a, int b) {return a - b; });
			else
				std::adjacent_difference(extracted.begin(), extracted.end(), extracted.begin(), [](int a, int b) {return b - a; });

			extracted.erase(0);
			out.concatenate(extracted, axis);
			this->incrementExtractionIndex(index, axis, this->nDims() - 1);
		}
		auto newShape = this->shape();
		newShape[axis]--;
		out.reshape(newShape);
		return out;
	}

	// Searching
	DynamicArray<int> find(DynamicArray<bool>&& condition)
	{
		assert(this->sameShapeAs(condition));
		int axis = (this->nDims() > 1) ? 0 : 1;
		DynamicArray<int> outIndices;
		for (int i = 0; i < condition.size(); i++) {
			if (condition.raw()[i] == true) {
				auto idx = this->reconstructIndex(i);
				outIndices.concatenate(idx, axis);
			}
		}
		return outIndices;
	}
	DynamicArray<int> find_if(std::function<bool(T)>&& pred)
	{
		int axis = (this->nDims() > 1) ? 0 : 1;
		DynamicArray<int> outIndices;
		for (int i = 0; i < this->size(); i++) {
			if (pred(m_data.at(i))) {
				auto idx = this->reconstructIndex(i);
				outIndices.concatenate(idx, axis);
			}
		}
		return outIndices;
	}

	// Sorting
	DynamicArray<int> argsort() 
	{
		assert(this->nDims() == 1); 
		DynamicArray<int> out(this->shape(), 0);
		std::iota(out.begin(), out.end(), 0);
		std::stable_sort(out.begin(), out.end(), [&](int i1, int i2) { return m_data.at(i1) < m_data.at(i2);  });
		return out;
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

		assert(this->nDims() > 1); 
		assert(this->nDims() > axis);

		DynamicArray<int> axisIdx({ this->shapeAlong(axis) }, 0);
		DynamicArray<int> out(this->shape(), 0);

		auto index = this->reconstructIndex(0);
		for (int i = 0; i < this->getNonAxisNumberOfElements(axis); i++) {
			auto nonAxisIndex = getNonAxisIndex(index, axis);
			DynamicArray<T> data = this->extract(axis, nonAxisIndex);
			std::iota(axisIdx.begin(), axisIdx.end(), 0);
			std::stable_sort(axisIdx.begin(), axisIdx.end(), [&](int i1, int i2) { return data[i1] < data[i2];  });
			out.replaceAlong(axisIdx, axis, nonAxisIndex);
			this->incrementExtractionIndex(index, axis, this->nDims() - 1);
		}
		return out;
	}
	DynamicArray<T>& sortFlat()
	{
		std::sort(this->begin(), this->end()); 
		this->flatten(); 
		return *this;
	}
	DynamicArray<T>& sort()
	{
		assert(this->nDims() == 1); 
		std::sort(this->begin(), this->end());
		return *this;
	}
	DynamicArray<T>& sort(int axis) 
	{
		assert(this->nDims() > 1);
		assert(this->nDims() > axis);

		auto index = this->reconstructIndex(0);
		for (int i = 0; i < this->getNonAxisNumberOfElements(axis); i++) {
			auto nonAxisIndex = getNonAxisIndex(index, axis);
			DynamicArray<T> sortedAxis = this->extract(axis, nonAxisIndex).sortFlat();
			this->replaceAlong(sortedAxis, axis, nonAxisIndex);
			this->incrementExtractionIndex(index, axis, this->nDims() - 1);
		}
		return *this;
	}

	// statics
	static DynamicArray<int> arange(const int start, const int end)
	{
		m_data = std::vector<T>(end - start);
		m_shape = std::vector<int>{ 1, end - start };
		std::iota(m_data.begin(), m_data.end(), start);
	}

	// Boolean checks	
	template<typename S>
	bool sameShapeAs(const DynamicArray<S>& other)const {
		return std::equal(m_shape.begin(), m_shape.end(), other.shape().begin());
	}
	bool isEqualTo(const DynamicArray<T>& other)const {
		return std::equal(m_data.begin(), m_data.end(), other.m_data.begin());
	}
	bool isPermutation(const DynamicArray<T>& other)const {
		return std::is_permutation(this->begin(), this->end(), other.begin(), other.end());
	}

	// Prints
	void print()const {
		std::cout << "Cnum::Array(";
		if (this->size() > 0) {
			DynamicArray<int> startIndex(this->nDims(), 0);
			printDim(startIndex, 0);
		}
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
		if (axis < 0) {
			return m_shape.at(this->shape().size() - axis);
		}
		return m_shape.at(axis);
	}
	int shapeAlong(int axis) {
		if (axis < 0) {
			return m_shape.at(this->shape().size() - axis);
		}
		return m_shape.at(axis);
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
	size_t size()const 
	{
		return m_data.size();
	};
	int getStride(int axis = 0)const
	{
		/*
			What is a stride?
				- How far you have to move in the 1d vector to get to the next element along the specified axis
		*/

		assert(this->nDims() > axis);
		if (this->nDims() == 1)
			return 1; 
		else
			return std::accumulate(m_shape.begin() + 1 + axis, m_shape.end(), 1, std::multiplies<>());
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
	auto rbegin()const
	{
		return m_data.rbegin();
	}
	auto rbegin()
	{
		return m_data.rbegin();
	}
	auto rend()const
	{
		return m_data.rend();
	}
	auto rend()
	{
		return m_data.rend();
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

	// Indexing
	int flattenIndex(const iArrayLike_1d auto& index)const
	{
		return this->flattenIndex(index, this->shape());
	}
	int flattenIndex(const iArrayLike_1d auto& index, std::vector<int> shape)const
	{
		/*
		General Formula:
			The 1D index is generalized into
			x_1 * (nx_2 * ... nx_n - 1) + ... x_n-1 * nx_n-1  + x_n

			where x_1 ... x_n are the indices specified by the user, and nx_1 ... nx_n are the dimension-size for the n-th dimension

			Example: 4D index (1,2,1,0) where the shape is (4,4,4,4).

			1 * 4*4*4 + 2 * 4*4 + 1 * 4 + 0	= 64 + 32 + 4 = 100
		*/

		int flatIndex = 0;

		for (int j = 0; j < index.size() - 1; j++)
			flatIndex += index.at(j) * std::accumulate(shape.begin() + j + 1, shape.end(), 1, std::multiplies<int>());

		flatIndex += index.at((int)index.size() - 1);
		return flatIndex;
	}

	std::vector<int> getNonAxisIndex(int flatIndex, int axis)const {

		auto index = reconstructIndex(flatIndex).raw();
		index.erase(index.begin() + axis);
		return index;

	}
	std::vector<int> getNonAxisIndex(DynamicArray<int>& index, int axis)const {
		auto idx = index.raw();
		idx.erase(idx.begin() + axis);
		return idx;

	}
	DynamicArray<int> reconstructIndex(int index)const 
	{
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

	// Element counts
	int getNumberOfElements()const
	{
		return getNumberOfElements(this->shape());
	}
	int getNonAxisNumberOfElements(int axis)const {
		std::vector<int> shape = this->shape(); 
		shape.erase(shape.begin() + axis); 
		return getNumberOfElements(shape); 
	}
	static int getNumberOfElements(const iArrayLike_1d auto& shape)
	{
		return std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int>());
	}

	// Actions
	template<typename Operation>
	static DynamicArray<T> binaryOperation(const DynamicArray<T>& arr1, const DynamicArray<T>& arr2, Operation binaryOp)
	{
		DynamicArray<T> out(arr1.size());
		std::transform(arr1.begin(), arr1.raw().end(), arr2.begin(), out.begin(), binaryOp);
		return out.reshape(arr1.shape());
	}
	template<typename Operation>
	static DynamicArray<T> unaryOperation(const DynamicArray<T>& arr, Operation unaryOp)
	{
		DynamicArray<T> out(arr.size());
		std::transform(arr.begin(), arr.raw().end(), out.begin(), unaryOp);
		return out.reshape(arr.shape());
	}
	template<typename Operation>
	DynamicArray<T>& unaryOperation(DynamicArray<T>& arr, Operation unaryOp)
	{
		std::transform(arr.begin(), arr.end(), arr.begin(), unaryOp);
		return arr;
	}
	void incrementExtractionIndex(DynamicArray<int>& index, int axis, int dim) {

		if (dim == -1)
			return;

		if (index.at(dim) + 1 > this->shapeAlong(dim) - 1 || dim == axis) {
			index.at(dim) = 0;
			incrementExtractionIndex(index, axis, dim - 1);
		}
		else {
			index.at(dim)++;
		}
	}
	void printDim(DynamicArray<int>& index, int dim)const
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
		// For the higher ranks, recursively call upwards in rank
		std::cout << "[";
		for (int i = 0; i < m_shape[dim]; i++) {
			index[dim] = i; 
			if (index[dim] != 0) { std::cout << "\t"; PrintSpaces(5 + dim); }
			printDim(index, dim + 1);
			if (i == m_shape[dim] - 1) {
				std::cout << "]";
				if (dim > 0 && index[dim - 1] != m_shape[dim - 1] - 1) { std::cout << "," << std::endl; }
			}
			else
				std::cout << std::endl;
		}
	}
	DynamicArray<T>& join(DynamicArray<T>&& arr, int axis, int offset) {

		// If the array is uninitialized i.e. empty, the join will simply act as assignment
		if (m_data.empty()) {
			*this = arr;
			return *this;
		}

		int stride = this->getStride(axis);

		// If the axis is 0 then the data can be put at the end of the array
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
				for (int k = 0; k < arr.shapeAlong(axis); k++) {
					m_data.insert(this->begin() + i + k, arr.at(arr.reconstructIndex(j)));
					j++;
				}
			}
		}
		return *this;
	}
	DynamicArray<T>& replaceAlong(DynamicArray<T>& newData, int axis, DynamicArray<int>&& nonAxisIndices) {
		return this->replaceAlong(std::move(newData), axis, std::move(nonAxisIndices));
	}
	DynamicArray<T>& replaceAlong(DynamicArray<T>&& newData, int axis, DynamicArray<int>&& nonAxisIndices) {

		//assert(nonAxisIndices.size() == this->nDims() - 1);
		//assert(newData.size() == this->shapeAlong(axis));

		int stride = getStride(axis);
		auto start_stop = determineStartEndIndexForAxis(axis, nonAxisIndices, 0, -1);
		int j = 0;
		for (int i = start_stop.first; i <= start_stop.second; i += stride) {
			m_data.at(i) = newData[j];
			j++;
		}
		return *this;
	}

	// Creators
	static DynamicArray<bool> createLogicalArray(const DynamicArray<T>& arr1, const DynamicArray<T>&& arr2, std::function<bool(T, T)> func)
	{
		return createLogicalArray(arr1, arr2, func);
	}
	static DynamicArray<bool> createLogicalArray(const DynamicArray<T>& arr1, const DynamicArray<T>& arr2, std::function<bool(T,T)> func)
	{
		DynamicArray<bool> out(arr1.size());
		std::transform(arr1.begin(), arr1.end(), arr2.begin(), out.begin(), func);
		return out.reshape(arr1.shape());
	}
	static DynamicArray<bool> createLogicalArray(const DynamicArray<T>& arr, T value, std::function<bool(T)> func)
	{
		DynamicArray<bool> out(arr.size()); 
		std::transform(arr.begin(), arr.end(), out.begin(), func);
		return out.reshape(arr.shape());
	}
	
	// Misc
	int getDominantAxis_1d()const
	{
		assert(nDims() == 1);
		return (int)(std::max_element(m_shape.begin(), m_shape.end()) - m_shape.begin());
	}
	std::pair<int, int> determineStartEndIndexForAxis(int axis, const DynamicArray<int>& nonAxisIndex, int start, int end)const
	{ 
		if (this->nDims() == 1) {
			end = (end < 0) ? m_shape.at(this->getDominantAxis_1d()) + end + 1 : end;
			return std::make_pair(start, end);
		}

		auto startIndex = nonAxisIndex;
		startIndex.insert(startIndex.begin()+axis, start);

		auto endIndex = startIndex;
		endIndex.at(axis) = (end < 0) ? m_shape.at(axis) + end : end;

		return std::make_pair(flattenIndex(startIndex), flattenIndex(endIndex)); 
	}
	void swap(DynamicArray<T>& arr1, DynamicArray<T>& arr2)
	{
		std::swap(arr1.m_data, arr2.m_data); 
		std::swap(arr1.m_shape, arr2.m_shape);
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

}