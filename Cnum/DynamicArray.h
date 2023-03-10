#pragma once
#include <vector>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <stdexcept>
#include <format>
#include <algorithm>
#include <iterator>
#include <functional>
#include <sstream>
#include <math.h>


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
		: m_data(src.m_data), m_shape(src.m_shape) {};
	template<typename S>
	DynamicArray(const DynamicArray<S>& src)
		: m_data(src.m_data), m_shape(src.m_shape) {};

	// Move Constructors
	DynamicArray(const DynamicArray<T>&& other)
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
		if (getNumberOfElements(shape) != m_data.size()) {
			throw std::invalid_argument(std::format("Cannot create array of size {} with shape {}", m_data.size(), toString(shape)));
		}

		if (m_shape.size() == 1) {
			m_shape = std::vector<int>{ m_shape[0], 1 };
		}
	}
	DynamicArray(const std::vector<T>&& other, const std::vector<int>& shape)
		: DynamicArray(std::move(other), std::move(shape))
	{}


	//--------------------------
	// Operators
	// -------------------------

	// Indexing
	T& operator[](std::vector<int> index)
	{
		return m_data.at(flattenIndex(index));
	}
	T& operator[](DynamicArray<int>& index) {
		if (index.nDims != m_shape.size()) {
			throw std::invalid_argument(std::format("Index {} is not of the same order as shape {}", index.sshape(), toString(m_shape)));
			exit(1);
		}
		return m_data.at(flattenIndex(index.m_data));
	}
	T& operator[](int index)
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
	DynamicArray<T> operator=( DynamicArray<T>&& rhs) {
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
		std::vector<T> sum = m_data;
		std::transform(m_data.begin(), m_data.end(), rhs.raw().begin(), sum.begin(), std::plus<T>());
		auto arr = DynamicArray(sum, this->shape());
		return arr;
	}
	DynamicArray<T> operator+(const T value)const
	{
		DynamicArray<T> other = DynamicArray<T>(m_shape, value);
		return *this + other;
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
		DynamicArray<T> difference = rhs;
		std::transform(m_data.begin(), m_data.end(), rhs.m_data.begin(), difference.m_data.begin(), std::minus<T>());
		return difference;
	}
	DynamicArray<T> operator-(const T value)const
	{
		DynamicArray<T> other = DynamicArray<T>(m_shape, value);
		return *this - other;
	}
	void operator-=(const DynamicArray& rhs)
	{
		*this = *this - rhs;
	}
	void operator-=(const T rhs) {
		*this = *this - rhs;
	}

	// Multiplication
	DynamicArray<T> operator*(const DynamicArray& rhs)const
	{
		DynamicArray<T> product = rhs;
		std::transform(m_data.begin(), m_data.end(), rhs.m_data.begin(), product.m_data.begin(), std::multiplies<T>());
		return product;
	}
	DynamicArray<T> operator*(const T value)const
	{
		DynamicArray<T> other = DynamicArray<T>(m_shape, value);
		return *this * other;
	}
	void operator*=(const DynamicArray& rhs)
	{
		*this = *this * rhs;
	}
	void operator*=(const T rhs) {
		*this = *this * rhs;
	}

	// Division
	DynamicArray<T> operator/(const DynamicArray& rhs)const
	{
		if (std::find(rhs.raw().begin(), rhs.raw().end(), 0) != rhs.raw().end()) {
			throw std::invalid_argument("Division by zero encountered"); 
			exit(1);
		}
		std::vector<T> data; 
		std::transform(m_data.begin(), m_data.end(), rhs.raw().begin(), data.begin(), std::divides<T>());
		return DynamicArray<T>(data, m_shape);
	}
	DynamicArray<T> operator/(const T value)const
	{
		DynamicArray<T> other = DynamicArray<T>(m_shape, value);
		return *this / other;
	}
	void operator/=(const DynamicArray& rhs)
	{
		*this = *this / rhs;
	}
	void operator/=(const T rhs) {
		*this = *this / rhs;
	}

	// Equailty
	DynamicArray<bool> operator==(const DynamicArray& rhs)const
	{
		if (this->sameShapeAs(rhs) == false) {
			throw std::invalid_argument(std::format("Shape {} and {} do not match", this->sshape(), rhs.sshape()));
			exit(1);
		}
		std::vector<bool> out(getNumberOfElements(m_shape), false);
		std::transform(m_data.begin(), m_data.end(), rhs.raw().begin(), out.begin(), [](T v1, T v2) {return v1 == v2; });
		return DynamicArray<bool>(out, this->shape());
	}
	DynamicArray<bool> operator==(const T rhs)const {
		std::vector<bool> out(getNumberOfElements(m_shape), false); 
		std::transform(m_data.begin(), m_data.end(), out.begin(), [&](T v) {return v == rhs;});
		return DynamicArray<bool>(out, m_shape);
	}

	// Anti-Equality
	DynamicArray<bool> operator!=(const DynamicArray& rhs)const
	{
		if (this->sameShapeAs(rhs) == false) {
			throw std::invalid_argument(std::format("Shape {} and {} do not match", this->sshape(), rhs.sshape()));
			exit(1);
		}
		std::vector<bool> out(getNumberOfElements(m_shape), false);
		std::transform(m_data.begin(), m_data.end(), rhs.raw().begin(), out.begin(), [](T v1, T v2) {return v1 != v2; });
		return DynamicArray<bool>(out, m_shape);
	}
	DynamicArray<bool> operator!=(const T rhs)const {
		std::vector<bool> out(getNumberOfElements(m_shape), false);
		std::transform(m_data.begin(), m_data.end(), out.begin(), [&](T v) {return v != rhs; });
		return DynamicArray<bool>(out, m_shape);
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
	DynamicArray<T> Flatten()
	{
		m_shape = { this->size() };
		return *this;
	}
	DynamicArray<T> abs()
	{
		std::transform(m_data.begin(), m_data.end(), m_data.begin(), [](T e) {return std::abs(e); });
		return *this;
	}
	DynamicArray<T> Reshape(const std::vector<int>& newShape)
	{
		// The new shape must have the same number of elements as the previous had
		if (getNumberOfElements(newShape) != this->size()) {
			std::cerr << std::format("Invalid shape {}", toString(newShape)) << std::endl;
			exit(0);
		}

		m_shape = newShape;
		return *this;
	}

	DynamicArray<T> Transpose() {
		std::vector<int> permutation = std::vector<int>(m_shape.size());
		std::iota(permutation.begin(), permutation.end(), 0);
		std::reverse(permutation.begin(), permutation.end());
		return this->Transpose(permutation);

	}
	DynamicArray<T> Transpose(DynamicArray<int>&& permutation) {
		return this->Transpose(std::move((permutation.raw())));
	}
	DynamicArray<T> Transpose(DynamicArray<int>& permutation) {
		return this->Transpose(std::move(permutation));
	}
	DynamicArray<T> Transpose(std::vector<int>& permutation) {
		return this->Transpose(std::move(permutation));
	}
	DynamicArray<T> Transpose(std::vector<int>&& permutation)
	{
		/*
			What is a permutation? 
				A permutation, same size as the shape, tells which axis should move to where. 
				If the original shape is (2,3,5) and the permutation is (0, 2, 1), then the new shape is (2, 5, 3)
				
				The default permutation is to reverse the shape
		*/

		std::vector<int> permValues = std::vector<int>(m_shape.size());
		std::iota(permValues.begin(), permValues.end(), 0);
		if (std::is_permutation(permutation.begin(), permutation.end(), permValues.begin(), permValues.end()) == false) {
			throw std::invalid_argument("Incorrectly specifed permutation"); 
			exit(1);
		}

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
	
	void append(const T value) {
		if (this->nDims() > 1) {
			throw std::exception("Cannot call append on a multi dimensional array");
			exit(1);
		}
		m_data.push_back(value);

		if (m_shape.empty()) {
			m_shape = std::vector<int>{ 1,1 };
			return;
		}
		if (m_shape[0] == 1)
			m_shape[1]++;
		else
			m_shape[0]++;
	}

	// Reductions
	template<typename BinaryFunction>
	T Reduce(T initVal, BinaryFunction op)const {
		return std::accumulate(m_data.begin(), m_data.end(), initVal, op);
	}
	DynamicArray<T> ReduceAlongAxis(int axis)const
	{
		// The axis which the sum is along gets reduced to 1
		std::vector<int> returnShape = m_shape;
		returnShape[axis] = 1;
		DynamicArray<T> returnArray = DynamicArray<T>(returnShape, 0);

		for (int i = 0; i < getNumberOfElements(returnShape); i++) {

			returnArray[i] = this->ExtractAxis(axis, i).Reduce(0, std::plus());

		}
		return returnArray;
	}

	// Extractions
	DynamicArray<T> ExtractAxis(int axis, std::vector<int>nonAxisLock, int start=0, int end=-1)const
	{
		// Error checking
		if (axis > m_shape.size()) {
			std::cerr << std::format("Error! Axis {} not valid for shape {}", axis, toString(m_shape)) << std::endl;
			exit(0);
		}
		if (nonAxisLock.size() != m_shape.size() - 1) {
			std::cerr << std::format("Error! Need to specify Only {} constraint(s) in function: Extract()", m_shape.size() - 1) << std::endl;
			exit(0);
		}
		int nonAxisIdx = 0;
		for (int i = 0; i < m_shape.size(); i++) {
			if (i != axis) {
				if (nonAxisLock[nonAxisIdx] > m_shape[i]) {
					std::cerr << std::format("Error! {} is out of range for axis {} in shape {}", nonAxisLock[nonAxisIdx], i, toString(m_shape)) << std::endl;
					exit(0);
				}
				nonAxisIdx++;
			}
		}

		// If negative end - wrap around
		if (end < 0)
			end = m_shape[axis] + end;

		// Determine starting index in the array
		auto startIndex = nonAxisLock;
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
	DynamicArray<T> ExtractAxis(int axis, int nonAxisLock, int start = 0, int end = -1)const {
		return ExtractAxis(axis, std::vector<int>(1, nonAxisLock), start, end);
	}

	// Boolean checks
	bool Contains(DynamicArray<T>& point)
	{
		if (point.shapeAlong(1) != this->shapeAlong(1))
			throw std::invalid_argument(std::format("Point and shape must both be either {} or {} dimensional", this->shapeAlong(1), point.shapeAlong(0)));

		// Goes through the elements of the 1d point, and checks if it is contained by the correct axis in the parent
		// Note that the method is inclusive at the lower end and exclusive at the upper end
		for (int i = 0; i < point.m_data.size(); i++) {
			if ((point[i] >= this->ExtractAxis(0, i).min() && point[i] < this->ExtractAxis(0, i).max()) == false) {
				return false;
			}
		}
		return true;
	} 
	bool isEqualTo(DynamicArray<T> other)const {
		return std::equal(m_data.begin(), m_data.end(), other.m_data.begin());
	}
	bool sameShapeAs(DynamicArray<T> other)const {
		return std::equal(m_shape.begin(), m_shape.end(), other.shape().begin());
	}

	

	// Prints
	void Print() {
		auto indices = std::vector<int>((int)m_shape.size(), 0);
		std::cout << "Cnum::Array(";
		PrintDim(indices, 0);
		std::cout << ")" << std::endl;
	}


	// Getters
	std::vector<int> shape()const { return m_shape; };
	int shapeAlong(int axis)const { 
		if (axis < m_shape.size()) { 
			return m_shape[axis]; 
		} 
		else {
			std::cerr << std::format("Cannot access axis {} in array of shape {}", axis, toString(m_shape)); 
			exit(1);
		}
	}
	std::string sshape()const { return toString(m_shape); };
	int nDims()const {
		return (int)std::count_if(m_shape.begin(), m_shape.end(), [](int dim) {return dim > 1; });
	}
	int size()const { return getNumberOfElements(); };
	int getStride(int axis = 0)const
	{
		/*
			What is a stride?
				- How far you have to move in the 1d vector to get to the next element along the specified axis
		*/
		return std::accumulate(m_shape.begin() + 1 + axis, m_shape.end(), 1, std::multiplies<>());
	}

	const std::vector<T>& raw()const { return this->m_data; };
	std::vector<T> raw() { return this->m_data; }

	T min() { return *std::min_element(m_data.begin(), m_data.end());}
	T max() { return *std::max_element(m_data.begin(), m_data.end());}

private:

	//--------------------------
	// Private Interface
	// -------------------------

	int flattenIndex(std::vector<int>& indices)const
	{
		return flattenIndex(indices, m_shape);
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
	std::vector<int> reconstructIndex(int index) {

		std::vector<int> indices = std::vector<int>(m_shape.size(), 0);

		for (int i = 0; i < m_shape.size(); i++) {

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
		return getNumberOfElements(m_shape);
	}
	int getNumberOfElements(const std::vector<int>& shape)const
	{
		return std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int>());
	}

	std::string toString(const std::vector<int> shape)const
	{
		std::stringstream ss;
		ss << "(";
		for (int i = 0; i < shape.size(); i++) {
			ss << std::to_string(shape[i]);
			if (i < shape.size() - 1)
				ss << ", ";
		}
		ss << ")";
		return ss.str();
	}

	void PrintDim(std::vector<int>& index, int dim) 
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
			if (dim == 0 && index[dim] != 0) { std::cout << "\t     "; }
			PrintDim(index, dim + 1);
			if (i == m_shape[dim] - 1) {
				std::cout << "]";
				if (dim > 0 && index[dim - 1] != m_shape[dim - 1] - 1) { std::cout << "," << std::endl; }
			}
			else
				std::cout << std::endl;
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

