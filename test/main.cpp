#include <iostream>
using namespace std;

#include "SolverTest.hpp"

int main(int argc, char* argv[])
{
    try {
        SolverTest solverTest;
        solverTest.testOpen_1();
        cout << "all success" << endl;
    } catch (string& ex) {
        cerr << ex << endl;
    } catch (exception& ex) {
        cerr << ex.what() << endl;
    }
    return 0;
}
