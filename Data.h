#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <iterator>

class Point3D {
public:
	double x, y, z;
	Point3D();
	Point3D(double p_x, double p_y, double p_z);
	Point3D(const Point3D& p_point);
	Point3D& operator-(const Point3D& p_point);
	double operator*(const Point3D& p_point);
	double getDistance(const Point3D& p_point);
	~Point3D() {};
};

class Node : public Point3D { 
public:
	double x, y, z;
	std::size_t globalID;
	Node();
	Node(double p_x, double p_y, double p_z);
	Node(const Node& p_node);
	const Node& operator=(const Node& p_node);
	friend std::ostream& operator<<(std::ostream&, const Node&);
	double getDistance(const Node& p_point);
	~Node();
};

class Element {
public:
	Node i_node, j_node, k_node;
	std::size_t i_globalID, j_globalID, k_globalID;
	Element();
	Element(Node p_i_node, Node p_j_node, Node p_k_node);
	Element(const Element& p_element);
	const Element& operator=(const Element& p_node);
	friend std::ostream& operator<<(std::ostream&, const Element&);
	bool operator ==(const Element& p_element) const;
	void setGlobalIDs(std::size_t i, std::size_t j, std::size_t k);
	Point3D getCenter();
	double getSquare();
	Point3D getNormal();
	~Element();
};

class Mesh {
public:
	Mesh() {};
	Mesh(std::vector<Node> p_nodes, std::vector<Element> p_elements);
	Mesh(const Mesh& p_mesh);
	const Mesh& operator=(const Mesh& p_mesh);
	std::vector<Node> nodes;
	std::vector<Element> elements;
	~Mesh() {};
};
