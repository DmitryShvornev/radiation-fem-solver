#include "MeshAdapter.h"

template <>
MeshAdapter<Element2D>::MeshAdapter(const std::vector<double> p_coords, 
	                     const std::vector<std::size_t> p_nodes_tags, 
	                     const std::vector<std::vector<std::size_t>> p_elements_to_nodes_tags) {
	std::copy(p_coords.begin(), p_coords.end(), std::back_inserter(m_coords));
	std::copy(p_nodes_tags.begin(), p_nodes_tags.end(),
		std::back_inserter(m_nodes_tags));
	std::copy(p_elements_to_nodes_tags.begin(), p_elements_to_nodes_tags.end(),
		std::back_inserter(m_elements_to_nodes_tags));
};


template <>
MeshAdapter<Element2D>::MeshAdapter(const json p_mesh_JSON_data) {
	m_mesh_JSON_data = p_mesh_JSON_data;
}

template <>
MeshAdapter<Element3D>::MeshAdapter(const json p_mesh_JSON_data) {
	m_mesh_JSON_data = p_mesh_JSON_data;
}

template <>
std::vector<Node> MeshAdapter<Element2D>::filterJSONMeshNodes(std::vector<Node> p_mesh_nodes) {
	std::vector<Node> result_mesh_nodes;
	std::set<int> unique_nodes;
	int i = 0;
	for (auto raw_element : m_mesh_JSON_data["inner"]) {
		for (int j = 0; j < 3; j++) {
			if (unique_nodes.find(raw_element[j]) == unique_nodes.end()) {
				int key = raw_element[j];
				unique_nodes.insert(key);
				this->m_mapped_tags.insert({ key, i });
				result_mesh_nodes.push_back(p_mesh_nodes[key]);
				i++;
			}
		}
	} 
	return result_mesh_nodes;
}

template <>
Mesh<Element2D> MeshAdapter<Element2D>::adaptJSONMesh() {
	std::cout << "Mesh preprocessing..." << std::endl;
	std::vector<Node> temp_mesh_nodes;
	for (auto point : m_mesh_JSON_data["points"]) {
		Node temp = Node(point[0], point[1], point[2]);
		temp_mesh_nodes.push_back(temp);
	}
	std::vector<Node> mesh_nodes = this->filterJSONMeshNodes(temp_mesh_nodes);
	std::vector<Element2D> mesh_elements;
	for (auto raw_element : m_mesh_JSON_data["inner"]) {
		int i_node_tag = this->m_mapped_tags[raw_element[0]];
		int j_node_tag = this->m_mapped_tags[raw_element[1]];
		int k_node_tag = this->m_mapped_tags[raw_element[2]];

		Node i_node = mesh_nodes[i_node_tag];
		i_node.setGlobalID(i_node_tag);
		Node j_node = mesh_nodes[j_node_tag];
		j_node.setGlobalID(j_node_tag);
		Node k_node = mesh_nodes[k_node_tag];
		k_node.setGlobalID(k_node_tag);
		Element2D element = Element2D(i_node, j_node, k_node);
		element.setGlobalIDs(i_node_tag, j_node_tag, k_node_tag);
		mesh_elements.push_back(element);
	}

	Mesh<Element2D> mesh(mesh_nodes, mesh_elements);
	return mesh;
}

template <>
Mesh<Element3D> MeshAdapter<Element3D>::adaptJSONMesh() {
	std::cout << "Mesh preprocessing..." << std::endl;
	std::vector<Node> mesh_nodes;
	for (auto point : m_mesh_JSON_data["points"]) {
		Node temp = Node(point[0], point[1], point[2]);
		mesh_nodes.push_back(temp);
	}
	std::vector<Element3D> mesh_elements;
	for (auto raw_element : m_mesh_JSON_data["tetra"]) {
		int i_node_tag = raw_element[0];
		int j_node_tag = raw_element[1];
		int k_node_tag = raw_element[2];
		int l_node_tag = raw_element[3];

		Node i_node = mesh_nodes[i_node_tag];
		i_node.setGlobalID(i_node_tag);
		Node j_node = mesh_nodes[j_node_tag];
		j_node.setGlobalID(j_node_tag);
		Node k_node = mesh_nodes[k_node_tag];
		k_node.setGlobalID(k_node_tag);
		Node l_node = mesh_nodes[l_node_tag];
		l_node.setGlobalID(l_node_tag);
		Element3D element = Element3D(i_node, j_node, k_node, l_node);
		element.setGlobalIDs(i_node_tag, j_node_tag, k_node_tag, l_node_tag);
		mesh_elements.push_back(element);
	}
	Mesh<Element3D> mesh(mesh_nodes, mesh_elements);
	return mesh;
}

template <>
Mesh<Element2D> MeshAdapter<Element2D>::adaptMesh() {
	std::cout << "Mesh preprocessing..." << std::endl;
	std::size_t nodes_number = m_nodes_tags.size();
	std::vector<std::size_t> new_nodes_tags(nodes_number);
	std::iota(new_nodes_tags.begin(), new_nodes_tags.end(), 0);
	std::map<std::size_t, std::size_t> tagsMap;
	std::transform(m_nodes_tags.begin(), m_nodes_tags.end(), new_nodes_tags.begin(),
		std::inserter(tagsMap, tagsMap.end()), std::make_pair<std::size_t const&, std::size_t const&>);

	std::vector<Node> mesh_nodes;
	for (std::size_t i = 2; i < m_coords.size(); i += 3)
	{
		Node temp = Node(m_coords[i - 2], m_coords[i - 1], m_coords[i]);
		mesh_nodes.push_back(temp);
	}
	std::vector<std::size_t> result_elements_tags;
	std::for_each(m_elements_to_nodes_tags.begin(), m_elements_to_nodes_tags.end(), [&](std::vector<std::size_t> element) {
		for (auto node_tag : element) {
			result_elements_tags.push_back(tagsMap[node_tag]);
		}
		});
	std::vector<Element2D> mesh_elements;
	for (std::size_t i = 2; i < result_elements_tags.size(); i += 3) {
		std::size_t i_node_tag = result_elements_tags[i - 2];
		std::size_t j_node_tag = result_elements_tags[i - 1];
		std::size_t k_node_tag = result_elements_tags[i];

		Node i_node = mesh_nodes[i_node_tag];
		i_node.setGlobalID(i_node_tag);
		Node j_node = mesh_nodes[j_node_tag];
		j_node.setGlobalID(j_node_tag);
		Node k_node = mesh_nodes[k_node_tag];
		k_node.setGlobalID(k_node_tag);
		Element2D element = Element2D(i_node, j_node, k_node);
		element.setGlobalIDs(i_node_tag, j_node_tag, k_node_tag);
		mesh_elements.push_back(element);
	}
	Mesh<Element2D> mesh(mesh_nodes, mesh_elements);
	return mesh;
}