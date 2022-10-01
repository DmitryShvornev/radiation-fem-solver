#include "Data.h"

Point3D::Point3D() : x(0), y(0), z(0) {};

Point3D::Point3D(double p_x, double p_y, double p_z) : x(p_x), y(p_y), z(p_z) {};

Point3D::Point3D(const Point3D& p_point) {
	x = p_point.x;
	y = p_point.y;
	z = p_point.z;
}

Point3D& Point3D::operator-(const Point3D& p_point) {
	Point3D res(*this);
	res.x -= p_point.x;
	res.y -= p_point.y;
	res.z -= p_point.z;
	return res;
}

double Point3D::operator*(const Point3D& p_point) {
	return x * p_point.x + y * p_point.y + z * p_point.z;
}

double Point3D::getDistance(const Point3D& p_point) {
	double result = sqrt((x - p_point.x) * (x - p_point.x) + (y - p_point.y) * (y - p_point.y) + (z - p_point.z) * (z - p_point.z));
	return result;
}

std::ostream& operator << (std::ostream& out_stream, const Point3D& point) {
	out_stream << "[" << point.x << " " << point.y << " " << point.z << "]";
	return out_stream;
}

Node::Node(double p_x, double p_y, double p_z) : x(p_x), y(p_y), z(p_z), globalID(0) {};


double Node::getDistance(const Node& p_point) {
	double result = sqrt((x - p_point.x) * (x - p_point.x) + (y - p_point.y) * (y - p_point.y) + (z - p_point.z) * (z - p_point.z));
	return result;
}

Node::Node() : x(0), y(0), z(0), globalID(0) {};

Node::Node(const Node& p_node) {
	x = p_node.x;
	y = p_node.y;
	z = p_node.z;
	globalID = p_node.globalID;
}

const Node& Node::operator=(const Node& p_node) {
	x = p_node.x;
	y = p_node.y;
	z = p_node.z;
	return (*this);
}

Node::~Node() {}

std::ostream& operator <<(std::ostream& out_stream, const Node& node) {
	out_stream << "[" << node.x << " " << node.y << " " << node.z << "]";
	return out_stream;
}


Element::Element() : i_globalID(0), j_globalID(0), k_globalID(0) {
	i_node = Node();
	j_node = Node();
	k_node = Node();
}

Element::Element(Node p_i_node, Node p_j_node, Node p_k_node): i_node(p_i_node), j_node(p_j_node), k_node(p_k_node), 
i_globalID(0), j_globalID(0), k_globalID(0) 
{
	i_node = p_i_node;
	j_node = p_j_node;
	k_node = p_k_node;
};

Element::Element(const Element& p_element) {
	i_node = p_element.i_node;
	j_node = p_element.j_node;
	k_node = p_element.k_node;
	i_globalID = p_element.i_globalID;
	j_globalID = p_element.j_globalID;
	k_globalID = p_element.k_globalID;
}

const Element& Element::operator=(const Element& p_element) {
	i_node = p_element.i_node;
	j_node = p_element.j_node;
	k_node = p_element.k_node;
	i_globalID = p_element.i_globalID;
	j_globalID = p_element.j_globalID;
	k_globalID = p_element.k_globalID;
	return (*this);
}

std::ostream& operator <<(std::ostream& out_stream, const Element& element) {
	out_stream << "[" << element.i_globalID << ":" <<  element.i_node << " " << element.j_globalID << ":" << element.j_node << " " << element.k_globalID << ":" << element.k_node << "]";
	return out_stream;
}

bool Element::operator==(const Element& p_element) const {
	bool x_condition = i_node.x == p_element.i_node.x && j_node.x == p_element.j_node.x && k_node.x == p_element.k_node.x;
	bool y_condition = i_node.y == p_element.i_node.y && j_node.y == p_element.j_node.y && k_node.y == p_element.k_node.y;
	bool z_condition = i_node.z == p_element.i_node.z && j_node.z == p_element.j_node.z && k_node.z == p_element.k_node.z;
	return x_condition && y_condition && z_condition;
 }

void Element::setGlobalIDs(std::size_t i, std::size_t j, std::size_t k) {
	i_globalID = i;
	j_globalID = j;
	k_globalID = k;
}

Point3D Element::getCenter() {
	double x = (i_node.x + j_node.x + k_node.x) / 3;
	double y = (i_node.y + j_node.y + k_node.y) / 3;
	double z = (i_node.z + j_node.z + k_node.z) / 3;
	Point3D res(x, y, z);
	return res;
}

double Element::getSquare() {
	double a = i_node.getDistance(j_node);
	double b = j_node.getDistance(k_node);
	double c = k_node.getDistance(i_node);
	double p = (a + b + c) / 2;
	double s = sqrt(p * (p - a) * (p - b) * (p - c));
	return s;
}

Point3D Element::getNormal() {
	double nx = (j_node.y - i_node.y) * (k_node.z - i_node.z) - (j_node.z - i_node.z) * (k_node.y - i_node.y);
	double ny = (j_node.z - i_node.z) * (k_node.x - i_node.x) - (j_node.x - i_node.x) * (j_node.z - i_node.z);
	double nz = (j_node.x - i_node.x) * (k_node.y - i_node.y) - (j_node.y - i_node.y) * (k_node.x - i_node.x);
	Point3D res(nx, ny, nz);
	return res;
}

Element::~Element() {}

Mesh::Mesh(std::vector<Node> p_nodes, std::vector<Element> p_elements) {
	std::copy(p_nodes.begin(), p_nodes.end(), std::back_inserter(nodes));
	std::copy(p_elements.begin(), p_elements.end(), std::back_inserter(elements));
}

Mesh::Mesh(const Mesh& p_mesh) {
	std::copy(p_mesh.nodes.begin(), p_mesh.nodes.end(), std::back_inserter(nodes));
	std::copy(p_mesh.elements.begin(), p_mesh.elements.end(), std::back_inserter(elements));
}

const Mesh& Mesh::operator=(const Mesh& p_mesh) {
	std::copy(p_mesh.nodes.begin(), p_mesh.nodes.end(), std::back_inserter(nodes));
	std::copy(p_mesh.elements.begin(), p_mesh.elements.end(), std::back_inserter(elements));
	return (*this);
}