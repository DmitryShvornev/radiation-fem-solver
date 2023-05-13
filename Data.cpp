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


// Element2D

Element2D::Element2D() : m_i_global_ID(0), m_j_global_ID(0), m_k_global_ID(0) {
	this->m_i_node = Node();
	this->m_j_node = Node();
	this->m_k_node = Node();
}

Element2D::Element2D(Node p_i_node, Node p_j_node, Node p_k_node) : m_i_node(p_i_node), m_j_node(p_j_node), m_k_node(p_k_node),
m_i_global_ID(0), m_j_global_ID(0), m_k_global_ID(0)
{
	this->m_i_node = p_i_node;
	this->m_j_node = p_j_node;
	this->m_k_node = p_k_node;
};

Element2D::Element2D(const Element2D& p_element) {
	this->m_i_node = p_element.m_i_node;
	this->m_j_node = p_element.m_j_node;
	this->m_k_node = p_element.m_k_node;
	this->m_i_global_ID = p_element.m_i_global_ID;
	this->m_j_global_ID = p_element.m_j_global_ID;
	this->m_k_global_ID = p_element.m_k_global_ID;
}

const Element2D& Element2D::operator=(const Element2D& p_element) {
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

std::ostream& operator <<(std::ostream& p_out, const Element2D& p_element) {
	p_out << "[" << p_element.m_i_global_ID << ":" << p_element.iNode() << " "
		<< p_element.m_j_global_ID << ":" << p_element.jNode() << " "
		<< p_element.m_k_global_ID << ":" << p_element.kNode() << "]";
	return p_out;
}

bool Element2D::operator==(const Element2D& p_element) const {
	bool i_condition = this->m_i_node == p_element.m_i_node;
	bool j_condition = this->m_j_node == p_element.m_j_node;
	bool k_condition = this->m_k_node == p_element.m_k_node;
	return i_condition && j_condition && k_condition;
 }

void Element2D::setGlobalIDs(std::size_t p_i_ID, std::size_t p_j_ID, std::size_t p_k_ID) {
	this->m_i_global_ID = p_i_ID;
	this->m_j_global_ID = p_j_ID;
	this->m_k_global_ID = p_k_ID;
}

Node Element2D::iNode() const {
	return this->m_i_node;
}

Node Element2D::jNode() const {
	return this->m_j_node;
}

Node Element2D::kNode() const {
	return this->m_k_node;
}

std::size_t Element2D::iGlobalID() const {
	return this->m_i_global_ID;
}

std::size_t Element2D::jGlobalID() const {
	return this->m_j_global_ID;
}

std::size_t Element2D::kGlobalID() const {
	return this->m_k_global_ID;
}

Point3D Element2D::getCenter() {
	double x = (this->m_i_node.x() + this->m_j_node.x() + this->m_k_node.x()) / 3;
	double y = (this->m_i_node.y() + this->m_j_node.y() + this->m_k_node.y()) / 3;
	double z = (this->m_i_node.z() + this->m_j_node.z() + this->m_k_node.z()) / 3;
	Point3D res(x, y, z);
	return res;
}

double Element2D::getSquare() {
	double a = this->m_i_node.getDistance(this->m_j_node);
	double b = this->m_j_node.getDistance(this->m_k_node);
	double c = this->m_k_node.getDistance(this->m_i_node);
	double p = (a + b + c) / 2;
	double s = sqrt(p * (p - a) * (p - b) * (p - c));
	if (isnan(s) || isinf(s)) {
		return 0;
	}
	return s;
}

Point3D Element2D::getNormal() {
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


// Element3D

Element3D::Element3D() : Element2D(), m_l_global_ID(0) {};

Element3D::Element3D(Node p_i_node, Node p_j_node, Node p_k_node, Node p_l_node) : Element2D(p_i_node, p_j_node, p_k_node),
m_l_node(p_l_node), m_l_global_ID(0) 
{
	this->m_l_node = p_l_node;
}

Element3D::Element3D(const Element3D& p_element) : Element2D(p_element),
m_l_node(p_element.m_l_node), m_l_global_ID(p_element.m_l_global_ID) {}

const Element3D& Element3D::operator=(const Element3D& p_element) {
	if (this == &p_element) {
		return (*this);
	}
	Element2D::operator=(p_element);
	this->m_l_node = p_element.m_l_node;
	this->m_l_global_ID = p_element.m_l_global_ID;
	return (*this);
}

Node Element3D::lNode() const {
	return this->m_l_node;
}

std::size_t Element3D::lGlobalID() const {
	return this->m_l_global_ID;
}

std::ostream& operator <<(std::ostream& p_out, const Element3D& p_element) {
	p_out << "[" << p_element.iGlobalID() << ":" << p_element.iNode() << " "
		<< p_element.jGlobalID() << ":" << p_element.jNode() << " "
		<< p_element.kGlobalID() << ":" << p_element.kNode() << ""
		<< p_element.lGlobalID() << ":" << p_element.lNode() << "";
	return p_out;
}

bool Element3D::operator==(const Element3D& p_element) const {
	return Element2D::operator==(p_element) && p_element.m_l_node == this->m_l_node;
}

void Element3D::setGlobalIDs(std::size_t p_i_ID, std::size_t p_j_ID, std::size_t p_k_ID, std::size_t p_l_ID) {
	Element2D::setGlobalIDs(p_i_ID, p_j_ID, p_k_ID);
	this->m_l_global_ID = p_l_ID;
}

double Element3D::getVolume() {
	matrix<double> C(4, 4);
	std::vector<Node> element_nodes({ this->iNode(), this->jNode(), this->kNode(), this->lNode() });
	C(0, 0) = 1; C(0, 1) = 1; C(0, 2) = 1; C(0, 3) = 1;
	for (int i = 0; i < 4; i++)
	{
		C(1, i) = element_nodes[i].x();
		C(2, i) = element_nodes[i].y();
		C(3, i) = element_nodes[i].z();
	}
	double res = fabs(Determinant(C) / 6);
	if (isnan(res) || isinf(res)) {
		return 0;
	}
	return res;
}

matrix<double> Element3D::getBaricentricCoords() {
	matrix<double> C(4, 4);
	matrix<double> C_res(4, 4);
	std::vector<Node> element_nodes({ this->iNode(), this->jNode(), this->kNode(), this->lNode()});
	C(0, 0) = 1; C(0, 1) = 1; C(0, 2) = 1; C(0, 3) = 1;
	for (int i = 0; i < 4; i++)
	{
		C(1, i) = element_nodes[i].x();
		C(2, i) = element_nodes[i].y();
		C(3, i) = element_nodes[i].z();
	}
	InvertMatrix(C, C_res);
	return C_res;
}

// Mesh

template Mesh<Element2D>; // инстанцируем класс Mesh для типа Element2D (Inclusion Model)
template Mesh<Element3D>;

template <class __ElementType>
Mesh<__ElementType>::Mesh(std::vector<Node> p_nodes, std::vector<__ElementType> p_elements) {
	std::copy(p_nodes.begin(), p_nodes.end(), std::back_inserter(m_nodes));
	std::copy(p_elements.begin(), p_elements.end(), std::back_inserter(m_elements));
}

template <class __ElementType>
Mesh<__ElementType>::Mesh(const Mesh<__ElementType>& p_mesh) {
	std::copy(p_mesh.m_nodes.begin(), p_mesh.m_nodes.end(), std::back_inserter(m_nodes));
	std::copy(p_mesh.m_elements.begin(), p_mesh.m_elements.end(), std::back_inserter(m_elements));
}

template <class __ElementType>
const Mesh<__ElementType>& Mesh<__ElementType>::operator=(const Mesh<__ElementType>& p_mesh) {
	if (this == &p_mesh) {
		return (*this);
	}
	std::copy(p_mesh.m_nodes.begin(), p_mesh.m_nodes.end(), std::back_inserter(m_nodes));
	std::copy(p_mesh.m_elements.begin(), p_mesh.m_elements.end(), std::back_inserter(m_elements));
	return (*this);
}

template <class __ElementType>
std::vector<Node> Mesh<__ElementType>::nodes() const {
	return this->m_nodes;
}

template <class __ElementType>
std::vector<__ElementType> Mesh<__ElementType>::elements() const {
	return this->m_elements;
}