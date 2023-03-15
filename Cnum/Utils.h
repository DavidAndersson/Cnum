#pragma once
#include <vector>
#include <string>
#include <sstream>

static std::string toString(const std::vector<int> shape)
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


static int multiplyElements(const std::vector<int>& vec)
{
	return std::accumulate(vec.begin(), vec.end(), 1, std::multiplies<int>());
}