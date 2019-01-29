#include <iostream>
#include "Solver.h"

using namespace std;

int main(int argc, char* argv[])
{
    //auto fileName = "../test/input/iraroji(15_15)-01.txt";
    //auto fileName = "../test/input/iraroji(50_60)-01.txt";
    auto fileName = "../test/input/iraroji(60_90)-01.txt";

    auto solver = Solver();
    try {
        cout << fileName << endl;
        solver.open(fileName);
        solver.solve();
    } catch (const exception& ex) {
        cerr << ex.what() << endl;
    } catch (const string& ex) {
        cerr << ex << endl;
    }

    return 0;
}
