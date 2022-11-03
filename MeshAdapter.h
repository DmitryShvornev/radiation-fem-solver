#pragma once
#include "Data.h"
#include <numeric>
#include <algorithm>
#include <map>

template <class __ElementType>
class MeshAdapter {
private:
	std::vector<std::vector<std::size_t>> m_elements_to_nodes_tags;
	std::vector<std::size_t> m_nodes_tags;
	std::vector<double> m_coords;
public:
	MeshAdapter(const std::vector<double> p_coords, const std::vector<std::size_t> p_nodes_tags,
		const std::vector<std::vector<std::size_t>> p_elements_to_nodes_tags);
	Mesh<__ElementType> adaptMesh();
	~MeshAdapter() {};
};
