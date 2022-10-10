#include "Data.h"

// Point3D

Point3D::Point3D() : m_x(0), m_y(0), m_z(0) {};

Point3D::Point3D(double p_x, double p_y, double p_z) : m_x(p_x), m_y(p_y), m_z(p_z) {};

Point3D::Point3D(const Point3D& p_point) {
	this->m_x = p_point.m_x;
	this->m_y = p_point.m_y;
	this->m_z = p_point.m_z;
}

const Point3D& Point3D::operator=(const Point3D& p_point) {
	if (this == &p_point) {
		return *this;
	}
	this->m_x = p_point.m_x;
	this->m_y = p_point.m_y;
	this->m_z = p_point.m_z;
	return (*this);
}

Point3D& Point3D::operator-(const Point3D& p_point) {
	Point3D l_Res(*this);
	l_Res.m_x -= p_point.m_x;
	l_Res.m_y -= p_point.m_y;
	l_Res.m_z -= p_point.m_z;
	return l_Res;
}

double Point3D::operator*(const Point3D& p_point) {
	return this->m_x * p_point.m_x + this->m_y * p_point.m_y + this->m_z * p_point.m_z;
}

double Point3D::x() const {
	return this->m_x;
}

double Point3D::y() const {
	return this->m_y;
}

double Point3D::z() const {
	return this->m_z;
}

double Point3D::getDistance(const Point3D& p_point) {
	double result = sqrt((this->m_x - p_point.m_x) * (this->m_x - p_point.m_x) 
					   + (this->m_y - p_point.m_y) * (this->m_y - p_point.m_y)
		               + (this->m_z - p_point.m_z) * (this->m_z - p_point.m_z));
	return result;
}

std::ostream& operator << (std::ostream& p_out, const Point3D& p_point) {
	p_out << "[" << p_point.m_x << " " << p_point.m_y << " " << p_point.m_z << "]";
	return p_out;
}


// Node

Node::Node() : Point3D(0, 0, 0), m_global_ID(0) {};

Node::Node(double p_x, double p_y, double p_z) : Point3D(p_x, p_y, p_z), m_global_ID(0) {};

Node::Node(Point3D p_point) : Point3D(p_point), m_global_ID(0) {};

Node::Node(const Node& p_node): Point3D(p_node), m_global_ID(p_node.m_global_ID) {}

const Node& Node::operator=(const Node& p_node) {
	if (this == &p_node) { 
		return *this; 
	}
	Point3D::operator=(p_node);
	this->m_global_ID = p_node.m_global_ID;
	return (*this);
}

Node& Node::operator-(const Node& p_node) {
	Node l_Res = static_cast<Node>(Point3D::operator-(p_node));
	return l_Res;
}

bool Node::operator==(const Node& p_node) const {
	return this->x() == p_node.x() && this->y() == p_node.y() && this->z() == p_node.z();
}

Node::~Node() {}

std::ostream& operator <<(std::ostream& p_out, const Node& p_node) {
	p_out << "[" << p_node.x() << " " << p_node.y() << " " << p_node.z() << "]";
	return p_out;
}

void Node::setGlobalID(std::size_t p_id) {
	this->m_global_ID = p_id;
}


// Element

Element::Element() : m_i_global_ID(0), m_j_global_ID(0), m_k_global_ID(0) {
	this->m_i_node = Node();
	this->m_j_node = Node();
	this->m_k_node = Node();
}

Element::Element(Node p_i_node, Node p_j_node, Node p_k_node) : m_i_node(p_i_node), m_j_node(p_j_node), m_k_node(p_k_node),
m_i_global_ID(0), m_j_global_ID(0), m_k_global_ID(0)
{
	this->m_i_node = p_i_node;
	this->m_j_node = p_j_node;
	this->m_k_node = p_k_node;
};

Element::Element(const Element& p_element) {
	this->m_i_node = p_element.m_i_node;
	this->m_j_node = p_element.m_j_node;
	this->m_k_node = p_element.m_k_node;
	this->m_i_global_ID = p_element.m_i_global_ID;
	this->m_j_global_ID = p_element.m_j_global_ID;
	this->m_k_global_ID = p_element.m_k_global_ID;
}

