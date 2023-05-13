// RadiationFEMSolver.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "Application.h"


int main()
{
    std::string filename1 = "all_data1.json";
    std::string suffix1 = "1";
    Application app1(filename1, suffix1);
    app1.run();
    /*std::string filename2 = "all_data2.json";
    std::string suffix2 = "2";
    Application app2(filename2, suffix2);
    app2.run();
    std::string filename3 = "all_data3.json";
    std::string suffix3 = "3";
    Application app3(filename3, suffix3);
    app3.run();*/
}

