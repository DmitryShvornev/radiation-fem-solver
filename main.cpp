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

    gmsh::finalize();
    MeshAdapter adapter(coords, tags, node_tags);
    Mesh msh = adapter.adaptMesh();
    FredholmSolver solver(msh);
    solver.assembleGlobalSystem();
    solver.solveGlobalSystem();
    solver.printToMV2();
}

