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
#include "Quaternion.h"

namespace Cnum
{

template<typename T>
class ndArray
{
public:

	//--------------------------
	// Constructors
	// -------------------------

	ndArray() = default;

	ndArray(const ArrayLike_1d auto& init)
	{
		std::copy(init.begin(), init.end(), std::back_inserter(m_data));
		m_shape = std::vector<int>{ 1, (int)init.size() };
	}
	ndArray(const ArrayLike_1d auto& init, const iArrayLike_1d auto& shape)
	{
		try {
			std::copy(init.begin(), init.end(), std::back_inserter(m_data));
			std::copy(shape.begin(), shape.end(), std::back_inserter(m_shape));
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
			exit(0);
		}
	}
	ndArray(const iArrayLike_1d auto& shape, T initialValue)
	{
		std::copy(shape.begin(), shape.end(), std::back_inserter(m_shape));
		if (m_shape.size() == 1) {
			m_shape = std::vector<int>{ 1, m_shape[0] };
		}

		m_data = std::vector<T>(this->getNumberOfElements(), initialValue);
	}

	// Creation by initializer list
	ndArray(const std::initializer_list<T>& init)
		: m_data{std::vector(init)}, m_shape{std::vector<int>{1, (int)init.size()}}
	{}
	ndArray(const std::initializer_list<T>& init, const std::initializer_list<int>& shape)
		: m_data{std::vector(init)}, m_shape{std::vector(shape)}
	{}
	ndArray(const std::initializer_list<int>& shape, T initialValue)
		: m_data{std::vector(getNumberOfElements(std::vector(shape)), initialValue )}, m_shape{std::vector(shape)}
	{};

	// Creation by size
	ndArray(const size_t size)
		: m_shape{ std::vector{1, (int)size} }, m_data{std::vector<T>(size)}
	{}

	
	ndArray(const size_t size, const T initialValue)
		: m_shape{ std::vector<int>{1, (int)size} }, m_data{ std::vector<T>(size, initialValue) }
	{}

	// Copy Constructor
	ndArray(const ndArray<T>& src) = default;

	// Move Constructor
	ndArray(ndArray&& other) noexcept
		: ndArray()
	{
		swap(*this, other);
	}

	// Destructor
	~ndArray() = default;

	//--------------------------
	// Operators
	// -------------------------

	T& operator[](const iArray& index)const
	{
		assert(this->nDims() == index.size());
		return (T&)m_data.at(flattenIndex(ndArray{ index }));
 	}
	T& operator[](int index)const
	{
		assert(this->nDims() == 1);
		return (T&)m_data.at(index);
	}
	const ndArray<T> operator[](iArray&& logicalIndices)const {

		assert(this->sameShapeAs(logicalIndices));

		ndArray<T> data;
		logicalIndices.flatten();
		for (int i = 0; i < logicalIndices.size(); i++) {
			if (logicalIndices.at(i))
				data.append(m_data[i]);
		}
		return data;


	}
	const ndArray<T> operator[](iArray& logicalIndices)const {
		
		assert(this->sameShapeAs(logicalIndices)); 
		ndArray<T> data;
		for (int i = 0; i < logicalIndices.size(); i++) {
			if (logicalIndices.at(i))
				data.append(m_data[i]);
		}
		return data;
		
	}
	T& at(int index)
	{
		assert(this->nDims() == 1); 
		return (T&)m_data.at(index);
	}
	T& at(const iArray& index) 
	{
		assert(this->nDims() == index.size()); 
		return (T&)m_data.at(flattenIndex(index));
	}
	const T& at(int index)const
	{
		assert(this->nDims() == 1);
		return (T&)m_data.at(index);
	}
	const T& at(const iArray& index)const
	{
		assert(this->nDims() == index.size());
		return (T&)m_data.at(flattenIndex(index));
	}

	// Assignment
	ndArray<T>& operator=(ndArray<T> other) 
	{
		// Copy and swap idiom. Copy is made in the parameter list
		swap(*this, other); 
		return *this;
	}

	// Addition
	friend ndArray<T> operator+(const ndArray& lhs, const ndArray& rhs)
	{
		assert(lhs.sameShapeAs(rhs)); 
		return binaryOperation(lhs, rhs, std::plus<>());
	}
	friend ndArray<T> operator+(const ndArray&& lhs, const ndArray&& rhs)
	{
		return lhs + rhs;
	}
	friend ndArray<T> operator+(const ndArray& lhs, const T value)
	{
		return lhs + ndArray<T>(lhs.shape(), value);
	}
	void operator+=(const ndArray& rhs)
	{
		*this = *this + rhs;
	}
	void operator+=(const ndArray&& rhs)
	{
		*this = *this + rhs;
	}
	void operator+=(const T value) {
		*this = *this + value;
	}

