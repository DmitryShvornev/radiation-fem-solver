// RadiationFEMSolver.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <gmsh.h>
#include <algorithm>
#include "FredholmSolver.h"
#include "MeshAdapter.h"
#include "Data.h"


int main()
{
    gmsh::initialize();
    gmsh::open("mesh_final.msh");
    std::vector<double> coords;
    std::vector<double> param_coords;
    std::vector<std::size_t> tags;
    gmsh::model::mesh::getNodes(tags, coords, param_coords);

    std::vector<int> element_types;
    gmsh::model::mesh::getElementTypes(element_types);

    std::vector<std::vector<std::size_t>> element_tags;
    std::vector<std::vector<std::size_t>> node_tags;
    gmsh::model::mesh::getElements(element_types, element_tags, node_tags);

   
    FredholmSolver fr(coords);
    gmsh::finalize();
    MeshAdapter adapter(coords, tags, node_tags);
    adapter.adaptMesh();
    //std::vector<Node> nodes = fr.getNodes();
    /*std::for_each(node_tags.begin(), node_tags.end(), [](std::vector<std::size_t> elem) {
        std::for_each(elem.begin(), elem.end(), [](std::size_t x) {
            std::cout << x << " ";
            });*/
    
}

