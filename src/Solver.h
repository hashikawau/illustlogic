
#include <string>
#include <vector>
using namespace std;

class Solver
{
private:
    //const int NUM;
    //IllustrationLogicCanvas* _ic;
    //Label* _label;
    //Button* _button;
    //TextField* _textfield;
    string _fileName;

    int _numRows;
    int _numCols;
    vector<vector<int>> _hints;

    int _numcalclation;
    char* _numblack;
    int** _blacks;
    int _gridsize;
    bool* _grid;
    bool* _flag;

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
    void printBlacks() {
        for (int i = 0; i < _numRows +_numCols; ++i) {
            for (int j = 0; j < _numblack[i]; ++j) {
                printf("%d ", _blacks[i][j]);
            }
            printf("\n");
        }
    }

public:
    Solver();
    ~Solver();
    void open(const string& fileName);
    void solve();
    //IllustrationLogicSolver(const String& string =String(L"")):
    //    Frame(string), NUM(1)
    //{
    //    _ic = new IllustrationLogicCanvas();
    //    ScrollPane* sc = new ScrollPane(*_ic);
    //    sc->setSize(600, 650);
    //    add(*sc);
    //    //add(*_ic);

    //    _label = new Label(String("Calculation time  [ms]"));
    //    _button = new Button(String("Start Calculation"));
    //    _button->setBounds(
    //        0,
    //        _label->getHeight(),
    //        2 *_button->getWidth(),
    //        2 *_button->getHeight());
    //    _button->addActionListener(*this);

    //    _textfield = new TextField(5);
    //    _textfield->setBounds(
    //        0,
    //        _label->getHeight() +_button->getHeight(),
    //        1 *_button->getWidth(),
    //        1 *_button->getHeight());
    //    _textfield->addActionListener(*this);

    //    Container* container = new Container();
    //    container->setSize(
    //        _button->getWidth(),
    //        1 *(_textfield->getHeight() +_button->getHeight() +_label->getHeight()));
    //    container->add(*_label);
    //    container->add(*_button);
    //    container->add(*_textfield);
    //    add(*container);



    //    MenuBar& mb = *new MenuBar();
    //    setMenuBar(mb);
    //    mb.add(*new MenuFile());

    //}
    //~IllustrationLogicSolver(){}

    //virtual void actionPerformed(ActionEvent& e)
    //{
    //    int elapsed = GetTickCount();
    //    Component* source = static_cast<Component*>(e.getSource());
    //    try{
    //        if(dynamic_cast<Button*>(source)){
    //            source = dynamic_cast<Button*>(source);
    //            if(_ic->_numver >0 && _ic->_numhor >0){
    //                for(int k=0; k< _ic->_numcalclation; ++k){
    //                    for(int i=0;
    //                        i< _ic->getNumVer() +_ic->getNumHor();
    //                        ++i)
    //                    {
    //                        _ic->calculateLine(i);
    //                    }
    //                }
    //            }
    //        }else
    //        if(dynamic_cast<TextField*>(source)){
    //            source = dynamic_cast<TextField*>(source);
    //            int line = Integer::parseInt(source->TextComponent::getText());
    //            if(line <0){
    //                for(int i=0; i< _ic->_numver *_ic->_numhor; ++i){
    //                    _ic->_grid[i] =false;
    //                    _ic->_flag[i] =false;
    //                }
    //            }else{
    //                _ic->calculateLine(line);
    //            }
    //        }

    //        elapsed = GetTickCount() -elapsed;
    //        _label->setText(
    //            String("Calculation time ")
    //            +String(elapsed)
    //            +String("[ms]"));
    //    }catch(String s){
    //        _label->setText(s);
    //    }

    //    repaint();
    //}
    //void setFile(const String& filename){
    //    _filename = filename;
    //    _ic->setNumGrid(_filename);
    //}
};