	// Subtraction
	friend ndArray<T> operator-(const ndArray& lhs, const ndArray& rhs)
	{
		assert(lhs.sameShapeAs(rhs));
		return binaryOperation(lhs, rhs, std::minus<>());
	}
	friend ndArray<T> operator-(const ndArray&& lhs, const ndArray&& rhs)
	{
		return lhs - rhs;
	}
	friend ndArray<T> operator-(const ndArray& lhs, const T value)
	{
		return lhs - ndArray<T>(lhs.shape(), value);
	}
	void operator-=(const ndArray& rhs)
	{
		*this = *this - rhs;
	}
	void operator-=(const ndArray&& rhs)
	{
		*this = *this - rhs;
	}
	void operator-=(const T value) {
		*this = *this - value;
	}
	ndArray<T> operator-()
	{
		return unaryOperation(*this, [](T t) {return t *= -1; }); 
	}

	// Multiplication
	friend ndArray<T> operator*(const ndArray& lhs, const ndArray& rhs)
	{
		assert(lhs.sameShapeAs(rhs));
		return binaryOperation(lhs, rhs, std::multiplies<>());
	}
	friend ndArray<T> operator*(const ndArray&& lhs, const ndArray&& rhs)
	{
		return lhs * rhs;
	}
	friend ndArray<T> operator*(const T value, const ndArray& rhs)
	{
		return rhs * ndArray<T>(rhs.shape(), value);
	}
	friend ndArray<T> operator*(const ndArray& lhs, const T value)
	{
		return value * lhs;
	}
	void operator*=(const ndArray& rhs)
	{
		*this = *this * rhs;
	}
	void operator*=(const ndArray&& rhs)
	{
		*this = *this * rhs;
	}
	void operator*=(const T value) {
		*this = *this * value;
	}

	// Division
	friend ndArray<T> operator/(const ndArray& lhs, const ndArray& rhs)
	{
		assert(lhs.sameShapeAs(rhs));
		assert(rhs[rhs == 0].size() == 0); 
		return binaryOperation(lhs, rhs, std::divides<>());
	}
	friend ndArray<T> operator/(const ndArray&& lhs, const ndArray&& rhs)
	{
		return lhs / rhs;
	}
	friend ndArray<T> operator/(const ndArray& rhs, const T value)
	{
		return rhs / ndArray<T>(rhs.shape(), value);;
	}
	void operator/=(const ndArray& rhs)
	{
		*this = *this / rhs;
	}
	void operator/=(const ndArray&& rhs)
	{
		*this = *this / rhs;
	}
	void operator/=(const T value) {
		*this = *this / value;
	}

	// Equailty
	iArray operator==(const ndArray<T>& rhs)const
	{
		return *this == std::move(rhs);
	}
	iArray operator==(const ndArray<T>&& rhs)const
	{
		assert(this->sameShapeAs(rhs)); 
		return createLogicalArray(*this, std::move(rhs), std::equal_to<>());
	}
	iArray operator==(const T value)const {
		return createLogicalArray(*this, ndArray<T>(this->shape(), value), std::equal_to<>());
	}
				 
	// Anti-Equality
	iArray operator!=(const ndArray<T>& rhs)const
	{
		return *this != std::move(rhs);
	}
	iArray operator!=(const ndArray<T>&& rhs)const
	{
		
		return createLogicalArray(*this, std::move(rhs), std::not_equal_to<>());
	}
	iArray operator!=(const T value)const {
		return createLogicalArray(*this, ndArray<T>(this->shape(), value), std::not_equal_to<>());
	}
				 
	// Less than 
	iArray operator < (T value)const {
		return *this < ndArray<T>(this->shape(), value);
	}
	iArray operator < (const ndArray<T>& rhs)const
	{
		return *this < std::move(rhs);
	}
	iArray operator < (const ndArray<T>&& rhs)const
	{
		assert(this->sameShapeAs(rhs));
		return createLogicalArray(*this, std::move(rhs), std::less<>());
	}
				 
	// Larger than
	iArray operator > (T value)const {
		return *this > ndArray<T>(this->shape(), value); 
	}
	iArray operator > (const ndArray<T>& rhs)const
	{
		return *this > std::move(rhs);
	}
	iArray operator > (const ndArray<T>&& rhs)const
	{
		assert(this->sameShapeAs(rhs));
		return createLogicalArray(*this, std::move(rhs), std::greater<>());
	}

