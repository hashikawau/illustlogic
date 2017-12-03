#include <vector>
#include <memory>

struct LineHint
{
private:
    std::vector<int> _hints;

public:
    LineHint(const std::vector<int>& hints)
    : _hints(hints)
    { }

    int getNumHints() { return _hints.size(); }
    int getHint(int index) { return _hints[index]; }
};

class IllustrationLogicSolver
{
private:
    std::vector<LineHint> _rowHints;
    std::vector<LineHint> _colHints;

    bool* _flagblack;
    bool* _flagwhite;
    bool* _ischanged;

public:
    IllustrationLogicSolver();
    virtual ~IllustrationLogicSolver();

    //static std::shared_ptr<IllustrationLogicSolver> createFrom(const std::string& inputString);
    void init(const std::string& inputString);
    void resetFlags(bool black, bool white);
    int getNumRowHints() { return _rowHints.size(); }
    int getNumColHints() { return _colHints.size(); }
    LineHint getBlack(int index) {
        if (index < _rowHints.size())
            return _rowHints[index];
        else
            return _colHints[index - _rowHints.size()];
    }
    bool calculateLine(int indexline);
    void print();
};
