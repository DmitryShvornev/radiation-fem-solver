#include "Data.h"

Node::Node(double p_x, double p_y, double p_z) : x(p_x), y(p_y), z(p_z) {
	coords = new double[3];
	coords[0] = p_x; coords[1] = p_y; coords[2] = p_z;
};

double* Node::getCoords() const {
	return coords;
}

Node::Node() : x(0), y(0), z(0), coords(nullptr) {};

Node::Node(const Node& p_node) {
	x = p_node.x;
	y = p_node.y;
	z = p_node.z;
	coords = new double[3];
	coords[0] = x; coords[1] = y; coords[2] = z;
}

const Node& Node::operator=(const Node& p_node) {
	x = p_node.x;
	y = p_node.y;
	z = p_node.z;
	coords[0] = x; coords[1] = y; coords[2] = z;
	return (*this);
}

double Node::getDistance(const Node& p_node) {
	double result = sqrt((x - p_node.x) * (x - p_node.x) + (y - p_node.y) * (y - p_node.y) + (z - p_node.z) * (z - p_node.z));
	return result;
}

Node::~Node() {
	delete[] coords;
}

std::ostream& operator <<(std::ostream& out_stream, const Node& node) {
	out_stream << "[" << node.x << " " << node.y << " " << node.z << "]";
	return out_stream;
}

Element::Element(Node p_i_node, Node p_j_node, Node p_k_node) {
	i_node = Node(p_i_node);
	j_node = Node(p_j_node);
	k_node = Node(p_k_node);
	i_coords = new double[3];
	j_coords = new double[3];
	k_coords = new double[3];
	for (int i = 0; i < 3; i++)
	{
		i_coords[i] = p_i_node.getCoords()[i];
		j_coords[i] = p_j_node.getCoords()[i];
		k_coords[i] = p_k_node.getCoords()[i];
	}
};

Element::Element(const Element& p_element) {
	i_node = p_element.i_node;
	j_node = p_element.j_node;
	k_node = p_element.k_node;
	i_coords = new double[3];
	j_coords = new double[3];
	k_coords = new double[3];
	for (int i = 0; i < 3; i++)
	{
		i_coords[i] = p_element.i_node.getCoords()[i];
		j_coords[i] = p_element.j_node.getCoords()[i];
		k_coords[i] = p_element.k_node.getCoords()[i];
	}
}

const Element& Element::operator=(const Element& p_element) {
	i_node = p_element.i_node;
	j_node = p_element.j_node;
	k_node = p_element.k_node;
	for (int i = 0; i < 3; i++)
	{
		i_coords[i] = p_element.i_node.getCoords()[i];
		j_coords[i] = p_element.j_node.getCoords()[i];
		k_coords[i] = p_element.k_node.getCoords()[i];
	}
	return (*this);
}

std::ostream& operator <<(std::ostream& out_stream, const Element& element) {
	out_stream << "[" << element.i_node << " " << element.j_node << " " << element.k_node << "]";
	return out_stream;
}

bool Element::operator==(const Element& p_element) const {
	bool flag = true;
	for (int i = 0; i < 3; i++)
	{
		if (!((i_coords[i] == p_element.i_coords[i]) && (j_coords[i] == p_element.j_coords[i]) && (k_coords[i] == p_element.k_coords[i]))) {
			flag = false;
		}
	}
	return flag;
 }

Node Element::getCenter() {
	double x = (i_coords[0] + j_coords[0] + k_coords[0]) / 3;
	double y = (i_coords[1] + j_coords[1] + k_coords[1]) / 3;
	double z = (i_coords[2] + j_coords[2] + k_coords[2]) / 3;
	Node res(x, y, z);
	return res;
}

double Element::getSquare() {
	return abs((j_coords[0] - i_coords[0]) * (k_coords[1] - i_coords[1]) - (k_coords[0] - i_coords[0]) * (j_coords[1] - i_coords[1])) / 2;
}

Node Element::getNormal() {
	double nx = (j_coords[1] - i_coords[1]) * (k_coords[2] - i_coords[2]) - (j_coords[2] - i_coords[2]) * (k_coords[1] - i_coords[1]);
	double ny = (j_coords[2] - i_coords[2]) * (k_coords[0] - i_coords[0]) - (j_coords[0] - i_coords[0]) * (j_coords[2] - i_coords[2]);
	double nz = (j_coords[0] - i_coords[0]) * (k_coords[1] - i_coords[1]) - (j_coords[1] - i_coords[1]) * (k_coords[0] - i_coords[0]);
	Node res(nx, ny, nz);
	return res;
}

Element::~Element() {
	delete[] i_coords;
	delete[] j_coords;
	delete[] k_coords;
}