	// Less or equal than
	iArray operator <= (T value)const {
		return *this <= ndArray<T>(this->shape(), value);
	}
	iArray operator <= (const ndArray<T>& rhs)const
	{
		return *this <= std::move(rhs);
	}
	iArray operator <= (const ndArray<T>&& rhs)const
	{
		assert(this->sameShapeAs(rhs));
		return createLogicalArray(*this, std::move(rhs), std::less_equal<>());
	}

	// Larger or equal than
	iArray operator >= (T value)const {
		return *this >= ndArray<T>(this->shape(), value);
	}
	iArray operator >= (const ndArray<T>& rhs)const
	{
		return *this >= std::move(rhs);
	}
	iArray operator >= (const ndArray<T>&& rhs)const
	{
		assert(this->sameShapeAs(rhs));
		return createLogicalArray(*this, std::move(rhs), std::greater_equal<>());
	}

	// Logical AND operator
	iArray operator && (const iArray& rhs)const
	{
		return *this && std::move(rhs);
	}
	iArray operator && (const iArray&& rhs)const
	{
		assert(this->sameShapeAs(rhs));
		return createLogicalArray(*this, rhs, std::logical_and<>());
	}

	// Logical OR operator
	iArray operator || (const iArray& rhs)const
	{
		return *this || std::move(rhs);
	}
	iArray operator || (const iArray&& rhs)const
	{
		assert(this->sameShapeAs(rhs));
		return createLogicalArray(*this, rhs, std::logical_or<>());
	}

	// Streaming
	friend std::ostream& operator << (std::ostream& stream, const ndArray<T>& arr) {
		return stream << std::move(arr);
	}
	friend std::ostream& operator << (std::ostream& stream, const ndArray<T>&& arr) {
		arr.print();
		return stream;
	}
	friend std::istream& operator >> (std::istream& stream, ndArray<T>& arr)
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
	ndArray<T>& flatten()
	{
		m_shape = { 1, (int)this->size() };
		return *this;
	} 
	ndArray<T>& abs()
	{
		std::transform(this->begin(), this->end(), this->begin(), [](T e) {return std::abs(e); });
		return *this;
	}
	ndArray<T>& reshape(iArray&& newShape)
	{
		assert(this->size() == newShape.reduce(1, std::multiplies<>()));
		m_shape.clear();
		std::copy(newShape.begin(), newShape.end(), std::back_inserter(m_shape));
		return *this;	
	}
	ndArray<T>& reshape(iArray& newShape) {
		return this->reshape(std::move(newShape));
	}

	ndArray<T>& normalize()
	{
		*this /= this->norm();
		return *this;
	}
	ndArray<T>& raiseTo(T exponent)
	{
		return unaryOperation(*this, [exponent](T value) {return std::pow(value, exponent); });
	}
	ndArray<T>& round(size_t nDecimals) {
		auto factor = std::pow(10, nDecimals);
		*this *= factor;
		unaryOperation(*this, [](T val) {return std::round(val); });
		*this /= factor;
		return *this;
	}

	ndArray<T>& transpose() {

		if (this->nDims() == 1) {
			std::reverse(m_shape.begin(), m_shape.end()); 
			return *this; 
		}
		std::vector<int> permutation = std::vector<int>(this->nDims());
		std::iota(permutation.begin(), permutation.end(), 0);
		std::reverse(permutation.begin(), permutation.end());
		return this->transpose(permutation);

	}
	ndArray<T>& transpose(const ndArray<int>&& permutation)
	{
		/*
			What is the meaning of a permutation in this context? 
				A permutation, same size as the shape, tells which axis should move to where. 
				If the original shape is (2,3,5) and the permutation is (0, 2, 1), then the new shape is (2, 5, 3)
				
				The default permutation is to reverse the shape
		*/

		assert(permutation.isPermutation(Cnum::Array::arange(this->nDims())));
		assert(this->nDims() > 1);

		std::vector<T> newData = std::vector<T>(this->size(), 0);
		std::vector<int> newShape = std::vector<int>(this->nDims(), 0);

		// Update the shape based on the permutation
		for (int j = 0; j < this->nDims(); j++) {
			newShape.at(j) = this->shapeAlong(permutation.at(j));
		}

		// Swap places of the data w.r.t the permutation
		for (int i = 0; i < newData.size(); i++) {
			iArray normalIndex = reconstructIndex(i);
			iArray permutatedIndex(this->nDims(), 0);
			for (int j = 0; j < permutatedIndex.size(); j++) {
				permutatedIndex.at(j) = normalIndex.at(permutation.at(j));
			}
			newData.at(flattenIndex(permutatedIndex, newShape)) = m_data.at(i);
		}

		m_data = newData;  m_shape = newShape;
		return *this;
	} 

