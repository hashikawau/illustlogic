#include <cassert>
#include <iostream>
#include <sstream>
//#include "../src/Solver.h"
#include "Solver.h"

using namespace std;

extern vector<vector<int>> expectedRowHints;
extern vector<vector<int>> expectedColHints;
extern vector<vector<int>> expectedBlackIndices;

int main(int argc, char* argv[])
{
    string fileName = "input/50x60_01.txt";

    auto solver = Solver();
    solver.open(fileName);
    assert(solver.numRows() == 50);
    assert(solver.numCols() == 60);
    for (int i=0; i < expectedRowHints.size(); ++i)
        assert(equal(
            expectedRowHints[i].begin(),
            expectedRowHints[i].end(),
            solver.lineHints()[i].hints().begin()));
    for (int i=0; i < expectedColHints.size(); ++i)
        assert(equal(
            expectedColHints[i].begin(),
            expectedColHints[i].end(),
            solver.lineHints()[i + solver.numRows()].hints().begin()));

    solver.solve();

    for (int i : expectedBlackIndices[0])
        assert(solver.blacks()[0 * solver.numCols() + i]);
    for (int i : expectedBlackIndices[1])
        assert(solver.blacks()[1 * solver.numCols() + i]);
    for (int i : expectedBlackIndices[2])
        assert(solver.blacks()[24 * solver.numCols() + i]);
    for (int i : expectedBlackIndices[3])
        assert(solver.blacks()[39 * solver.numCols() + i]);

    //solver.printGrids();

    //string input;
    //while (true) {
    //    try {
    //        cout<< "numRows=" << solver.numRows()
    //            << ", numCols=" << solver.numCols()
    //            << endl;
    //        cin >> input;
    //        if (input == "q")
    //            break;

    //        int num;
    //        stringstream(input) >> num;
    //        cout<< "num=" << num << endl;
    //        if (num < 0) {
    //            solver.printGrids();
    //            continue;
    //        }

    //        solver.printHints(num);
    //        solver.printLine(num);
    //        solver.calculateLine(num);
    //        solver.printLine(num);
    //    } catch (const exception& ex) {
    //        cerr << ex.what() << endl;
    //    } catch (const string& ex) {
    //        cerr << ex << endl;
    //    }
    //}

    return 0;
}

vector<vector<int>> expectedRowHints = {
    {10, 2},
    {3, 1, 2, 2},
    {2, 2, 2},
    {2, 1, 2},
    {10, 3, 7, 1, 24, 7},
    {10, 2, 2, 2, 1, 25, 6},
    {4, 3, 2},
    {2, 1, 2},
    {2, 4, 2, 2},
    {16, 2},
    {3, 8, 3, 2},
    {4, 1, 2, 1, 2},
    {11, 1, 3, 3, 1, 33},
    {11, 1, 2, 1, 3, 2, 20, 7},
    {11, 1, 1, 19, 5, 5},
    {11, 1, 1, 4, 13, 8, 4},
    {13, 1, 3, 11, 10, 4},
    {13, 3, 2, 1, 9, 3, 1, 1, 2, 3},
    {14, 2, 1, 1, 7, 3, 5, 2, 3},
    {14, 4, 3, 1, 1, 6, 2, 1, 1, 1, 1, 2},
    {15, 3, 3, 2, 2, 4, 2, 8, 1, 2},
    {13, 2, 3, 1, 1, 4, 2, 2, 1, 1, 2},
    {11, 1, 3, 2, 1, 1, 2, 4, 1, 1, 2},
    {9, 1, 1, 6, 1, 5, 1, 2, 1, 1, 1, 2},
    {8, 1, 7, 1, 2, 3, 2, 3, 1, 2},
    {6, 2, 3, 2, 2, 2, 2, 1, 2, 1, 2},
    {5, 2, 3, 2, 1, 1, 1, 2, 1, 2},
    {5, 1, 4, 3, 1, 1, 1, 2, 1, 2},
    {4, 1, 1, 8, 1, 2, 2, 2, 1, 2},
    {4, 2, 7, 8, 1, 2, 1, 2},
    {4, 2, 3, 5, 1, 2, 1, 3},
    {4, 2, 1, 1, 1, 3, 1, 3},
    {5, 3, 1, 3, 1, 2, 2, 4},
    {5, 2, 5, 3, 3, 1, 4},
    {5, 2, 2, 2, 3, 3, 2, 5},
    {6, 2, 1, 2, 4, 2, 6},
    {6, 2, 2, 3, 5, 6, 7},
    {6, 2, 10, 3, 9},
    {6, 2, 5, 2, 1, 4, 18},
    {6, 2, 2, 4, 1, 1, 2, 19},
    {6, 2, 1, 2, 2, 2, 1, 21},
    {7, 2, 2, 1, 2, 1, 2, 24},
    {7, 3, 4, 2, 2, 2, 4, 20},
    {7, 4, 1, 1, 2, 8, 20},
    {7, 1, 1, 2, 2, 5, 20},
    {8, 1, 2, 2, 2, 11, 1},
    {8, 4, 1, 7, 21},
    {2, 6, 3, 2},
    {1, 6, 1},
    {2, 6, 1, 1, 2},
};

