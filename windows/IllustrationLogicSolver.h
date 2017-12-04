#include <vector>
#include <memory>

enum struct Status
{
    UNKNOWN,
    BLACK,
    WHITE
};

struct Cell
{
private:
    enum Status _status;

public:
    Cell()
    : _status(Status::UNKNOWN)
    { }

    Status getStatus() { return _status; }
    void fixStatus(Status status)
    {
        if (status == Status::UNKNOWN) throw "Invalid argument: cannot assign Status::UNKNOWN";
        if (_status != Status::UNKNOWN) throw "Invalid argument: cannot change fixed Status";
        _status = status;
    }
};

struct LineHint
{
private:
    std::vector<int> _hints;
    std::vector<Cell> _cells;

public:
    LineHint(const std::vector<int>& hints, int numCells)
    : _hints(hints)
    , _cells(numCells)
    { }

    int getNumHints() { return _hints.size(); }
    int getHint(int index) { return _hints[index]; }

    Cell getCell(int index) { return _cells[index]; }
    std::vector<Status> getCellStatus()
    {
        std::vector<Status> result;
        for (Cell cell: _cells) {
            result.push_back(cell.getStatus());
        }
        return result;
    }
};

class IllustrationLogicSolver
{
private:
    std::vector<LineHint> _rowHints;
    std::vector<LineHint> _colHints;

    std::vector<bool> _flagblack;
    std::vector<bool> _flagwhite;
    std::vector<bool> _ischanged;

public:
    IllustrationLogicSolver() {}
    virtual ~IllustrationLogicSolver() {}

    //static std::shared_ptr<IllustrationLogicSolver> createFrom(const std::string& inputString);
    void init(const std::string& inputString);
    void print();

    void resetFlags(bool black, bool white);
    int getNumRowHints() { return _rowHints.size(); }
    int getNumColHints() { return _colHints.size(); }
    LineHint getBlack(int index)
    {
        if (index < _rowHints.size())
            return _rowHints[index];
        else
            return _colHints[index - _rowHints.size()];
    }
    bool calculateLine(int indexline);
    bool algorithm_01(bool* flagblack, bool* flagwhite);
    int calcNumChanbers(int numline, bool* flagwhite);
};