	ndArray<T>& concatenate(ndArray<T>&& arr, int axis) {
		return this->join(std::move(arr), axis, -1);
	}
	ndArray<T>& concatenate(ndArray<T>& arr, int axis) {
		return this->join(std::move(arr), axis, -1);
	}
	
	template<typename iter>
	ndArray<T>& insert(iter it, T value)
	{
		assert(this->nDims() == 1);
		m_data.insert(it, value);
		m_shape[this->getDominantAxis_1d()]++;
		return *this;

	}
	ndArray<T>& insert(ndArray<T>& arr, int axis, int offset) {
		return this->join(std::move(arr), axis, offset);
	}
	ndArray<T>& insert(ndArray<T>&& arr, int axis, int offset) {
		return this->join(arr, axis, offset);
	}
	
	ndArray<T>& reverse() {
		assert(this->nDims() == 1);
		std::reverse(m_data.begin(), m_data.end()); 
		return *this;
	}
	ndArray<T>& reverse(int axis) 
	{
		assert(this->nDims() > axis);

		auto index = this->reconstructIndex(0);
		for (int i = 0; i < this->getNonAxisNumberOfElements(axis); i++) {
			auto nonAxisIndex = getNonAxisIndex(index, axis);
			ndArray<T> extracted = extract(axis, nonAxisIndex);
			std::reverse(extracted.begin(), extracted.end());
			this->replaceAlong(extracted, axis, nonAxisIndex);
			this->incrementExtractionIndex(index, axis, this->nDims() - 1);
		}
		return *this;
	}
	ndArray<T>& roll(int shift, int axis)
	{
		assert(this->nDims() > axis); 

		auto index = this->reconstructIndex(0);
		for (int i = 0; i < this->getNonAxisNumberOfElements(axis); i++) {
			auto nonAxisIndex = getNonAxisIndex(index, axis);
			ndArray<T> extracted = extract(axis, nonAxisIndex);
			if (shift > 0)
				std::rotate(extracted.rbegin(), extracted.rbegin() + shift, extracted.rend());
			else
				std::rotate(extracted.begin(), extracted.begin() - shift, extracted.end());
			this->replaceAlong(extracted, axis, nonAxisIndex);
			this->incrementExtractionIndex(index, axis, this->nDims() - 1);
		}
		return *this;
	}

	ndArray<T>& rotate(Cnum::Rotation::Axis axisOfRotation, Cnum::Rotation::Degrees theta) {
		switch (axisOfRotation) {
		case (Cnum::Rotation::Axis::X):
			return this->rotate({ 1, 0, 0 }, Cnum::Rotation::toRadians(theta));
		case (Cnum::Rotation::Axis::Y):
			return this->rotate({ 0, 1, 0 }, Cnum::Rotation::toRadians(theta));
		case (Cnum::Rotation::Axis::Z):
			return this->rotate({ 0, 0, 1 }, Cnum::Rotation::toRadians(theta));
		default:
			return *this;
		}
	}
	ndArray<T>& rotate(const ndArray<T>&& axisOfRotation, Cnum::Rotation::Degrees theta) {
		return this->rotate(axisOfRotation, Cnum::Rotation::toRadians(theta));
	}
	ndArray<T>& rotate(Cnum::Rotation::Axis axisOfRotation, Cnum::Rotation::Radians theta) {
		switch (axisOfRotation) {
		case (Cnum::Rotation::Axis::X):
			return this->rotate({ 1, 0, 0 }, theta);
		case (Cnum::Rotation::Axis::Y):
			return this->rotate({ 0, 1, 0 }, theta);
		case (Cnum::Rotation::Axis::Z):
			return this->rotate({ 0, 0, 1 }, theta);
		}
	}
	ndArray<T>& rotate(ndArray<T>&& axisOfRotation, Cnum::Rotation::Radians theta) {
		assert(this->size() == 3);
		assert(this->nDims() == 1);
		assert(axisOfRotation.size() == 3);
		assert(axisOfRotation.nDims() == 1);

		axisOfRotation.normalize();
		Cnum::Quaternion<T> q1((T)std::cos(theta / 2), axisOfRotation * (T)std::sin(theta / 2));
		*this = (q1 * (*this) * q1.inverse()).to3D().normalize();
		return *this;
	}

