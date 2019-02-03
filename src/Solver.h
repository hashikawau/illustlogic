
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

class Mass
{
private:
public:
    

};

class Solver
{
private:
    string _fileName;

    int _numRows;
    int _numCols;
    vector<LineHint> _lineHints;

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
        int blacks[],
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
    vector<LineHint> lineHints() { return _lineHints; }
    vector<bool> blacks() { return _flagblack; }

    void solve();
    bool calculateLine(int index);

    void printHints();
    void printGrids();
    void printHints(int index);
    void printLine(int index);

};

