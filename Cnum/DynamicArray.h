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

	// Alternatively have a flag declaring that the array is uninitialized?
	DynamicArray() = default;

	// Constructors
	DynamicArray(std::vector<T>& initializer, std::vector<int>& shape)
		: m_data{ initializer }, m_shape{shape}
	{
		if (getNumberOfElements(shape) != m_data.size()) {
			throw std::invalid_argument(std::format("Cannot create array of size {} with shape {}", m_data.size(), toString(shape)));
		}

		if (shape.size() == 1) {
			shape = std::vector<int>{ 1, shape[0] };
		}
	}

	DynamicArray(std::vector<T>& initializer)
		: m_data{ initializer }
	{
		m_shape = std::vector<int>{ 1, (int)initializer.size() };
	};

	DynamicArray(std::vector<int> shape, T initialValue)
		: m_shape{ shape } 
	{
		initArray(initialValue);

		if (shape.size() == 1) {
			shape = std::vector<int>{ 1, shape[0] };
		}
	};


	// Copy Constructor
	template<typename S>
	DynamicArray(DynamicArray<S>& src)
		: m_data(src.m_data), m_shape(src.m_shape) {};


	// Static Creators -- Make private, and friend to Cnum?
	static DynamicArray<T> Arange(T start, T end, T stepSize) 
	{
		if (std::abs(end - start) < stepSize) {
			throw std::invalid_argument("Step size cannot be larger than the range");
		}

		if (start > end)
			stepSize *= -1;

		std::vector<T> initializer;
		for (T i = start; i > end; i += stepSize) {
			initializer.push_back(i);
		}

		return DynamicArray<T>{initializer};
	}
	static DynamicArray<T> Linspace(T start, T end, int nSteps) 
	{
		T rangeLength = end - start;
		T stepSize = rangeLength / (nSteps - 1);

		std::vector<T> initializer;
		for (int i = 0; i < nSteps; i++) {
			initializer.push_back(start + i * stepSize);
		}

		return DynamicArray<T>{initializer};
	}
	static DynamicArray<T> FromFile(std::string_view filePath, char delimiter = ' ') 
	{
		std::fstream dataFile(filePath.data(), std::ios::in);
		int rowCount = 0, colCount = 0, finalColCount = 0;
		std::vector<T> initializer;

		if (dataFile.is_open())
		{
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
		}
		else
			throw std::runtime_error("Could not open the file");

		std::vector<int> shape = { rowCount, finalColCount };
		return DynamicArray<T>(initializer, shape);
	}


	// Operators
	T& operator[](std::vector<int> indices)
	{
		return m_data.at(flattenIndex(indices));
	}
	//T& operator[](DynamicArray<T> indices);
	T& operator[](int index)
	{
		return m_data[index];
	}

	DynamicArray<T> operator+(const DynamicArray& rhs)const
	{
		DynamicArray<T> sum = rhs;
		std::transform(m_data.begin(), m_data.end(), rhs.m_data.begin(), sum.m_data.begin(), std::plus<T>());
		return sum;
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

	DynamicArray<T> operator/(const DynamicArray& rhs)const
	{
		DynamicArray<T> result = rhs;
		std::transform(m_data.begin(), m_data.end(), rhs.m_data.begin(), result.m_data.begin(), std::divides<T>());
		return result;
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

	bool operator==(const DynamicArray& rhs)const
	{
		return std::equal(m_data.begin(), m_data.end(), rhs.m_data.begin());
	}
	bool operator!=(const DynamicArray& rhs)const
	{
		return !(*this == rhs);
	}

public:

	// Public Interface
	void Reshape(const std::vector<int>& newShape)
	{
		// Firstly the new shape must have the same number of elements as the previous had
		if (int nIndicesNew = getNumberOfElements(newShape); nIndicesNew != getNumberOfElements()) {
			std::cerr << std::format("Invalid shape {}", toString(newShape)) << std::endl;
			exit(0);
		}

		m_shape = newShape;
	}
	
	// Add function in Cnum which copies
	// Add overload without permutation, where the default then is the identity
	DynamicArray<T> Transpose(std::vector<int> permutation)
	{
		/*
			What is a permutation? 
				A permutation, same size as the shape, tells which axis should move to where. 
				If the original shape is (2,3,5) and the permutation is (0, 2, 1), then the new shape is (2, 5, 3)

				There are identity permutations, which rolls forwards or backward: 
					(1, 2, 0) - Backward
					(2, 0, 1) - Forward
		*/

		// Check that permutation has correct size and that it has the correct values etc

		std::vector<T> newData = std::vector<T>(m_data.size(), 0);
		std::vector<int> newShape = std::vector<int>(this->shape().size(), 0);

		// Swap places of the data w.r.t the permutation
		for (int i = 0; i < newData.size(); i++) {
			std::vector<int> indices = reconstructIndex(i);
			std::vector<int> temp = this->shape();
			for (int j = 0; j < this->shape().size(); j++) {
				temp[j] = indices[permutation[j]];
			}
			newData[i] = m_data[flattenIndex(temp)];
		}
		// Update the shape based on the permutation
		for (int j = 0; j < this->shape().size(); j++) {
			newShape[j] = this->shapeAlong(permutation[j]);
		}

		m_data = newData;  m_shape = newShape;
		return *this;
	} 
	
	DynamicArray<T> Flatten()
	{
		DynamicArray<T> copy = *this;
		copy.m_shape = { getNumberOfElements() };
		return copy;
	}

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
	DynamicArray<T> abs()
	{
		std::transform(m_data.begin(), m_data.end(), m_data.begin(), [](T e) {return std::abs(e); });
		return *this;
	}

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

	void Concatenate(DynamicArray<T> other, int axis = 0, int offset=-1)
	{
		// If the array is uninitialized i.e. empty, the concatenation will simply act as assignment
		if (m_data.empty()) {
			*this = other;
			return;
		}

		// The off-axis dimensions must all be the same for a valid concatenation
		for (int i = 0; i < m_shape.size(); i++) {
			if (i == axis)
				continue;
			if (other.shapeAlong(i) != this->shapeAlong(i)) {
				throw std::invalid_argument(std::format("Arrays are not of equal length in axis {}", axis));
			}
		}

		int stride = getStride(axis);

		if (axis == 0) {
			auto startPoint = (offset == -1) ? m_data.end() : m_data.begin() + offset * stride;
			m_data.insert(startPoint, other.m_data.begin(), other.m_data.end());
			m_shape[axis] += other.shapeAlong(axis);
		}

		else {

			int newNumberOfElements = this->size() + other.size();

			// Update the shape first so that the stepLength can be computed correctly
			m_shape[axis] += other.shapeAlong(axis);
			int stepLength = stride * this->shapeAlong(axis);

			int startIndex = (offset == -1) ? stepLength - 1 : offset;  // Most likely wrong. Maybe stride*offset

			int j = 0;
			for (int i = startIndex; i < newNumberOfElements; i += stepLength) {
				m_data.insert(m_data.begin() + i, other[j]);
				j++;
			}
		}
	}
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

	void Print() {
		auto indices = DynamicArray<int>({ (int)m_shape.size() }, 0);
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
	int size()const { return getNumberOfElements(); };
	std::vector<T> raw()const { return m_data; };
	T min() { return *std::min_element(m_data.begin(), m_data.end());}
	T max() { return *std::max_element(m_data.begin(), m_data.end()); }

	static void ToFile(std::string_view filename, DynamicArray<T>& data, char writeMode = 'w', char delimiter = ' ')

	{
		// Problems:
		//	Two different delimiters can be used if the write mode is append


		if (data.m_shape.size() > 2)
			throw std::runtime_error("Cannot save data in higher dimension than 2");

		if (writeMode != 'w' && writeMode != 'a')
			throw std::invalid_argument("Write mode must be either 'w' or 'a'");

		auto mode = (writeMode == 'w') ? std::ios::out : std::ios::app;
		std::ofstream dataFile(filename.data(), mode);

		int stride = data.getStride();

		for (int i = 0; i < data.getNumberOfElements(); i++) {

			std::string s = std::to_string(data.m_data[i]);
			dataFile << s;

			if (fmod(i + 1, stride) == 0)
				dataFile << "\n";
			else
				dataFile << delimiter;
		}


	}

private:

	// Private Interface
	int flattenIndex(DynamicArray<int>& indices)const {
		auto ind = indices.raw();
		return flattenIndex(ind); 
	}
	int flattenIndex(std::vector<int>& indices)const

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
			index += indices[j] * std::accumulate(m_shape.begin() + j + 1, m_shape.end(), 1, std::multiplies<int>());

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
			for (; j < m_shape[i] - 1 && index > 0; j++) {
				index -= stride;
			}
			indices[i] = j;

			if (index < 0)
				break;
		}
		return indices;
	}
	void initArray(T value)
	{
		m_data = std::vector<T>(getNumberOfElements(), value);
	}
	int getNumberOfElements()const
	{
		return getNumberOfElements(m_shape);
	}
	int getNumberOfElements(const std::vector<int>& shape)const
	{
		return std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int>());
	}
	int getStride(int axis=0)const
	{
		/*
			What is a stride?
				- How far you have to move in the 1d vector to get to the next element along the specified axis
		*/
		return std::accumulate(m_shape.begin() + 1 + axis, m_shape.end(), 1, std::multiplies<>());
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

	void PrintDim(DynamicArray<int>& index, int dim) 
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
			PrintDim(index, dim + 1);
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



private:

	std::vector<T> m_data; 
	std::vector<int> m_shape; 

};


typedef DynamicArray<int> iArray;
typedef DynamicArray<float> fArray;
typedef DynamicArray<double> dArray;