vector<vector<int>> expectedColHints = {
    {2, 35},
    {2, 35},
    {2, 35},
    {2, 35},
    {2, 16, 15},
    {2, 14, 12},
    {2, 13, 6},
    {2, 13, 3},
    {2, 12, 3},
    {2, 11, 1, 1, 1, 1},
    {12, 2, 5, 2},
    {8, 6, 14, 2},
    {11, 8, 6, 1, 1},
    {4, 1, 3, 3, 3, 7, 1},
    {3, 1, 1, 2, 10, 1, 3, 1, 2},
    {2, 1, 2, 2, 2, 8, 1, 4},
    {1, 2, 2, 1, 3, 4, 9},
    {2, 1, 4, 1, 1, 3, 3, 2, 4},
    {1, 1, 3, 1, 1, 3, 2, 1, 2},
    {1, 1, 3, 1, 2, 2, 2, 1, 3},
    {1, 1, 3, 5, 2, 2, 3, 1, 4, 1},
    {1, 1, 2, 2, 1, 2, 3, 1, 2, 2},
    {1, 2, 2, 2, 2, 3, 1, 1, 1},
    {2, 1, 1, 1, 3, 2, 1, 1, 1, 2},
    {2, 1, 3, 1, 7, 2, 1, 4},
    {3, 1, 13, 1, 2, 3},
    {6, 2, 2, 2, 1},
    {10, 3, 3},
    {2, 4, 3, 3, 1, 2, 1},
    {2, 3, 2, 2, 4, 3},
    {2, 3, 2, 2, 1, 2, 2, 2},
    {2, 3, 3, 1, 2, 6, 2},
    {2, 4, 1, 2, 2, 1, 2, 2},
    {2, 5, 2, 2, 1, 3, 1, 2},
    {2, 6, 2, 2, 1, 5, 1},
    {2, 7, 2, 3, 8, 2},
    {2, 8, 5, 3, 2, 3},
    {2, 10, 2, 1, 2},
    {2, 11, 1, 3, 2, 2, 2, 1},
    {2, 17, 4, 1, 2, 4},
    {2, 10, 2, 1, 7, 1},
    {2, 8, 10, 1, 8},
    {2, 6, 14, 11},
    {2, 5, 5, 1, 4, 10},
    {2, 4, 3, 2, 3, 10},
    {2, 3, 3, 2, 3, 9, 1},
    {2, 2, 3, 4, 2, 9},
    {2, 1, 3, 1, 1, 1, 2, 9},
    {2, 1, 8, 1, 9, 1},
    {2, 1, 3, 1, 1, 1, 9, 1},
    {2, 1, 7, 1, 7, 1},
    {2, 1, 3, 1, 1, 1, 10},
    {4, 8, 2, 1, 2, 8, 1},
    {5, 8, 3, 3, 9, 1},
    {2, 2, 3, 3, 10, 1},
    {2, 3, 12, 11, 1},
    {2, 5, 13, 1},
    {2, 7, 15, 1},
    {2, 33, 1},
    {2, 33, 1},
};

vector<vector<int>> expectedBlackIndices = {
    {14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 52, 53},
    {13, 14, 15, 17, 23, 24, 52, 53},
    {0, 1, 2, 3, 4, 5, 6, 7, 11, 14, 15, 16, 17, 18, 19, 20, 23, 29, 30, 33, 34, 35, 38, 39, 41, 42, 43, 55, 58, 59},
    {0, 1, 2, 3, 4, 5, 12, 13, 18, 19, 24, 25, 26, 27, 29, 31, 34, 35, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59},
};

