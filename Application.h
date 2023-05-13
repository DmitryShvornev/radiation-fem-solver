#pragma once
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <algorithm>
#include "FredholmSolver.h"
#include "ConductivitySolver.h"
#include "MeshAdapter.h"
#include "Data.h"

using json = nlohmann::json;

class Application {
	std::string m_filename;
	std::string m_suffix;
	bool m_is_with_radiation;
public:
	Application(std::string p_filename, std::string p_suffix, bool p_is_with_radiation);
	void run();
};