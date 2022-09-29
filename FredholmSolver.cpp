#include "FredholmSolver.h"

FredholmSolver::FredholmSolver(const std::vector<double> p_coords) {
    for (int i = 2; i < p_coords.size(); i += 3)
    {
        Node temp = Node(p_coords[i - 2], p_coords[i - 1], p_coords[i]);
        mesh_nodes.push_back(temp);
    }
}

std::vector<Node> FredholmSolver::getNodes() {
    return mesh_nodes;
}