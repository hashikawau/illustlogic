#include <fstream>
#include <iostream>
using namespace std;

void assert(bool success, const string& message = "")
{
    if (!success)
        throw "assert failed: " + message;
}

template<class T>
void assert(T expected, T actual, const string& message = "")
{
    if (actual != expected)
        throw "assert failed: " + message
            + ": actual=" + to_string(actual)
            + ", expected=" + to_string(expected);
}

#include "../src/Solver.h"

class SolverTest
{
private:
    string fileName = "data.txt";
public:
    void testOpen_1() {
        ofstream ostream(fileName);
        ostream << "1 2" << endl;
        ostream << "2" << endl;
        ostream << "1" << endl;
        ostream << "1" << endl;
        ostream.close();

        Solver solver;
        solver.open(fileName);
        assert(1, solver.numRows(), "numRows()");
        assert(2, solver.numCols(), "numCols()");
        assert(3, (int)solver.lineHints().size());
        assert(1, (int)solver.lineHints()[0].hints().size());
        assert(2, (int)solver.lineHints()[0].hints()[0]);
    }

};