	ndArray<T>& blend_if(const ndArray<T>&& arr, const iArray&& condition) {
		return this->blend_if(arr, condition);
	}
	ndArray<T>& blend_if(const ndArray<T>& arr, const iArray& condition) {
		
		assert(this->sameShapeAs(arr)); 
		assert(this->sameShapeAs(condition));

		for (int i = 0; i < condition.size(); i++) {
			if (condition.at(i)) {
				m_data.at(i) = arr[i];
			}
		}
		return *this;	
	}
	ndArray<T>& blend_if(const ndArray<T>&& arr, std::function<bool(T)>&& condition) {
		return blend_if(arr, std::move(condition));
	}
	ndArray<T>& blend_if(const ndArray<T>& arr, std::function<bool(T)>&& condition) {
		
		assert(this->sameShapeAs(arr));

		for (int i = 0; i < arr.size(); i++) {
			if (condition(this->m_data.at(i)) == true) {
				this->m_data.at(i) = arr.m_data.at(i);
			}
		}
		return *this;
	}

	ndArray<T>& replace_if(std::function<bool(T)>&& condition, T replacement) {
		return blend_if(ndArray(this->shape(), replacement), std::move(condition));
	}

	template<typename iter>
	ndArray<T>& erase(iter it)
	{
		assert(this->nDims() == 1);
		m_data.erase(it);
		m_shape[getDominantAxis_1d()]--;
		return *this;
	}
	ndArray<T>& erase(int index)
	{
		assert(this->nDims() == 1); 
		m_data.erase(m_data.begin() + index);
		m_shape[getDominantAxis_1d()]--;
		return *this;
	}
	ndArray<T>& erase(int start, int end)
	{
		assert(this->nDims() == 1);
		end = (end < 0) ? m_shape.at(this->getDominantAxis_1d()) + end + 1 : end;
		assert(end > start);
		for (; start < end; start++) {
			m_data.erase(m_data.begin() + start);
		}
		m_shape[getDominantAxis_1d()] -= end-start;
		return *this;
	}
	ndArray<T>& erase_if(const iArray&& condition) {
		assert(this->size() == condition.size());
		ndArray<T> out;
		for (int i = 0; i < this->size(); i++) {
			if (condition.raw()[i] == 0) {
				out.append(m_data[i]);
			}
		}
		*this = out;
		return *this;
	}
	ndArray<T>& erase_if(std::function<bool(T)>&& pred) {
		ndArray<T> out;
		for (int i = 0; i < this->size(); i++) {
			if (pred(m_data[i]) == false) {
				out.append(m_data[i]);
			}
		}
		*this = out;
		return *this;
	}
	

	void append(const T value) {

		if (m_data.empty()) {
			m_data.push_back(value);
			m_shape = std::vector<int>{ 1,1 };
			return;
		}
		assert(this->nDims() == 1); 
		if (m_data.size() == 1) {
			m_shape.at(1)++;
		}else {
			m_shape.at(this->getDominantAxis_1d())++;
		}
		m_data.push_back(value);
	}
	void append(const ndArray<T>& arr) {
		assert(arr.nDims() == 1);
		return this->concatenate(arr, 0);
	}

	// Reductions
	template<typename Operation>
	T reduce(T initVal, Operation op)const {
		return std::accumulate(m_data.begin(), m_data.end(), initVal, op);
	}
	T norm() {
		assert(this->nDims() == 1); 
		auto copy = *this;
		return (T)std::pow(copy.raiseTo(2).reduce(0, std::plus<>()), 0.5);
	}
	ndArray<T>& norm(int axis) {
		assert(this->nDims() > 1); 
		assert(this->nDims() > axis);
		int i;
		iArray index = this->reconstructIndex(0);
		for (i = 0; i < getNonAxisNumberOfElements(axis); i++) {
			iArray nonAxisIndex = getNonAxisIndex(index, axis);
			m_data.at(i) = this->extract(axis, nonAxisIndex).norm();
			this->incrementExtractionIndex(index, axis, this->nDims() - 1);
		}
		m_data = std::vector<T>(m_data.begin(), m_data.begin() + i);
		m_shape[axis] = 1;
		return *this;
	}

