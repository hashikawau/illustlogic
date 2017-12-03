
class IllustrationLogicCanvas
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
    IllustrationLogicCanvas();
    virtual ~IllustrationLogicCanvas();

    void setNumGrid();
    void resetFlags(bool black, bool white);
    int getNumVer(){ return _numver; }
    int getNumHor(){ return _numhor; }
    bool calculateLine(int indexline);
    void print();
};
