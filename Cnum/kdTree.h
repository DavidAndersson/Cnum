#pragma once
#include "Rect.h"
#include "Vec2.h"
#include <vector>
#include <memory>

template<typename T>
class kdTree
{
public:

	kdTree(int dim, Rect& bdry)
		: m_boundary(bdry), m_dim(dim)
	{
		for (int i = 0; i < std::pow(2, dim); i++) {
			m_children.push_back(nullptr);
		}
	};

	void Insert(T& object, Vec2& pos) {
		if (!m_boundary.Contains(pos)) {
			return;
		}

		if (m_objects.size() < m_capacity) {
			m_objects.push_back(object);
			m_positions.push_back(pos);
		}
		else {
			if (!m_divided) {
				Subdivide();
			}

			for (auto& child : m_children)
				child->Insert(object, pos);
		}
	}

	std::vector<T> Query(Rect& query_boundary) {
		std::vector<T> found; 
		return Query(query_boundary, found);
	}

	std::vector<T> Query(Rect& query_boundary, std::vector<T>& found)
	{

		if (!m_boundary.IsOverlappingWith(query_boundary)) {
			return found;
		}
		else {
			if (!m_divided) {
				for (int i = 0; i < m_positions.size(); i++) {
					if (query_boundary.Contains(m_positions[i])) {
						found.push_back(m_objects[i]);
					}
				}
			}
			else {
				for (auto& child : m_children)
					child->Query(query_boundary, found);
			}

			return found;
		}
	}


private:

	void Subdivide() {

		// Creating the children with smart pointers
		std::vector<Rect> childrenRects = m_boundary.SubDivide();
		for (int i = 0; i < std::pow(2, m_dim); i++)
			m_children[i] = std::make_unique<kdTree>(m_dim, childrenRects[i]);

		// Re-distribute the points in the parent to the children
		for (int i = 0; i < m_positions.size(); i++) {
			for (auto& child : m_children)
				child->Insert(m_objects[i], m_positions[i]);
		}

		// Remove all the data from the parent
		m_objects.clear();
		m_positions.clear();
		m_divided = true;
	}

private:
	Rect m_boundary;
	int m_capacity = 1; 
	std::vector<T> m_objects;
	std::vector<Vec2> m_positions;
	bool m_divided = false;
	int m_dim = 0;

	std::vector<std::unique_ptr<kdTree>> m_children; 
	
};

