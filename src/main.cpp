#include <iostream>
#include "Solver.h"

using namespace std;

int main(int argc, char* argv[])
{
    string fileName = argv[1];

    auto solver = Solver();
    try {
        cout << fileName << endl;
        solver.open(fileName);
        solver.solve();
        solver.printGrids();
    } catch (const exception& ex) {
        cerr << ex.what() << endl;
    } catch (const string& ex) {
        cerr << ex << endl;
    }

    return 0;
}
