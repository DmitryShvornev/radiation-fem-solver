#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <iterator>

class Point3D {
public:
	Point3D();
	Point3D(double p_x, double p_y, double p_z);
	double getDistance(const Point3D& p_point);
	~Point3D() {};
private:
	double x, y, z;
};

class Node : public Point3D { 
public:
	Node();
	Node(double p_x, double p_y, double p_z);
	Node(const Node& p_node);
	const Node& operator=(const Node& p_node);
	friend std::ostream& operator<<(std::ostream&, const Node&);
	void setGlobalID(std::size_t p_id);
	double* getCoords() const;
	double getDistance(const Node& p_point);
	~Node();

private:
	double x, y, z;
	std::size_t globalID;
	double* coords;
};

class Element {
public:
	Element();
	Element(Node p_i_node, Node p_j_node, Node p_k_node);
	Element(const Element& p_element);
	const Element& operator=(const Element& p_node);
	friend std::ostream& operator<<(std::ostream&, const Element&);
	bool operator ==(const Element& p_element) const;
	Point3D getCenter();
	double getSquare();
	Point3D getNormal();
	void setGlobalIDs(std::size_t i, std::size_t j, std::size_t k);
	~Element();

private:
	Node i_node, j_node, k_node;
	std::vector<std::size_t> globalIDs{0, 0, 0};
	double* i_coords;
	double* j_coords;
	double* k_coords;
};

class Mesh {
private:
	std::vector<Node> nodes;
	std::vector<Element> elements;
public:
	Mesh(std::vector<Node> p_nodes, std::vector<Element> p_elements);
	std::vector<Node> getNodes();
	std::vector<Element> getElements();
	~Mesh() {};
};
