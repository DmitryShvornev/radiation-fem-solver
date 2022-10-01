#include "MeshAdapter.h"

MeshAdapter::MeshAdapter(const std::vector<double> p_coords, const std::vector<std::size_t> p_nodes_tags, const std::vector<std::vector<std::size_t>> p_elements_to_nodes_tags) {
	std::copy(p_coords.begin(), p_coords.end(), std::back_inserter(coords));
	std::copy(p_nodes_tags.begin(), p_nodes_tags.end(),
		std::back_inserter(nodes_tags));
	std::copy(p_elements_to_nodes_tags.begin(), p_elements_to_nodes_tags.end(),
		std::back_inserter(elements_to_nodes_tags));
};

Mesh MeshAdapter::adaptMesh() {
	std::size_t nodes_number = nodes_tags.size();
	std::vector<std::size_t> new_nodes_tags(nodes_number);
	std::iota(new_nodes_tags.begin(), new_nodes_tags.end(), 0);
	std::map<std::size_t, std::size_t> tagsMap;
	std::transform(nodes_tags.begin(), nodes_tags.end(), new_nodes_tags.begin(),
		std::inserter(tagsMap, tagsMap.end()), std::make_pair<std::size_t const&, std::size_t const&>);

	std::vector<Node> mesh_nodes;
	for (std::size_t i = 2; i < coords.size(); i += 3)
	{
		Node temp = Node(coords[i - 2], coords[i - 1], coords[i]);
		mesh_nodes.push_back(temp);
	}
	std::vector<std::size_t> result_elements_tags;
	std::for_each(elements_to_nodes_tags.begin(), elements_to_nodes_tags.end(), [&](std::vector<std::size_t> element) {
		for (auto node_tag : element) {
			result_elements_tags.push_back(tagsMap[node_tag]);
		}
		});
	std::vector<Element> mesh_elements;
	for (std::size_t i = 2; i < result_elements_tags.size(); i += 3) {
		std::size_t i_node_tag = result_elements_tags[i - 2];
		std::size_t j_node_tag = result_elements_tags[i - 1];
		std::size_t k_node_tag = result_elements_tags[i];

		Node i_node = mesh_nodes[i_node_tag];
		i_node.globalID = i_node_tag;
		Node j_node = mesh_nodes[j_node_tag];
		j_node.globalID = j_node_tag;
		Node k_node = mesh_nodes[k_node_tag];
		k_node.globalID = k_node_tag;
		Element element = Element(i_node, j_node, k_node);
		element.setGlobalIDs(i_node_tag, j_node_tag, k_node_tag);
		//std::cout << element.getCenter() << std::endl;
		mesh_elements.push_back(element);
	}
	Mesh mesh(mesh_nodes, mesh_elements);
	return mesh;
}