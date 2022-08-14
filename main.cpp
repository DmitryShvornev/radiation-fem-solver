// RadiationFEMSolver.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <gmsh.h>
#include <algorithm>

void print(double elem) {
    std::cout << elem << ' ';
}

int main()
{
    gmsh::initialize();
    gmsh::open("mesh_final.msh");
    std::vector<double> coords;
    std::vector<double> param_coords;
    std::vector<std::size_t> tags;
    gmsh::model::mesh::getNodes(tags, coords, param_coords);
    std::for_each(coords.begin(), coords.end(), print);
    gmsh::finalize();
}

