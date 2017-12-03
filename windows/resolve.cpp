#include <iostream>
#include <memory>
#include <sstream>

#include "IllustrationLogicSolver.h"

int main(int argc, char** argv)
{
    std::stringstream inputString;
    std::string line;
    while (std::getline(std::cin, line)) {
        //std::cout << line << std::endl;
        inputString << line << std::endl;
    }

    auto logic = std::make_unique<IllustrationLogicSolver>();
    logic->init(inputString.str());
    //auto logic = IllustrationLogicSolver::createFrom(inputString.str());
    for (int k = 0; k < 500; ++k)
        for (int i = 0; i < logic->getNumRowHints(); ++i)
            for (int j = 0; j < logic->getNumColHints(); ++j)
                logic->calculateLine(i + j);
    logic->print();

    return 0;
}
