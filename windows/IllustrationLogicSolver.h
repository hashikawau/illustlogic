#include <memory>

class IllustrationLogicSolver
{
private:
    char* _numblack;
    int** _blacks;

    int _gridsize;
    int _numver;
    int _numhor;

    bool* _flagblack;
    bool* _flagwhite;
    bool* _ischanged;

public:
    IllustrationLogicSolver();
    virtual ~IllustrationLogicSolver();

    //static std::shared_ptr<IllustrationLogicSolver> createFrom(const std::string& inputString);
    void setNumGrid(const std::string& inputString);
    void setNumGrid();
    void resetFlags(bool black, bool white);
    int getNumVer(){ return _numver; }
    int getNumHor(){ return _numhor; }
    bool calculateLine(int indexline);
    void print();
};