	template<typename Operation>
	ndArray<T>& reduceAlongAxis(int axis, T initValue, Operation op)
	{
		assert(this->nDims() > axis);
		int i;
		iArray index = this->reconstructIndex(0);
		for (i = 0; i < getNonAxisNumberOfElements(axis); i++) {
			iArray nonAxisIndex = getNonAxisIndex(index, axis);
			m_data.at(i) = this->extract(axis, nonAxisIndex).reduce(initValue, op);
			this->incrementExtractionIndex(index, axis, this->nDims() - 1);
		}
		m_data = std::vector<T>(m_data.begin(), m_data.begin() + i);
		m_shape[axis] = 1;
		return *this;
	}

	// Extractions
	ndArray<T> extract(int start, int end)const {
		assert(this->nDims() == 1); 
		ndArray<T> out; 
		end = (end < 0) ? m_shape.at(this->getDominantAxis_1d()) + end + 1: end;
		for (int i = start; i < end; i++) {
			out.append(m_data.at(i)); 
		}
		return out;
	}
	ndArray<T> extract(int axis, int nonAxisIndex, int start, int end = -1)const 
	{
		// No need for exception here since they are checked in extract_if()
		return this->extract_if(axis, std::vector<int>(1, nonAxisIndex), [](T t) {return true; }, start, end);
	}
	ndArray<T> extract(int axis, iArray& nonAxisIndex, int start=0, int end=-1)const
	{
		// No need for exception here since they are checked in extract_if()
		return this->extract_if(axis, nonAxisIndex, [](T t) {return true; }, start, end);
	}	
	ndArray<T> extract(int axis, iArray&& nonAxisIndex, int start = 0, int end = -1)const {
		return this->extract(axis, nonAxisIndex, start, end);
	}
	ndArray<T> extract_if(int axis, const iArray& nonAxisIndex, std::function<bool(T)>&& pred, int start=0, int end=-1)const
	{
		assert(this->nDims() > axis); 
		assert(nonAxisIndex.size() == this->nDims() - 1);

		auto start_stop = determineStartEndIndexForAxis(axis, nonAxisIndex, start, end);
		int stride = getStride(axis);
		ndArray<T> out;
		for (int i = start_stop.first; i < start_stop.second; i += stride) {
			if (pred(m_data.at(i)))
				out.append(m_data.at(i)); // Potential error. Append now defaults to row vector perhaps it needs to be flipped
		}
		return out;
	}

	ndArray<T>& adjacentDiff(bool forwardDiff = true)
	{
		assert(this->nDims() == 1); 
		if (forwardDiff)
			std::adjacent_difference(m_data.begin(), m_data.end(), m_data.begin(), [](int a, int b) {return a - b; });
		else
			std::adjacent_difference(m_data.begin(), m_data.end(), m_data.begin(), [](int a, int b) {return b - a; });

		this->erase(0);
		return *this;
	}
	ndArray<T>& adjacentDiff(int axis, bool forwardDiff = true)
	{
		assert(this->nDims() > 1); 
		assert(this->nDims() > axis); 

		iArray newShape = this->shape();
		newShape[axis]--;
		ndArray<T> out = Cnum::Array::uniformArray(newShape, 0);
		iArray index = this->reconstructIndex(0);

		for (int i = 0; i < this->getNonAxisNumberOfElements(axis); i++) {
			iArray nonAxisIndex = getNonAxisIndex(index, axis);
			ndArray<T> axisDiff = extract(axis, nonAxisIndex, 0).adjacentDiff(forwardDiff);
			out.replaceAlong(axisDiff, axis, nonAxisIndex);
			this->incrementExtractionIndex(index, axis, this->nDims() - 1);
		}
		*this = out;
		return *this; 
	}

	// Searching
	iArray find(iArray&& condition)
	{
		assert(this->sameShapeAs(condition));
		int axis = (this->nDims() > 1) ? 0 : 1;
		iArray outIndices;
		for (int i = 0; i < condition.size(); i++) {
			if (condition.m_data.at(i)) {
				outIndices.concatenate(this->reconstructIndex(i), axis);
			}
		}
		return outIndices;
	}
	iArray find_if(std::function<bool(T)>&& pred)
	{
		int axis = (this->nDims() > 1) ? 0 : 1;
		iArray outIndices;
		for (int i = 0; i < this->size(); i++) {
			if (pred(m_data.at(i))) {
				outIndices.concatenate(this->reconstructIndex(i), axis);
			}
		}
		return outIndices;
	}

