#pragma once
#include <iostream>
#include <cmath>

class Node {
public:
	Node(double p_x, double p_y, double p_z);
	Node(const Node& p_node);
	friend std::ostream& operator<<(std::ostream&, const Node&);
	double getDistance(const Node& p_node);
	double getCoords();
	~Node() {};

private:
	double x, y, z;
};

class Element {
public:
	Element(Node p_i_node, Node p_j_node, Node p_k_node);
	Element(const Element& p_element);
	friend std::ostream& operator<<(std::ostream&, const Element&);
	bool operator ==(const Element& p_element) const;
	double getCenter();
	double getSquare();
	Node getNormal();
	~Element();

private:
	Node i_node, j_node, z_node;
};