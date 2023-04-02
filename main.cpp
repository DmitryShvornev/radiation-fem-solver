// RadiationFEMSolver.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <algorithm>
#include "FredholmSolver.h"
#include "MeshAdapter.h"
#include "Data.h"

using json = nlohmann::json;


int main()
{
    std::ifstream data_file("all_data.json");
    json data = json::parse(data_file);
    MeshAdapter<Element2D> adapter(data);
    Mesh<Element2D> msh = adapter.adaptJSONMesh();
    FredholmSolver solver(msh, 0.7);
    double start = omp_get_wtime();
    solver.assembleGlobalSystem();
    solver.solveGlobalSystem();
    double end = omp_get_wtime();
    std::cout << "Elapsed time: " << end - start << std::endl;
    solver.printToMV2();
}

