#include "DynamicArray.h"
#include <numeric>
#include <algorithm>
#include <iterator>
#include <functional>
#include <sstream>
#include <math.h>

template class DynamicArray<int>;
template class DynamicArray<float>;
template class DynamicArray<double>;

// ---------------------------------------
// Array Creation
//----------------------------------------

template<typename T>
DynamicArray<T> DynamicArray<T>::Arange(T start, T end, T stepSize)
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

template<typename T>
DynamicArray<T> DynamicArray<T>::Linspace(T start, T end, int nSteps)
{
	T rangeLength = end - start;
	T stepSize = rangeLength / (nSteps-1); 
	
	std::vector<T> initializer;
	for (int i = 0; i < nSteps; i++) {
		initializer.push_back(start + i*stepSize);
	}

	return DynamicArray<T>{initializer};
}

template<typename T>
DynamicArray<T> DynamicArray<T>::FromFile(std::string_view filePath, char delimiter)
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

// ---------------------------------------
// Operator Overloading
// ---------------------------------------


template<typename T>
T& DynamicArray<T>::operator[](std::vector<int> indices)
{
	return m_data.at(flattenIndex(indices));
}

template<typename T>
T& DynamicArray<T>::operator[](int index)
{
	return m_data[index];
}



template<typename T>
DynamicArray<T> DynamicArray<T>::operator+(const DynamicArray& rhs) const
{
	DynamicArray<T> sum = rhs; 
	std::transform(m_data.begin(), m_data.end(), rhs.m_data.begin(), sum.m_data.begin(), std::plus<T>());
	return sum;
}

template<typename T>
void DynamicArray<T>::operator+=(const DynamicArray& rhs)
{
	*this = *this + rhs;
}

template<typename T>
DynamicArray<T> DynamicArray<T>::operator-(const DynamicArray& rhs) const
{
	DynamicArray<T> sum = rhs;
	std::transform(m_data.begin(), m_data.end(), rhs.m_data.begin(), sum.m_data.begin(), std::minus<T>());
	return sum;
}

template<typename T>
void DynamicArray<T>::operator-=(const DynamicArray& rhs)
{
	*this = *this - rhs;
}

template<typename T>
DynamicArray<T> DynamicArray<T>::operator*(const DynamicArray& rhs) const
{
	DynamicArray<T> sum = rhs;
	std::transform(m_data.begin(), m_data.end(), rhs.m_data.begin(), sum.m_data.begin(), std::multiplies<T>());
	return sum;
}

template<typename T>
void DynamicArray<T>::operator*=(const DynamicArray& rhs)
{
	*this = *this * rhs;
}

template<typename T>
DynamicArray<T> DynamicArray<T>::operator/(const DynamicArray& rhs) const
{
	DynamicArray<T> sum = rhs;
	std::transform(m_data.begin(), m_data.end(), rhs.m_data.begin(), sum.m_data.begin(), std::divides<T>());
	return sum;
}

template<typename T>
void DynamicArray<T>::operator/=(const DynamicArray& rhs)
{
	*this = *this / rhs;
}



// ---------------------------------------
// Public Interface
//----------------------------------------

template<typename T>
void DynamicArray<T>::Reshape(const std::vector<int>& newShape)
{
	// Firstly the new shape must have the same number of elements as the previous had
	if (int nIndicesNew = getNumberOfElements(newShape); nIndicesNew != getNumberOfElements()) {
		std::cerr << std::format("Invalid shape {}", toString(newShape)) << std::endl;
		exit(0);
	}

	m_shape = newShape;
}

template<typename T>
DynamicArray<T> DynamicArray<T>::Transpose()
{
	DynamicArray<T> copy = *this;
	std::reverse(copy.m_shape.begin(), copy.m_shape.end());

	int stride = getStride();
	int place = 0;

	for (int i = 0; i < stride; i++) {
		for (int j = i; j < getNumberOfElements(); j += stride) {
			copy.m_data[place] = m_data[j];
			place++;
		}
	}

	return copy;
}

