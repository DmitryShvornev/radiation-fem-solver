#include "Application.h"

Application::Application(std::string p_filename, std::string p_suffix, bool p_is_with_radiation) {
	this->m_filename = p_filename;
    this->m_suffix = p_suffix;
    this->m_is_with_radiation = p_is_with_radiation;
}

void Application::run() {
    std::ifstream data_file(m_filename);
    json data = json::parse(data_file);
    data_file.close();
    MeshAdapter<Element2D> adapter(data);
    Mesh<Element2D> msh = adapter.adaptJSONMesh();
    FredholmSolver solver(msh, 0.7, this->m_suffix);
    double start = omp_get_wtime();
    solver.assembleGlobalSystem();
    solver.solveGlobalSystem();
    double end = omp_get_wtime();
    std::cout << "Elapsed Fredholm time: " << end - start << std::endl;
    solver.printToMV2();
    MeshAdapter<Element3D> adapter_3d(data);
    Mesh<Element3D> msh_3d = adapter_3d.adaptJSONMesh();
    ConductivitySolver solver_3d(msh_3d, data, solver.solution(), this->m_suffix, this->m_is_with_radiation);
    solver_3d.init();
    start = omp_get_wtime();
    solver_3d.assembleGlobalSystem();
    solver_3d.solveGlobalSystem();
    end = omp_get_wtime();
    std::cout << "Elapsed Conductivity time: " << end - start << std::endl;
    solver_3d.printToMV2();
}