
#include <string>
#include <vector>
using namespace std;

class Solver
{
private:
    string _fileName;

    int _numRows;
    int _numCols;
    vector<vector<int>> _hints;
    vector<bool> _grids;
    vector<bool> _flags;

    vector<bool> _flagblack;
    vector<bool> _flagwhite;
    vector<bool> _ischanged;

    void readSize(ifstream& istream);
    void readHints(ifstream& istream);
    void printHints() {
        for (vector<int>& lineHint: _hints) {
            for (int hint: lineHint) {
                printf("%d ", hint);
            }
            printf("\n");
        }
    }
    void printGrids() {
        for (int i = 0; i < _numRows; ++i) {
            for (int j = 0; j < _numCols; ++j) {
                printf("%c",
                    _flagblack[i * _numCols + j] ? '#' :
                    _flagwhite[i * _numCols + j] ? ' ' :
                    '.');
            }
            printf("\n");
        }
    }

    bool calculateLine(int indexLine);

public:
    Solver();
    ~Solver();
    void open(const string& fileName);
    void solve();
};

