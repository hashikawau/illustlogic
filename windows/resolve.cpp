#include <iostream>
#include <memory>

#include "IllustrationLogicSolver.h"

int main(int argc, char** argv)
{
    //std::string line;
    //while (std::getline(std::cin, line)) {
    //    std::cout << line << std::endl;
    //}

    auto logic = std::make_unique<IllustrationLogicCanvas>();
    logic->setNumGrid();
    for (int k = 0; k < 500; ++k)
        for (int i = 0; i < logic->getNumVer(); ++i)
            for (int j = 0; j < logic->getNumHor(); ++j)
                logic->calculateLine(i + j);
    logic->print();

    return 0;
}
