#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <iterator>

class Point3D {
private:
	double m_x, m_y, m_z;
public:
	Point3D();
	Point3D(double p_x, double p_y, double p_z);
	Point3D(const Point3D& p_point);
	const Point3D& operator=(const Point3D& p_point);
	Point3D& operator-(const Point3D& p_point);
	double operator*(const Point3D& p_point);
	virtual double x() const;
	virtual double y() const;
	virtual double z() const;
	virtual double getDistance(const Point3D& p_point);
	friend std::ostream& operator<<(std::ostream&, const Point3D&);
	~Point3D() {};
};

class Node : public Point3D { 
private:
	std::size_t m_global_ID;
public:
	Node();
	Node(double p_x, double p_y, double p_z);
	Node(Point3D p_point);
	Node(const Node& p_node);
	const Node& operator=(const Node& p_node);
	Node& operator-(const Node& p_point);
	friend std::ostream& operator<<(std::ostream&, Node&);
	void setGlobalID(std::size_t p_id);
	~Node();
};

class Element {
private:
	Node m_i_node, m_j_node, m_k_node;
	std::size_t m_i_global_ID, m_j_global_ID, m_k_global_ID;
public:
	Element();
	Element(Node p_i_node, Node p_j_node, Node p_k_node);
	Element(const Element& p_element);
	const Element& operator=(const Element& p_element);
	friend std::ostream& operator<<(std::ostream&, const Element&);
	bool operator ==(const Element& p_element) const;
	void setGlobalIDs(std::size_t p_i_ID, std::size_t p_j_ID, std::size_t p_k_ID);
	Node iNode() const;
	Node jNode() const;
	Node kNode() const;
	std::size_t iGlobalID() const;
	std::size_t jGlobalID() const;
	std::size_t kGlobalID() const;
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
