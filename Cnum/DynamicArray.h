#pragma once
#include <vector>
#include <numeric>
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <format>


template<typename T>
class DynamicArray
{

public:

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
	}; 

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


	// Static Creators -- Make private, and friend to Cnum
	static DynamicArray<T> Arange(T start, T end, T stepSize);
	static DynamicArray<T> Linspace(T start, T end, int nSteps);
	static DynamicArray<T> FromFile(std::string_view filePath, char delimiter = ' ');


	// Operators
	T& operator[](std::vector<int> indices); 
	T& operator[](int index);

	DynamicArray<T> operator+(const DynamicArray& rhs)const; 
	void operator+=(const DynamicArray& rhs); 

	DynamicArray<T> operator-(const DynamicArray& rhs)const;
	void operator-=(const DynamicArray& rhs);

	DynamicArray<T> operator*(const DynamicArray& rhs)const;
	void operator*=(const DynamicArray& rhs);

	DynamicArray<T> operator/(const DynamicArray& rhs)const;
	void operator/=(const DynamicArray& rhs);

public:

	// Public Interface
	void Reshape(const std::vector<int>& newShape);
	DynamicArray<T> Transpose(); 
	DynamicArray<T> Flatten(); 
	DynamicArray<T> ExtractAxis(int axis, std::vector<int>nonAxisLock, int start=0, int end=-1)const;
	DynamicArray<T> ExtractAxis(int axis, int nonAxisLock, int start = 0, int end = -1)const {
		return ExtractAxis(axis, std::vector<int>(1, nonAxisLock), start, end);
	}
	DynamicArray<T> abs();

	template<typename BinaryFunction>
	T Reduce(T initVal, BinaryFunction op)const {
		return std::accumulate(m_data.begin(), m_data.end(), initVal, op);
	}

	void Concatenate(DynamicArray<T>& other, int axis = 0); 
	bool Contains(DynamicArray<T>& point);

	// Getters
	std::vector<int> shape()const { return m_shape; };
	std::string sshape()const { return toString(m_shape); };
	int size()const { return getNumberOfElements(); };
	std::vector<T> raw()const { return m_data; };
	T min() { return *std::min_element(m_data.begin(), m_data.end());}
	T max() { return *std::max_element(m_data.begin(), m_data.end()); }

	// Not tested very well
	DynamicArray<T> getRow(int row) {

		int rowLength = m_shape[1];
		int startIdx = row * rowLength;
		std::vector<T> test = std::vector<T>(m_data.begin() + startIdx, m_data.begin() + startIdx + rowLength);
		return DynamicArray<T>(test); 

	}

	static void ToFile(std::string_view filename, DynamicArray<T>& data, char writeMode = 'w', char delimiter = ' ');

private:

	// Private Interface
	int flattenIndex(std::vector<int>& indices)const;
	void initArray(T value); 
	int getNumberOfElements()const;
	int getNumberOfElements(const std::vector<int>& shape)const;
	int getStride(int axis=0)const; 
	std::string toString(const std::vector<int> shape)const; 

private:

	std::vector<T> m_data; 
	std::vector<int> m_shape; 

};


typedef DynamicArray<int> iArray;
typedef DynamicArray<float> fArray;
typedef DynamicArray<double> dArray;
