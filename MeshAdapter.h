#pragma once
#include "Data.h"
#include <numeric>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <map>
#include <set>

using json = nlohmann::json;

template <class __ElementType>
class MeshAdapter {
private:
	std::vector<std::vector<std::size_t>> m_elements_to_nodes_tags;
	std::vector<std::size_t> m_nodes_tags;
	std::vector<double> m_coords;
	json m_mesh_JSON_data;
	std::map<int, int> m_mapped_tags;
public:
	MeshAdapter(const std::vector<double> p_coords, const std::vector<std::size_t> p_nodes_tags,
		const std::vector<std::vector<std::size_t>> p_elements_to_nodes_tags);
	MeshAdapter(const json p_mesh_JSON_data);
	Mesh<__ElementType> adaptMesh();
	Mesh<__ElementType> adaptJSONMesh();
	std::vector<Node> filterJSONMeshNodes(std::vector<Node> p_mesh_nodes);
	~MeshAdapter() {};
};
