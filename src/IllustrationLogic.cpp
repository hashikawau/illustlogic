

//#include "_awt.h"
//#include "_lang.h"
//#include "_util.h"
//#include "_awt.h"
//#include "_awt_event.h"

//#include "awt\Frame.h"
//#include "awt\Label.h"
//#include "awt\Button.h"
//#include "awt\Canvas.h"
//#include "awt\ScrollPane.h"
//#include "awt\TextField.h"
//#include "awt\FileDialog.h"

//#include "util\LinkedList.h"

#include <fstream>

#include <mmsystem.h>

//===========================================================
//===========================================================
class MenuFile: public Menu
{
public:
    class MenuNew: public MenuItem
    {
    private:
        class Listener: public ActionListener
        {
        public:
            virtual void actionPerformed(ActionEvent& e){}
        };
    public:
        MenuNew(): MenuItem(String("&New"))
        {
            addActionListener(*new Listener());
        }
        virtual ~MenuNew(){}
    };
    //===========================================================
    //===========================================================
    class MenuOpen: public MenuItem
    {
    private:
        class Listener: public ActionListener
        {
        public:
            virtual void actionPerformed(ActionEvent& e);
        };
    public:
        MenuOpen(): MenuItem(String("&Open"))
        {
            addActionListener(*new Listener());
        }
        virtual ~MenuOpen(){}
    };
    //===========================================================
    //===========================================================
    class MenuSave: public MenuItem
    {
    private:
        class Listener: public ActionListener
        {
        public:
            virtual void actionPerformed(ActionEvent& e){}
        };
    public:
        MenuSave(): MenuItem(String("&Save"))
        {
            addActionListener(*new Listener());
        }
        virtual ~MenuSave(){}
    };
    //===========================================================
    //===========================================================
    class MenuSaveAs: public MenuItem
    {
    private:
        class Listener: public ActionListener
        {
        public:
            virtual void actionPerformed(ActionEvent& e)
            {
                MenuSaveAs& m
                    = *static_cast<MenuSaveAs*>(e.getSource());
                MenuComponent* mc
                    = dynamic_cast<MenuComponent*>(&m.getParent());
                if(!mc){ return; }
                
                Frame* f
                    = static_cast<Frame*>(&mc->getParent());
                    //= dynamic_cast<Frame*>(&mc->getParent());
                //if(!f){ throw int(); }
                
                FileDialog fd(*f, String("Save As"), FileDialog::SAVE);
                fd.setVisible(true);
                //String filename
                //    = String(fd.getDirectory())
                //    +String(fd.getFile());
            }
        };
    public:
        MenuSaveAs(): MenuItem(String("Save &As..."))
        {
            addActionListener(*new Listener());
        }
        virtual ~MenuSaveAs(){}
    };
    //===========================================================
    //===========================================================
    class MenuExit: public MenuItem
    {
    private:
        class Listener: public ActionListener
        {
        public:
            virtual void actionPerformed(ActionEvent& e){}
        };
    public:
        MenuExit(): MenuItem(String("&Exit"))
        {
            addActionListener(*new Listener());
        }
        virtual ~MenuExit(){}
    };
    //===========================================================
    //===========================================================
    MenuFile(): Menu(String("&File"))
    {
        add(*new MenuNew());
        add(*new MenuOpen());
        addSeparator();
        add(*new MenuSave());
        add(*new MenuSaveAs());
        addSeparator();
        add(*new MenuExit());
    }
};
//===========================================================
//===========================================================

