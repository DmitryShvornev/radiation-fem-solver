#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <iterator>
#include <boost/numeric/ublas/matrix.hpp>
#include "MatrixOperations.h"

using namespace boost::numeric::ublas;

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
	Node& operator-(const Node& p_node);
	bool operator ==(const Node& p_node) const;
	friend std::ostream& operator<<(std::ostream&, Node&);
	void setGlobalID(std::size_t p_id);
	~Node();
};

class Element2D {
private:
	Node m_i_node, m_j_node, m_k_node;
	std::size_t m_i_global_ID, m_j_global_ID, m_k_global_ID;
public:
	Element2D();
	Element2D(Node p_i_node, Node p_j_node, Node p_k_node);
	Element2D(const Element2D& p_element);
	const Element2D& operator=(const Element2D& p_element);
	friend std::ostream& operator<<(std::ostream&, const Element2D&);
	bool operator ==(const Element2D& p_element) const;
	void setGlobalIDs(std::size_t p_i_ID, std::size_t p_j_ID, std::size_t p_k_ID);
	virtual Node iNode() const;
	virtual Node jNode() const;
	virtual Node kNode() const;
	virtual std::size_t iGlobalID() const;
	virtual std::size_t jGlobalID() const;
	virtual std::size_t kGlobalID() const;
	Point3D getCenter();
	double getSquare();
	Point3D getNormal();
	~Element2D() {};
};

class Element3D : public Element2D {
private:
	Node m_l_node;
	std::size_t m_l_global_ID;
public:
	Element3D();
	Element3D(Node p_i_node, Node p_j_node, Node p_k_node, Node p_l_node);
	Element3D(const Element3D& p_element);
	const Element3D& operator=(const Element3D& p_element);
	friend std::ostream& operator<<(std::ostream&, const Element3D&);
	bool operator ==(const Element3D& p_element) const;
	void setGlobalIDs(std::size_t p_i_ID, std::size_t p_j_ID, std::size_t p_k_ID, std::size_t p_l_ID);
	Node lNode() const;
	std::size_t lGlobalID() const;
	double getVolume();
	matrix<double> getBaricentricCoords();
	~Element3D() {};
};

template <class __ElementType>
class Mesh {
private:
	std::vector<Node> m_nodes;
	std::vector<__ElementType> m_elements;
public:
	Mesh() {};
	Mesh(std::vector<Node> p_nodes, std::vector<__ElementType> p_elements);
	Mesh(const Mesh& p_mesh);
	const Mesh& operator=(const Mesh& p_mesh);
	std::vector<Node> nodes() const;
	std::vector<__ElementType> elements() const;
	~Mesh() {};
};