template<typename T>
DynamicArray<T> DynamicArray<T>::Flatten()
{
	DynamicArray<T> copy = *this;
	copy.m_shape = { getNumberOfElements() }; 
	return copy;
}

template<typename T>
DynamicArray<T> DynamicArray<T>::ExtractAxis(int axis, std::vector<int>nonAxisLock, int start, int end)const
{
	// Error checking
	if (axis > m_shape.size()) {
		std::cerr << std::format("Error! Axis {} not valid for shape {}", axis, toString(m_shape)) << std::endl;
		exit(0);
	}
	if (nonAxisLock.size() != m_shape.size() - 1) {
		std::cerr << std::format("Error! Need to specify Only {} constraint(s) in function: Extract()", m_shape.size() - 1)<< std::endl;
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


template<typename T>
DynamicArray<T> DynamicArray<T>::abs()
{
	std::transform(m_data.begin(), m_data.end(), m_data.begin(), [](T e) {return std::abs(e); });
	return *this;
}


// Only tested in 2 dimensions
template<typename T>
void DynamicArray<T>::Concatenate(DynamicArray<T>& other, int axis)
{
	// The off-axis dimensions must all be the same for a valid concatenation
	for (int i = 0; i < m_shape.size(); i++) {
		if (i == axis)
			continue;
		if (other.shape()[i] != m_shape[i]) {
			throw std::invalid_argument(std::format("Arrays are not of equal length in axis {}", axis));
		}		
	}

	if (axis == 0) {
		m_data.insert(m_data.end(), other.m_data.begin(), other.m_data.end());
		m_shape[axis] += other.shape()[axis]; 
	}
		
	else {
		int stride = getStride(axis);
		int startIndex = stride;
		int j = 0; 
		for (int i = startIndex; i < m_data.size(); i += stride) {
			m_data.insert(m_data.begin() + i + j, other[j]);
			j++;
		}
		m_shape[axis] += other.shape()[axis];
	}

}

template<typename T>
bool DynamicArray<T>::Contains(DynamicArray<T>& point)
{
	if (point.shape()[1] != m_shape[1])
		throw std::invalid_argument(std::format("Point and shape must both be either {} or {} dimensional", m_shape[1], point.shape()[0]));

	// Goes through the elements of the 1d point, and checks if it is contained by the correct axis in the parent
	// Note that the method is inclusive at the lower end and exclusive at the upper end
	for (int i = 0; i < point.m_data.size(); i++) {
		if ((point[i] >= this->ExtractAxis(0, i).min() && point[i] < this->ExtractAxis(0, i).max()) == false) {
			return false;
		}
	}
	return true;
}



// ---------------------------------------
// Private Interface
//----------------------------------------


template<typename T>
void DynamicArray<T>::ToFile(std::string_view filename, DynamicArray<T>& data, char writeMode, char delimiter)
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

template<typename T>
int DynamicArray<T>::flattenIndex(std::vector<int>& indices)const
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

	for (int j = 0; j < indices.size()-1; j++)
		index += indices[j] * std::accumulate(m_shape.begin()+j+1, m_shape.end(), 1, std::multiplies<int>());
	
	index += indices[indices.size() - 1];
	return index;
}

template<typename T>
void DynamicArray<T>::initArray(T value)
{
	m_data = std::vector<T>(getNumberOfElements(), value);
}

template <typename T>
int DynamicArray<T>::getNumberOfElements()const {

	return getNumberOfElements(m_shape);
}

template<typename T>
int DynamicArray<T>::getNumberOfElements(const std::vector<int>& shape) const
{
	return std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int>());
}

template<typename T>
int DynamicArray<T>::getStride(int axis) const
{	
	/*
		What is a stride?
			- How far you have to move in the 1d vector to get to the next element along the specified axis
	*/
	return std::accumulate(m_shape.begin() + 1 + axis, m_shape.end(), 1, std::multiplies<>());
}

template<typename T> 
std::string DynamicArray<T>::toString(const std::vector<int> shape) const
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