const Element& Element::operator=(const Element& p_element) {
	if (this == &p_element) {
		return *this;
	}
	this->m_i_node = p_element.m_i_node;
	this->m_j_node = p_element.m_j_node;
	this->m_k_node = p_element.m_k_node;
	this->m_i_global_ID = p_element.m_i_global_ID;
	this->m_j_global_ID = p_element.m_j_global_ID;
	this->m_k_global_ID = p_element.m_k_global_ID;
	return (*this);
}

std::ostream& operator <<(std::ostream& p_out, const Element& p_element) {
	p_out << "[" << p_element.m_i_global_ID << ":" << p_element.iNode() << " "
		<< p_element.m_j_global_ID << ":" << p_element.jNode() << " "
		<< p_element.m_k_global_ID << ":" << p_element.kNode() << "]";
	return p_out;
}

bool Element::operator==(const Element& p_element) const {
	bool i_condition = this->m_i_node == p_element.m_i_node;
	bool j_condition = this->m_j_node == p_element.m_j_node;
	bool k_condition = this->m_k_node == p_element.m_k_node;
	return i_condition && j_condition && k_condition;
 }

void Element::setGlobalIDs(std::size_t p_i_ID, std::size_t p_j_ID, std::size_t p_k_ID) {
	this->m_i_global_ID = p_i_ID;
	this->m_j_global_ID = p_j_ID;
	this->m_k_global_ID = p_k_ID;
}

Node Element::iNode() const {
	return this->m_i_node;
}

Node Element::jNode() const {
	return this->m_j_node;
}

Node Element::kNode() const {
	return this->m_k_node;
}

std::size_t Element::iGlobalID() const {
	return this->m_i_global_ID;
}

std::size_t Element::jGlobalID() const {
	return this->m_j_global_ID;
}

std::size_t Element::kGlobalID() const {
	return this->m_k_global_ID;
}

Point3D Element::getCenter() {
	double x = (this->m_i_node.x() + this->m_j_node.x() + this->m_k_node.x()) / 3;
	double y = (this->m_i_node.y() + this->m_j_node.y() + this->m_k_node.y()) / 3;
	double z = (this->m_i_node.z() + this->m_j_node.z() + this->m_k_node.z()) / 3;
	Point3D res(x, y, z);
	return res;
}

double Element::getSquare() {
	double a = this->m_i_node.getDistance(this->m_j_node);
	double b = this->m_j_node.getDistance(this->m_k_node);
	double c = this->m_k_node.getDistance(this->m_i_node);
	double p = (a + b + c) / 2;
	double s = sqrt(p * (p - a) * (p - b) * (p - c));
	return s;
}

Point3D Element::getNormal() {
	Point3D v1 = this->m_j_node - this->m_i_node;
	Point3D v2 = this->m_k_node - this->m_i_node;
	double nx = v1.y() * v2.z() - v2.y() * v1.z();
	double ny = -(v1.x() * v2.z() - v2.x() * v1.z());
	double nz = v1.x() * v2.y() - v2.x() * v1.y();
	double d = sqrt(nx * nx + ny * ny + nz * nz);
	nx /= d;
	ny /= d;
	nz /= d;
	Point3D res(nx, ny, nz);
	return res;
}

Element::~Element() {}


// Mesh

Mesh::Mesh(std::vector<Node> p_nodes, std::vector<Element> p_elements) {
	std::copy(p_nodes.begin(), p_nodes.end(), std::back_inserter(m_nodes));
	std::copy(p_elements.begin(), p_elements.end(), std::back_inserter(m_elements));
}

Mesh::Mesh(const Mesh& p_mesh) {
	std::copy(p_mesh.m_nodes.begin(), p_mesh.m_nodes.end(), std::back_inserter(m_nodes));
	std::copy(p_mesh.m_elements.begin(), p_mesh.m_elements.end(), std::back_inserter(m_elements));
}

const Mesh& Mesh::operator=(const Mesh& p_mesh) {
	if (this == &p_mesh) {
		return (*this);
	}
	std::copy(p_mesh.m_nodes.begin(), p_mesh.m_nodes.end(), std::back_inserter(m_nodes));
	std::copy(p_mesh.m_elements.begin(), p_mesh.m_elements.end(), std::back_inserter(m_elements));
	return (*this);
}

std::vector<Node> Mesh::nodes() const {
	return this->m_nodes;
}

std::vector<Element> Mesh::elements() const {
	return this->m_elements;
}