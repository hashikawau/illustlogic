
#include <string>
#include <vector>
using namespace std;

class LineHint
{
private:
    vector<int> _hints;

public:
    LineHint() = default;
    ~LineHint() = default;
    vector<int> hints() { return _hints; }

    bool parse(const string& lineHintStr);

    bool calculate(
        int indexLine,
        bool flagblack[],
        bool flagwhite[]
    );
};

enum State
{
    UNKNOWN,
    BLACK,
    WHITE,
};

class Chamber
{
private:
public:
    int head;
    int size;

    void init();
};

class Solver
{
private:
    string _fileName;

    int _numRows;
    int _numCols;
    vector<LineHint> _rowHints;
    vector<LineHint> _colHints;
    vector<vector<int>> _hints;

    vector<bool> _flagblack;
    vector<bool> _flagwhite;
    vector<bool> _ischanged;

    void readSize(ifstream& istream);
    void readHints(ifstream& istream);

    bool calculateLine(
        int indexLine,
        int numline,
        int offset,
        int step,
        bool flagblack[],
        bool flagwhite[]
    );
    vector<Chamber> createChambers(
        int numline,
        bool flagwhite[]
    );

public:
    Solver() = default;
    ~Solver() = default;
    void open(const string& fileName);
    int numRows() { return _numRows; }
    int numCols() { return _numCols; }
    vector<LineHint> rowHints() { return _rowHints; }
    vector<LineHint> colHints() { return _colHints; }

    void solve();
    vector<bool> blacks() { return _flagblack; }

    void printHints();
    void printGrids();

    void printHints(int index);
    void printLine(int index);
    bool calculateLine(int index);

};