//===========================================================
//===========================================================
class IllustrationLogicCanvas:
    public Canvas,
    public MouseListener,
    public MouseMotionListener
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
    virtual void update(Graphics& g);
    virtual void paint(Graphics& g);
    
    virtual void mouseClicked(MouseEvent& e);
    virtual void mouseEntered(MouseEvent& e);
    virtual void mouseExited(MouseEvent& e);
    virtual void mousePressed(MouseEvent& e);
    virtual void mouseReleased(MouseEvent& e);
    virtual void mouseDragged(MouseEvent& e);
    virtual void mouseMoved(MouseEvent& e);
    
    void setNumGrid(String& filename);
    void resetFlags(bool black, bool white);
    int getNumVer(){ return _numver; }
    int getNumHor(){ return _numhor; }
    bool calculateLine(int indexline);
};
//===========================================================
//===========================================================
IllustrationLogicCanvas::IllustrationLogicCanvas()
{
    _numblack =0;
    _blacks   =0;
    
    _gridsize =10;
    
    _flagblack =0;
    _flagwhite =0;
    _ischanged =0;
    
    addMouseListener(*this);
    addMouseMotionListener(*this);
    setVisible(true);
}
IllustrationLogicCanvas::~IllustrationLogicCanvas()
{
    delete [] _ischanged;
    delete [] _flagblack;
    delete [] _flagwhite;
    
    if(_blacks){
        for(int i=0; i< _numver +_numhor; ++i){
            delete [] _blacks[i];
        }
        delete [] _blacks;
    }
    delete [] _numblack;
    
    _ischanged =0;
    _flagblack =0;
    _flagwhite =0;
    _blacks =0;
    _numblack =0;
}
//===========================================================
//===========================================================
void IllustrationLogicCanvas::setNumGrid(String& filename)
{
    delete [] _ischanged;
    delete [] _flagblack;
    delete [] _flagwhite;
    
    if(_blacks){
        for(int i=0; i< _numver +_numhor; ++i){
            delete [] _blacks[i];
        }
        delete [] _blacks;
    }
    delete [] _numblack;
    
    _blacks   =0;
    _numblack =0;
    
    _flagblack =0;
    _flagwhite =0;
    _ischanged =0;
    
    char name[255];
    wcstombs(name, filename.toCharArray(), filename.length() +1);
    ifstream stream;
    stream.open(name);
    //===========================================================
    if(stream){
        char ch;
        char str[10];
        LinkedList<int> list;
        int count =0;
        while(stream.get(ch)){
            if(ch == '\n'){
                if(count){
                    str[count] = '\0';
                    list.addLast(Integer::parseInt(String(str)));
                }
                list.addLast(0);
                count =0;
            }else
            if(ch == ' '){
                str[count] = '\0';
                list.addLast(Integer::parseInt(String(str)));
                count =0;
            }else{
                str[count] = ch;
                ++count;
            }
        }
        if(count){
            str[count] = '\0';
            list.addLast(Integer::parseInt(String(str)));
        }
        list.addLast(0);
        
        stream.close();
        
        //===========================================================
        int size = list.size();
        int array[size];
        list.toArray(array);
        int num[] ={0, 0, 0};
        int index =0;
        int countzero =0;
        for(int i=0; i< size; ++i){
            if(array[i]){
                if(countzero >1){
                    ++index;
                }
                countzero =0;
                if(!index){
                    array[i] =0;
                }
            }else{
                if(!countzero){
                    ++num[index];
                }
                ++countzero;
            }
        }
        
        _numver = num[1];
        _numhor = num[2];
        
        
        _blacks   = new int* [_numver +_numhor];
        _numblack = new char [_numver +_numhor];
        
        int start =0;
        index =-1;
        count =0;
        countzero =0;
        for(int i=0; i< size; ++i){
            if(array[i]){
                if(countzero >0){
                    start =i;
                    ++index;
                }
                countzero =0;
                ++count;
            }else{
                if(countzero == 0){
                    _numblack[index] =count;
                    _blacks[index] = new int [count];
                    for(int j= start, k=0; j< i; ++j){
                        if(array[j]){
                            _blacks[index][k] = array[j];
                            ++k;
                        }
                    }
                }
                count =0;
                ++countzero;
            }
        }
        
        //===========================================================
        _flagblack = new bool [_numver *_numhor];
        _flagwhite = new bool [_numver *_numhor];
        for(int i=0; i< _numver *_numhor; ++i){
            _flagblack[i] =false;
            _flagwhite[i] =false;
        }
        _ischanged = new bool [_numver +_numhor];
        for(int i=0; i< _numver +_numhor; ++i){
            _ischanged[i] =true;
        }
    }else{
        _numver =0;
        _numhor =0;
    }
    
    setSize(_gridsize *_numhor +2, _gridsize *_numver +2);
}
//===========================================================
//===========================================================
void IllustrationLogicCanvas::resetFlags(bool black, bool white)
{
    if(black){
        for(int i=0; i< _numver *_numhor; ++i){
            _flagblack[i] =false;
            //_flagwhite[i] =false;
        }
    }
    if(white){
        for(int i=0; i< _numver *_numhor; ++i){
            //_flagblack[i] =false;
            _flagwhite[i] =false;
        }
    }
    for(int i=0; i< _numver +_numhor; ++i){
        _ischanged[i] =true;
    }
}
//===========================================================
//===========================================================
void IllustrationLogicCanvas::update(Graphics& g)
{
    Canvas::update(g);
    //paint(g);
}
void IllustrationLogicCanvas::paint(Graphics& g)
{
    g.setColor(Color::gray);
    int width = getWidth();
    int height = getHeight();
    
    for(int i=0; i<= Math::max(_numver, _numhor); ++i){
        g.drawLine(0, _gridsize *i, width, _gridsize *i);
        g.drawLine(_gridsize *i, 0, _gridsize *i, height);
    }
    for(int i=0; i<= Math::max(_numver, _numhor); i +=5){
        g.drawLine(0, _gridsize *i +1, width, _gridsize *i +1);
        g.drawLine(_gridsize *i +1, 0, _gridsize *i +1, height);
    }
    
    for(int i=0; i< _numver; ++i){
        for(int j=0; j< _numhor; ++j){
            if(_flagblack[_numhor *i +j]){
                g.setColor(Color::black);
                g.fillRect(
                    _gridsize *j, _gridsize *i,
                    _gridsize, _gridsize);
            }else
            if(_flagwhite[_numhor *i +j]){
                g.setColor(Color::gray);
                g.drawLine(
                    _gridsize *j, _gridsize *i,
                    _gridsize *(j+1), _gridsize *(i+1));
                g.drawLine(
                    _gridsize *(j+1), _gridsize *i,
                    _gridsize *j, _gridsize *(i+1));
            }
        }
    }
}
//===========================================================
//===========================================================
void IllustrationLogicCanvas::mouseClicked(MouseEvent& e){}
void IllustrationLogicCanvas::mouseEntered(MouseEvent& e){}
void IllustrationLogicCanvas::mouseExited(MouseEvent& e){}
void IllustrationLogicCanvas::mousePressed(MouseEvent& e)
{
    int button = e.getButton();
    int numver = e.getY() /_gridsize;
    int numhor = e.getX() /_gridsize;
    
    if(button == MouseEvent::BUTTON1){
        _flagblack[_numhor *numver +numhor] =true;
        _flagwhite[_numhor *numver +numhor] =false;
    }else
    if(button == MouseEvent::BUTTON2){
        _flagblack[_numhor *numver +numhor] =false;
        _flagwhite[_numhor *numver +numhor] =true;
    }else
    if(button == MouseEvent::BUTTON3){
        _flagblack[_numhor *numver +numhor] =false;
        _flagwhite[_numhor *numver +numhor] =false;
    }
    _ischanged[numver] =true;
    _ischanged[numhor] =true;
        
    repaint();
}
void IllustrationLogicCanvas::mouseReleased(MouseEvent& e){}
//===========================================================
//===========================================================
void IllustrationLogicCanvas::mouseDragged(MouseEvent& e)
{
    static int prenumver;
    static int prenumhor;
    int button = e.getButton();
    int numver = e.getY() /_gridsize;
    int numhor = e.getX() /_gridsize;
    if(prenumver !=numver || prenumhor !=numhor){
        if(button == MouseEvent::BUTTON1){
            _flagblack[_numhor *numver +numhor] =true;
            _flagwhite[_numhor *numver +numhor] =false;
        }else
        if(button == MouseEvent::BUTTON2){
            _flagblack[_numhor *numver +numhor] =false;
            _flagwhite[_numhor *numver +numhor] =true;
        }else
        if(button == MouseEvent::BUTTON3){
            _flagblack[_numhor *numver +numhor] =false;
            _flagwhite[_numhor *numver +numhor] =false;
        }
        _ischanged[numver] =true;
        _ischanged[numhor] =true;
        
        repaint();
    }
    prenumver = numver;
    prenumhor = numhor;
}
void IllustrationLogicCanvas::mouseMoved(MouseEvent& e){}
//===========================================================
//===========================================================
bool IllustrationLogicCanvas::calculateLine(int indexline)
{
    int numline;
    int offset;
    int step;
    if(indexline < _numver){
        numline = _numhor;
        offset  = indexline *_numhor;
        step    = 1;
    }else
    if(indexline < _numver +_numhor){
        numline = _numver;
        offset  = indexline -_numver;
        step    = _numhor;
    }else{
        throw String("error at line ")
            +String(indexline)
            +String(" - initialization");
    }
    
    //==================================================================
    bool flagblack[numline];
    bool flagwhite[numline];
    bool isfinished =true;
    for(int i=0; i< numline; ++i){
        flagblack[i] = _flagblack[offset +step *i];
        flagwhite[i] = _flagwhite[offset +step *i];
        isfinished &= flagblack[i] | flagwhite[i];
    }
    if(isfinished){
        return true;
    }
    if(!_ischanged[indexline]){
        return false;
    }
    
    //==================================================================
    int numblack = _numblack[indexline];
    int blacks[numblack];
    for(int i=0; i< numblack; ++i){
        blacks[i] = _blacks[indexline][i];
    }
    
    //==================================================================
{//
    /*//bool isblack =false;
    int countblack =0;
    int countstring =0;
    for(int j=0; j< numline; ++j){
        if(flagblack[j]){
            ++countstring;
        }else{
            if(countstring >0){
                if(countblack < numblack){
                    if(countstring == blacks[countblack]){
                        ++countblack;
                    }else{
                        countstring =-1;
                        break;
                    }
                }else{
                    countstring =-1;
                    break;
                }
            }
            countstring =0;
        }
    }
    if(countstring >0){
        if(countblack < numblack){
            if(countstring == blacks[countblack]){
                ++countblack;
                countstring =0;
            }else{
                countstring =-1;
            }
        }else{
            countstring =-1;
        }
    }
    if(countblack == numblack && countstring ==0){
        return true;
    }
    if(!_ischanged[indexline]){
        return false;
    }*/
}//
    
    //==================================================================
    bool blackhead[numblack][numline];
    for(int i=0; i< numblack; ++i){
        bool doexist =true;
        for(int j=0; j< numline; ++j){
            blackhead[i][j] = true;
            doexist &= blackhead[i][j];
        }
    }
    
    //==================================================================
    bool prevflagblack[numline];
    bool prevflagwhite[numline];
    bool prevblackhead[numblack][numline];
    //ismuch =true;
    for(int j=0; j< numline; ++j){
        //ismuch &= prevflagblack[j] == flagblack[j];
        //ismuch &= prevflagwhite[j] == flagwhite[j];
        prevflagblack[j] = flagblack[j];
        prevflagwhite[j] = flagwhite[j];
        for(int i=0; i< numblack; ++i){
            prevblackhead[i][j] = blackhead[i][j];
        }
    }
    bool ismuch =false;
    
    //==================================================================
    while(!ismuch){
        
        //chamber
        //==================================================================
        int numchamber =0;
{//numchamber
        bool iswhite =true;
        for(int j=0; j< numline; ++j){
            if(flagwhite[j]){
                iswhite =true;
            }else{
                if(iswhite){
                    ++numchamber;
                    //++countchamber;
                    //chamberhead[countchamber] = j;
                }
                iswhite =false;
                //++sizechamber[countchamber];
            }
        }
}//numchamber
        
        //==================================================================
        int chamberhead[numchamber];
        int sizechamber[numchamber];
{//chamberhead, sizechamber
        for(int i=0; i< numchamber; ++i){
            sizechamber[i] =0;
        }
        bool iswhite =true;
        int countchamber =-1;
        for(int j=0; j< numline; ++j){
            if(flagwhite[j]){
                iswhite =true;
            }else{
                if(iswhite){
                    //++numchamber;
                    ++countchamber;
                    chamberhead[countchamber] = j;
                }
                iswhite =false;
                ++sizechamber[countchamber];
            }
        }
}//chamberhead, sizechamber
        
        //==================================================================
        for(int j=0; j< numline; ++j){
            if(flagblack[j]){
                for(int i=0; i< numblack; ++i){
                    if(j -blacks[i] >=0){
                        blackhead[i][j -blacks[i]] =false;
                    }
                    if(j+1 < numline){
                        blackhead[i][j+1] =false;
                    }
                }
            }
            if(flagwhite[j]){
                for(int i=0; i< numblack; ++i){
                    blackhead[i][j] =false;
                }
            }
        }
        
        //==================================================================
        for(int k=0; k< numchamber; ++k){
            for(int i=0; i< numblack; ++i){
                for(int j= chamberhead[k];
                    j< chamberhead[k] +sizechamber[k];
                    ++j)
                {
                    if(sizechamber[k] < blacks[i]){
                        blackhead[i][j] =false;
                    }else
                    if(blackhead[i][j]
                        && j +blacks[i] > chamberhead[k] +sizechamber[k])
                    {
                        blackhead[i][j] = false;
                    }
                }
            }
        }
        
        //==================================================================
{//
        int offsetblack =0;
        for(int k=0; k< numchamber; ++k){
            for(int j= chamberhead[k];
                j< chamberhead[k] +sizechamber[k];
                ++j)
            {
                if(blackhead[offsetblack][j]){
                    int tail = j +blacks[offsetblack];
                    if(tail <= chamberhead[k] +sizechamber[k]){
                        ++offsetblack;
                        if(offsetblack >= numblack){
                            break;
                        }
                        for(int i= offsetblack; i< numblack; ++i){
                            for(int j2=0;
                                j2<= Math::min(numline -1, tail);
                                ++j2)
                            {
                                blackhead[i][j2] =false;
                            }
                        }
                    }else{
                        blackhead[offsetblack][j] =false;
                    }
                }
            }
            if(offsetblack >= numblack){
                break;
            }
        }
}//
{//
        int offsetblack = numblack -1;
        for(int k= numchamber -1; k>= 0; --k){
            int indexwall = chamberhead[k] +sizechamber[k];
            for(int j= indexwall -1;
                j>= chamberhead[k];
                --j)
            {
                if(blackhead[offsetblack][j]){
                    int tail = j +blacks[offsetblack];
                    if(tail <= indexwall){
                        indexwall = j -1;
                        --offsetblack;
                        if(offsetblack <0){
                            break;
                        }
                        for(int i= offsetblack; i>= 0; --i){
                            for(int j2= Math::max(0, j-1);
                                j2< numline;
                                ++j2)
                            {
                                blackhead[i][j2] =false;
                            }
                        }
                    }else{
                        blackhead[offsetblack][j] =false;
                    }
                }
            }
            if(offsetblack <0){
                break;
            }
        }
}//
        
        //string black
        //==================================================================
        int numstring =0;
{//numstring
        bool isblack =false;
        for(int j=0; j< numline; ++j){
            if(flagblack[j]){
                if(!isblack){
                    ++numstring;
                    //++countstring;
                    //stringhead[countstring] = j;
                }
                isblack =true;
                //++sizestring[countstring];
            }else{
                isblack =false;
            }
        }
}//numstring
        
        //==================================================================
        int stringhead[numstring];
        int sizestring[numstring];
{//stringhead, sizestring
        for(int i=0; i< numstring; ++i){
            sizestring[i] =0;
        }
        bool isblack =false;
        int countstring =-1;
        for(int j=0; j< numline; ++j){
            if(flagblack[j]){
                if(!isblack){
                    //++numstring;
                    ++countstring;
                    stringhead[countstring] = j;
                }
                isblack =true;
                ++sizestring[countstring];
            }else{
                isblack =false;
            }
        }
}//stringhead, sizestring
        
        //ispossible
        //==================================================================
        bool ispossible[numstring][numblack];
{//ispossible
        //==================================================================
        bool blackoverlap[numblack][numline];
        for(int i=0; i< numblack; ++i){
            for(int j=0; j< numline; ++j){
                blackoverlap[i][j] = false;
            }
        }
        for(int i=0; i< numblack; ++i){
            for(int j=0; j< numline; ++j){
                if(blackhead[i][j]){
                    for(int k=0; k< blacks[i]; ++k){
                        blackoverlap[i][j +k] |= true;
                    }
                }
            }
        }
        
        //==================================================================
        for(int k=0; k< numstring; ++k){
            for(int i=0; i< numblack; ++i){
                ispossible[k][i] =true;
            }
        }
        for(int k=0; k< numstring; ++k){
            for(int j= stringhead[k];
                j< stringhead[k] +sizestring[k];
                ++j)
            {
                for(int i=0; i< numblack; ++i){
                    ispossible[k][i] &= blackoverlap[i][j];
                }
            }
            for(int i=0; i< numblack; ++i){
                ispossible[k][i] &= blacks[i] >= sizestring[k];
            }
        }
}//
        
        //==================================================================
        int indexchamberstring[numstring];
        for(int k=0; k< numstring; ++k){
            int countchamber = numchamber -1;
            while(chamberhead[countchamber] > stringhead[k]){
                if(--countchamber <0){
                    throw String("error at line ")
                        +String(indexline)
                        +String(" - black string");
                }
            }
            indexchamberstring[k] = countchamber;
        }
        
        //==================================================================
{//
        int indexblackdetermined[numstring];
        for(int k=0; k< numstring; ++k){
            indexblackdetermined[k] =-1;
            for(int i=0; i< numblack; ++i){
                if(ispossible[k][i]){
                    if(indexblackdetermined[k] ==-1){
                        indexblackdetermined[k] = i;
                    }else{
                        indexblackdetermined[k] =-2;
                        break;
                    }
                }
            }
        }
        int prevcount =0;
        for(int k=0; k< numstring; ++k){
            if(indexblackdetermined[k] >=0){
                ++prevcount;
            }
        }
        
        while(1){
            for(int k=0; k< numstring; ++k){
                int index = indexblackdetermined[k];
                if(index >=0){
                    for(int i=0; i<= index; ++i){
                        for(int k2= k+1; k2< numstring; ++k2){
                            if(indexchamberstring[k]
                                != indexchamberstring[k2])
                            {
                                ispossible[k2][i] =false;
                            }else{
                                if(stringhead[k] +blacks[index]
                                    < stringhead[k2] +sizestring[k2])
                                {
                                    ispossible[k2][i] =false;
                                }
                            }
                        }
                    }
                    for(int i= index; i< numblack; ++i){
                        for(int k2=0; k2< k; ++k2){
                            if(indexchamberstring[k]
                                != indexchamberstring[k2])
                            {
                                ispossible[k2][i] =false;
                            }else{
                                if(stringhead[k] +blacks[index]
                                    < stringhead[k2] +sizestring[k2])
                                {
                                    ispossible[k2][i] =false;
                                }
                            }
                        }
                    }
                }
                
                indexblackdetermined[k] =-1;
                for(int i=0; i< numblack; ++i){
                    if(ispossible[k][i]){
                        if(indexblackdetermined[k] ==-1){
                            indexblackdetermined[k] = i;
                        }else{
                            indexblackdetermined[k] =-2;
                            break;
                        }
                    }
                }
            }
            int count =0;
            for(int k=0; k< numstring; ++k){
                if(indexblackdetermined[k] >=0){
                    ++count;
                }
            }
            if(prevcount == count){ break; }
            prevcount = count;
        }
        
        //==================================================================
        /*for(int k=0; k< numstring; ++k){
            cout << indexline << ", ";
            cout << stringhead[k] << " - ";
            cout << sizestring[k] << ", ";
            cout << "[ ";
            for(int i=0; i< numblack; ++i){
                cout << blacks[i] << " ";
            }
            cout << "], ";
            cout << "chamber - " << indexchamberstring[k];
            cout << " ==> ";
            for(int i=0; i< numblack; ++i){
                cout << ispossible[k][i] << " ";
            }
            cout << endl;
        }*/
        //==================================================================
}//
        
        //==================================================================
        for(int k=0; k< numstring; ++k){
            for(int i=0; i< numblack; ++i){
                if(ispossible[k][i]){
                    for(int j= stringhead[k] +1; j< numline; ++j){
                        blackhead[i][j] =false;
                    }
                    break;
                }
            }
        }
        for(int k= numstring -1; k>= 0; --k){
            for(int i= numblack -1; i>= 0; --i){
                if(ispossible[k][i]){
                    for(int j=0;
                        j< stringhead[k] +sizestring[k] -blacks[i];
                        ++j)
                    {
                        blackhead[i][j] =false;
                    }
                    break;
                }
            }
        }
        
        //==================================================================
{//painting in black string
        bool blackoverlap[numline];
        bool whiteoverlap[numline];
        bool blackoverlapsolo[numline];
        bool whiteoverlapsolo[numline];
        for(int i=0; i< numline; ++i){
            blackoverlap[i] =false;
            //whiteoverlap[i] =true;
            whiteoverlap[i] =false;
        }
        for(int k=0; k< numstring; ++k){
            bool blackoverlapstring[numline];
            bool whiteoverlapstring[numline];
            for(int j=0; j< numline; ++j){
                blackoverlapstring[j] =true;
                whiteoverlapstring[j] =true;
            }
            for(int i=0; i< numblack; ++i){
                if(ispossible[k][i]){
                    for(int j=0; j< numline; ++j){
                        blackoverlapsolo[j] =true;
                        whiteoverlapsolo[j] =true;
                    }
                    for(int j= Math::max(
                            0, 
                            stringhead[k] +sizestring[k] -blacks[i]);
                        j<= stringhead[k];
                        ++j)
                    {
                        if(blackhead[i][j]){
                            for(int j2= 0; j2< j; ++j2){
                                blackoverlapsolo[j2] =false;
                            }
                            for(int j2= j +blacks[i]; j2< numline; ++j2){
                                blackoverlapsolo[j2] =false;
                            }
                            for(int j2= 0; j2< j -1; ++j2){
                                whiteoverlapsolo[j2] =false;
                            }
                            for(int j2= j; j2< j +blacks[i]; ++j2){
                                whiteoverlapsolo[j2] =false;
                            }
                            for(int j2= j +blacks[i] +1; j2< numline; ++j2){
                                whiteoverlapsolo[j2] =false;
                            }
                        }
                    }
                    for(int j=0; j< numline; ++j){
                        blackoverlapstring[j] &= blackoverlapsolo[j];
                        whiteoverlapstring[j] &= whiteoverlapsolo[j];
                    }
                }else{
                    
                }
            }
            
            for(int j=0; j< numline; ++j){
                blackoverlap[j] |= blackoverlapstring[j];
                whiteoverlap[j] |= whiteoverlapstring[j];
            }
        }
        for(int j=0; j< numline; ++j){
            flagblack[j] |= blackoverlap[j];
            flagwhite[j] |= whiteoverlap[j];
        }
        
        /*for(int i=0; i< numline; ++i){
            if(flagblack[i] & flagwhite[i]){
                throw String("error at line ")
                    +String(indexline)
                    +String(" - painting in black string");
            }
        }*/
}//painting in black string
    
    //==================================================================
{//painting
        bool blackoverlap[numline];
        bool whiteoverlap[numline];
        bool blackoverlapsolo[numline];
        for(int i=0; i< numline; ++i){
            blackoverlap[i] =false;
            whiteoverlap[i] =true;
        }
        for(int i=0; i< numblack; ++i){
            for(int j=0; j< numline; ++j){
                blackoverlapsolo[j] =true;
            }
            for(int j=0; j< numline; ++j){
                if(blackhead[i][j]){
                    for(int k=0; k< j; ++k){
                        blackoverlapsolo[k] =false;
                    }
                    for(int k= j; k< j +blacks[i]; ++k){
                        whiteoverlap[k] =false;
                    }
                    for(int k= j +blacks[i]; k< numline; ++k){
                        blackoverlapsolo[k] =false;
                    }
                }
            }
            for(int j=0; j< numline; ++j){
                blackoverlap[j] |= blackoverlapsolo[j];
            }
        }
        for(int j=0; j< numline; ++j){
            flagblack[j] |= blackoverlap[j];
            flagwhite[j] |= whiteoverlap[j];
        }
        
        for(int i=0; i< numline; ++i){
            if(flagblack[i] & flagwhite[i]){
                throw String("error at line ")
                    +String(indexline)
                    +String(" - painting");
            }
        }
}//painting
    
        //==================================================================
        /*for(int i=0; i< numblack; ++i){
            cout << indexline << ", " << blacks[i] << "==> ";
            for(int j=0; j< numline; ++j){
                cout << blackhead[i][j];
                if(!((j+1)%5)){
                    cout << " ";
                }
            }
            cout << endl;
        }
        cout << endl;*/
        //==================================================================
        
        //==================================================================
        ismuch =true;
        for(int j=0; j< numline; ++j){
            ismuch &= prevflagblack[j] == flagblack[j];
            ismuch &= prevflagwhite[j] == flagwhite[j];
            prevflagblack[j] = flagblack[j];
            prevflagwhite[j] = flagwhite[j];
            
            for(int i=0; i< numblack; ++i){
                ismuch &= prevblackhead[i][j] == blackhead[i][j];
                prevblackhead[i][j] = blackhead[i][j];
            }
        }
    }//while
    
    //==================================================================
    int offset2 = indexline < _numver?
        _numver:
        0;
    for(int j=0; j< numline; ++j){
        /*if(!_flagblack[offset +step *j] & !_flagwhite[offset +step *j]){
            if(flagblack[j] | flagwhite[j]){
                _ischanged[offset2 +j] =true;
            }
        }*/
        if(_flagblack[offset +step *j] != flagblack[j]
            || _flagwhite[offset +step *j] != flagwhite[j]){
            _ischanged[offset2 +j] =true;
        }
        _flagblack[offset +step *j] = flagblack[j];
        _flagwhite[offset +step *j] = flagwhite[j];
    }
    _ischanged[indexline] =false;
    
    //==================================================================
    return false;
}
//===========================================================
//===========================================================
class IllustrationLogicSolver:
    public Frame,
    public ActionListener
{
private:
    IllustrationLogicCanvas* _ic;
    ScrollPane* _sc;
    Label* _label;
    Button** _button;
    TextField* _textfield;
    String _filename;
    
public:
    IllustrationLogicSolver(String& string =String(L""));
    ~IllustrationLogicSolver();
    virtual void actionPerformed(ActionEvent& e);
    
    void teststart();
    void setFile(String& filename);
};
//===========================================================
//===========================================================
IllustrationLogicSolver::IllustrationLogicSolver(String& string):
    Frame(string)
{
    _ic = new IllustrationLogicCanvas();
    _sc = new ScrollPane(*_ic);
    _sc->setSize(650, 450);
    add(*_sc);
    
    _label = new Label(String("Calculation time  [ms]"));
    _label->setLocation(2, 2);
    
    _textfield = new TextField(5);
    _textfield->setLocation(2, _label->getY() +_label->getHeight() +2);
    _textfield->addActionListener(*this);
    
    int width =180;
    int height =30;
    _button = new Button*[3];
    _button[0] = new Button(String("Solve Completely"));
    _button[0]->setBounds(2, 2 *(height +2), width, height);
    _button[0]->addActionListener(*this);
    _button[1] = new Button(String("Reset"));
    _button[1]->setBounds(2, 3 *(height +2), width, height);
    _button[1]->addActionListener(*this);
    _button[2] = new Button(String("One Cycle"));
    _button[2]->setBounds(2, 4 *(height +2), width, height);
    _button[2]->addActionListener(*this);
    
    Container* container = new Container();
    container->setBackground(Color::lightGray);
    container->setSize(width +4, 5 *(height +2));
    container->add(*_label);
    container->add(*_textfield);
    container->add(*_button[0]);
    container->add(*_button[1]);
    container->add(*_button[2]);
    add(*container);
    
    MenuBar& mb = *new MenuBar();
    setMenuBar(mb);
    mb.add(*new MenuFile());
    
    //setFile(String("iraroji/iraroji(60_90)-02.txt"));
}
IllustrationLogicSolver::~IllustrationLogicSolver(){}
//===========================================================
//===========================================================
void IllustrationLogicSolver::actionPerformed(ActionEvent& e)
{
    if(_ic->getNumVer() <=0 || _ic->getNumHor() <=0){
        return;
    }
    int elapsed = System::currentTimeMillis();
    Component* source = static_cast<Component*>(e.getSource());
    try{
        if(source == _button[0]){
            bool isfinished =false;
            while(!isfinished){
                isfinished =true;
                for(int i=0;
                    i< _ic->getNumVer() +_ic->getNumHor();
                    ++i)
                {
                    isfinished &= _ic->calculateLine(i);
                }
            }
            _ic->resetFlags(false, true);
        }else
        if(source == _button[1]){
            _ic->resetFlags(true, true);
        }else
        if(source == _button[2]){
            bool isfinished =true;
            for(int i=0;
                i< _ic->getNumVer() +_ic->getNumHor();
                ++i)
            {
                isfinished &= _ic->calculateLine(i);
            }
            if(isfinished){
                _ic->resetFlags(false, true);
            }
        }else
        if(source == _textfield){
            int line = Integer::parseInt(source->TextComponent::getText());
            if(line >=0 && line < _ic->getNumVer() +_ic->getNumHor()){
                if(_ic->calculateLine(line)){
                    _ic->resetFlags(false, true);
                }
            }
        }
        
        elapsed = System::currentTimeMillis() -elapsed;
        _label->setText(
            String("Calculation time ")
            +String(elapsed)
            +String("[ms]"));
    }catch(String s){
        _label->setText(s);
    }
    
    repaint();
}
//===========================================================
//===========================================================
void IllustrationLogicSolver::teststart()
{
    setSize(900, 550);
    setVisible(true);
}
void IllustrationLogicSolver::setFile(String& filename)
{
    _filename = filename;
    setTitle(filename);
    _ic->setNumGrid(_filename);
    _sc->setSize(650, 450);
    repaint();
}
//===========================================================
//===========================================================

