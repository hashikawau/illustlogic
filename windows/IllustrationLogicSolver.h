#include <memory>

class IllustrationLogicSolver
{
private:
    int _numver;
    int _numhor;

    int* _numblack;
    int** _blacks;

    bool* _flagblack;
    bool* _flagwhite;
    bool* _ischanged;

public:
    IllustrationLogicSolver();
    virtual ~IllustrationLogicSolver();

    //static std::shared_ptr<IllustrationLogicSolver> createFrom(const std::string& inputString);
    void init(const std::string& inputString);
    void resetFlags(bool black, bool white);
    int getNumVer(){ return _numver; }
    int getNumHor(){ return _numhor; }
    bool calculateLine(int indexline);
    void print();
};