	// Sorting
	iArray argsort() 
	{
		assert(this->nDims() == 1); 
		ndArray<int> out(this->shape(), 0);
		std::iota(out.begin(), out.end(), 0);
		std::stable_sort(out.begin(), out.end(), [&](int i1, int i2) { return m_data.at(i1) < m_data.at(i2);  });
		return out;
	}
	iArray argSort(int axis) {

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

		ndArray<int> axisIdx({ this->shapeAlong(axis) }, 0);
		ndArray<int> out(this->shape(), 0);

		auto index = this->reconstructIndex(0);
		for (int i = 0; i < this->getNonAxisNumberOfElements(axis); i++) {
			auto nonAxisIndex = getNonAxisIndex(index, axis);
			ndArray<T> data = this->extract(axis, nonAxisIndex);
			std::iota(axisIdx.begin(), axisIdx.end(), 0);
			std::stable_sort(axisIdx.begin(), axisIdx.end(), [&](int i1, int i2) { return data[i1] < data[i2];  });
			out.replaceAlong(axisIdx, axis, nonAxisIndex);
			this->incrementExtractionIndex(index, axis, this->nDims() - 1);
		}
		return out;
	}
	ndArray<T>& sortFlat()
	{
		std::sort(this->begin(), this->end()); 
		this->flatten(); 
		return *this;
	}
	ndArray<T>& sort()
	{
		assert(this->nDims() == 1); 
		std::sort(this->begin(), this->end());
		return *this;
	}
	ndArray<T>& sort(int axis) 
	{
		assert(this->nDims() > 1);
		assert(this->nDims() > axis);

		auto index = this->reconstructIndex(0);
		for (int i = 0; i < this->getNonAxisNumberOfElements(axis); i++) {
			auto nonAxisIndex = getNonAxisIndex(index, axis);
			ndArray<T> sortedAxis = this->extract(axis, nonAxisIndex).sortFlat();
			this->replaceAlong(sortedAxis, axis, nonAxisIndex);
			this->incrementExtractionIndex(index, axis, this->nDims() - 1);
		}
		return *this;
	}

	// Boolean checks	
	template<typename S>
	bool sameShapeAs(const ndArray<S>& other)const {
		return std::equal(m_shape.begin(), m_shape.end(), other.shape().begin());
	}
	bool isEqualTo(const ndArray<T>& other, size_t tol=4)const 
	{
		ndArray<T> arr1 = Cnum::Array::round(*this, tol); 
		ndArray<T> arr2 = Cnum::Array::round(other, tol);
		auto pred = [tol](T first, T second) {return std::abs(first - second) < std::pow(10, -(int)tol); };
		return std::equal(arr1.begin(), arr1.end(), arr2.begin(), arr2.end(), pred);
	}
	bool isPermutation(const ndArray<T>& other)const {
		return std::is_permutation(this->begin(), this->end(), other.begin(), other.end());
	}

