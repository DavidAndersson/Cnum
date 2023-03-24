#pragma once
#include <vector>
#include <string>
#include <sstream>

static std::string toString(const std::vector<int> arr)
{
	std::stringstream ss;
	ss << "(";
	for (int i = 0; i < arr.size(); i++) {
		ss << std::to_string(arr[i]);
		if (i < arr.size() - 1)
			ss << ", ";
	}
	ss << ")";
	return ss.str();
}

static void PrintSpaces(int n) {
	for (int i = 0; i < n; i++)
		std::cout << " ";
}

static int multiplyElements(const std::vector<int>& vec)
{
	return std::accumulate(vec.begin(), vec.end(), 1, std::multiplies<int>());
}