//===========================================================
//===========================================================
void MenuFile::MenuOpen::Listener::actionPerformed(ActionEvent& e)
{
    MenuOpen& m
        = *static_cast<MenuOpen*>(e.getSource());
    MenuComponent* mc
        = dynamic_cast<MenuComponent*>(&m.getParent());
    MenuBar* mb
        = dynamic_cast<MenuBar*>(&mc->getParent());
    
    Frame* f
        = static_cast<Frame*>(&mb->getParent());
    
    FileDialog fd(*f, String("Open File"));
    fd.setVisible(true);
    static_cast<IllustrationLogicSolver*>(f)->setFile(
        String(fd.getDirectory())
        +String(fd.getFile()));
}
//===========================================================
//===========================================================

//===========================================================
//===========================================================
int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR lpCmdLine,
    int nCmdShow)
{
    //wchar_t programname[MAX_PATH +1];
    //GetModuleFileNameW(0, programname, MAX_PATH);
    //TestButton test(programname);
    IllustrationLogicSolver& test
        = *new IllustrationLogicSolver(String(GetCommandLineW()));
    test.teststart();
    
    JOYCAPS joycaps;
    JOYINFO joyinfo;
    cout << joyGetNumDevs() << endl;
    
    
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return msg.wParam;
}
//===========================================================
//===========================================================