	// Prints
	void print()const {
		std::cout << "ndArray(";
		if (this->size() > 0) {
			iArray startIndex(this->nDims(), 0);
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
		int dims = (int)std::count_if(m_shape.begin(), m_shape.end(), [](int dim) {return dim >= 1; });
		if (dims == 0 && m_shape.at(0) == 1)
			return 1;
		else if (dims == 2 && (m_shape.at(0) == 1 || m_shape.at(1) == 1))
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

		if (this->nDims() == 1) {
			return 1;
		}
		else {
			assert(this->nDims() > axis);
			return std::accumulate(m_shape.begin() + 1 + axis, m_shape.end(), 1, std::multiplies<>());
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

	iArray argMin()const {
		size_t flatIdx = std::min_element(m_data.begin(), m_data.end()) - m_data.begin();
		return reconstructIndex((int)flatIdx);
	}
	iArray argMax()const {
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
	int flattenIndex(const iArrayLike_1d auto& index, iArray shape)const
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

	iArray getNonAxisIndex(int flatIndex, int axis)const {
		auto index = reconstructIndex(flatIndex).erase(axis);
		return index;

	}
	iArray getNonAxisIndex(iArray& index, int axis)const {
		iArray idx = index;
		idx.erase(idx.begin() + axis);
		return idx;

	}
	iArray reconstructIndex(int index)const 
	{
		if (this->nDims() == 1) {
			return iArray{index};
		}

		iArray indices = Cnum::Array::uniformArray(this->nDims(), 0);
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
	static ndArray<T> binaryOperation(const ndArray<T>& arr1, const ndArray<T>& arr2, Operation binaryOp)
	{
		ndArray<T> out(arr1.size());
		std::transform(arr1.begin(), arr1.raw().end(), arr2.begin(), out.begin(), binaryOp);
		return out.reshape(arr1.shape());
	}
	template<typename Operation>
	static ndArray<T> unaryOperation(const ndArray<T>& arr, Operation unaryOp)
	{
		ndArray<T> out(arr.size());
		std::transform(arr.begin(), arr.raw().end(), out.begin(), unaryOp);
		return out.reshape(arr.shape());
	}
	template<typename Operation>
	ndArray<T>& unaryOperation(ndArray<T>& arr, Operation unaryOp)
	{
		std::transform(arr.begin(), arr.end(), arr.begin(), unaryOp);
		return arr;
	}
	void incrementExtractionIndex(ndArray<int>& index, int axis, int dim) {

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
	void printDim(ndArray<int>& index, int dim)const
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
			if (index[dim] != 0) { std::cout << "\t"; PrintSpaces(1 + dim); }
			printDim(index, dim + 1);
			if (i == m_shape[dim] - 1) {
				std::cout << "]";
				if (dim > 0 && index[dim - 1] != m_shape[dim - 1] - 1) { 
					std::cout << "," << std::endl; 
				}
			}
			else
				std::cout << std::endl;
		}
	}
	ndArray<T>& join(ndArray<T>&& arr, int axis, int offset) {

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

			size_t newNumberOfElements = this->size() + arr.size();
			int startIndex = (offset == -1) ? this->shapeAlong(axis) : offset;

			// Update the shape first so that the stepLength can be computed correctly
			m_shape[axis] += arr.shapeAlong(axis);
			int stepLength = stride * this->shapeAlong(axis);
			int j = 0;

			for (size_t i = startIndex; i < newNumberOfElements; i += stepLength) {
				for (int k = 0; k < arr.shapeAlong(axis); k++) {
					m_data.insert(this->begin() + i + k, arr.at(arr.reconstructIndex(j)));
					j++;
				}
			}
		}
		return *this;
	}
	ndArray<T>& replaceAlong(ndArray<T>& newData, int axis, iArray& nonAxisIndices) {
		return this->replaceAlong(std::move(newData), axis, nonAxisIndices);
	}
	ndArray<T>& replaceAlong(ndArray<T>&& newData, int axis, iArray& nonAxisIndices) {

		//assert(nonAxisIndices.size() == this->nDims() - 1);
		//assert(newData.size() == this->shapeAlong(axis));

		int stride = getStride(axis);
		auto start_stop = determineStartEndIndexForAxis(axis, nonAxisIndices, 0, -1);
		int j = 0;
		for (int i = start_stop.first; i <= start_stop.second - stride; i += stride) {
			m_data.at(i) = newData[j];
			j++;
		}
		return *this;
	}

	// Creators
	static iArray createLogicalArray(const ndArray<T>& arr1, const ndArray<T>&& arr2, std::function<bool(T, T)> func)
	{
		return createLogicalArray(arr1, arr2, func);
	}
	static iArray createLogicalArray(const ndArray<T>& arr1, const ndArray<T>& arr2, std::function<bool(T,T)> func)
	{
		ndArray<int> out(arr1.size());
		std::transform(arr1.begin(), arr1.end(), arr2.begin(), out.begin(), func);
		return out.reshape(arr1.shape());
	}
	static iArray createLogicalArray(const ndArray<T>& arr, T value, std::function<bool(T)> func)
	{
		ndArray<int> out(arr.size()); 
		std::transform(arr.begin(), arr.end(), out.begin(), func);
		return out.reshape(arr.shape());
	}
	
	// Misc
	int getDominantAxis_1d()const
	{
		assert(nDims() == 1);
		return (int)(std::max_element(m_shape.begin(), m_shape.end()) - m_shape.begin());
	}
	std::pair<int, int> determineStartEndIndexForAxis(int axis, const ndArray<int>& nonAxisIndex, int start, int end)const
	{ 
		if (this->nDims() == 1) {
			end = (end < 0) ? m_shape.at(this->getDominantAxis_1d()) + end + 1 : end;
			return std::make_pair(start, end);
		}

		auto startIndex = nonAxisIndex;
		startIndex.insert(startIndex.begin()+axis, start);

		auto endIndex = startIndex;
		endIndex.at(axis) = (end < 0) ? m_shape.at(axis) + end + 1: end;

		return std::make_pair(flattenIndex(startIndex), flattenIndex(endIndex)); 
	}
	void swap(ndArray<T>& arr1, ndArray<T>& arr2)
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

typedef ndArray<int> iArray;
typedef ndArray<float> fArray;
typedef ndArray<double> dArray;

}