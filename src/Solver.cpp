
#include "Solver.h"
#include "string_utils.h"


#include <fstream>
#include <cstring>
#include <vector>
#include <cassert>
using namespace std;

bool LineHint::parse(const string& lineHintStr)
{
    vector<string> split = remove_empty_string(
        splitString(lineHintStr, " "));

    if (split.size() == 0)
        return false;

    for (string digitString : split)
        _hints.push_back(stoi(digitString));

    return true;
}

void Solver::open(const string& fileName)
{
    ifstream istream;
    istream.open(fileName);
    if (!istream)
        throw "file not found: path=" + fileName;
    readSize(istream);
    readHints(istream);
    istream.close();

    for (int i = 0; i < _numRows *_numCols; ++i) {
        _flagblack.push_back(false);
        _flagwhite.push_back(false);
    }
    for (int i = 0; i < _numRows + _numCols; ++i) {
        _ischanged.push_back(true);
    }

}

void Solver::readSize(ifstream& istream)
{
    char line[256];
    if (!istream.getline(line, 255))
        throw "file not read";

    vector<string> split = remove_empty_string(
        splitString(line, " "));
    _numRows = stoi(split[0]);
    _numCols = stoi(split[1]);
}

void Solver::readHints(ifstream& istream)
{
    char line[256];
    while (istream.getline(line, 255)) {
        string str = string(line);
        LineHint hint;
        if (!hint.parse(str))
            continue;
        _lineHints.push_back(hint);
    }
}

void Solver::solve()
{
    vector<int> unfinished;
    for (int i = 0; i< _numRows + _numCols; ++i)
        unfinished.push_back(i);

    int num = 100;
    for (int k = 0; k < num; ++k) {
        vector<int> new_unfinished;
        for (int i : unfinished)
            if (!calculateLine(i))
                new_unfinished.push_back(i);
        if (new_unfinished.size() == 0) {
            printf("num=%d\n", k);
            return;
        }
        unfinished = move(new_unfinished);
    }
}

bool LineHint::calculate(
    int indexLine,
    bool flagblack[],
    bool flagwhite[]
)
{
    return false;
}

bool Solver::calculateLine(int indexLine)
{
    int numline;
    int offset;
    int step;
    if(indexLine < _numRows){
        numline = _numCols;
        offset  = indexLine *_numCols;
        step    = 1;
    }else
    if(indexLine < _numRows +_numCols){
        numline = _numRows;
        offset  = indexLine -_numRows;
        step    = _numCols;
    }else{
        throw "error at line " + to_string(indexLine) + " - initialization";
    }

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
    if(!_ischanged[indexLine]){
        return false;
    }

    int numblack = _lineHints[indexLine].hints().size();
    int blacks[numblack];
    for(int i=0; i< numblack; ++i){
        blacks[i] = _lineHints[indexLine].hints()[i];
    }

    bool finished = calculateLine(
        indexLine,
        numline,
        offset,
        step,
        blacks,
        flagblack,
        flagwhite
    );

    //==================================================================
    int offset2 = indexLine < _numRows
        ? _numRows
        : 0;
    for(int j=0; j< numline; ++j){
        if(_flagblack[offset +step *j] != flagblack[j]
            || _flagwhite[offset +step *j] != flagwhite[j]){
            _ischanged[offset2 +j] =true;
        }
        _flagblack[offset +step *j] = flagblack[j];
        _flagwhite[offset +step *j] = flagwhite[j];
    }
    _ischanged[indexLine] =false;

    return finished;
}

bool Solver::calculateLine(
    int indexLine,
    int numline,
    int offset,
    int step,
    int blacks[],
    bool flagblack[],
    bool flagwhite[]
)
{
    int numblack = _lineHints[indexLine].hints().size();

    bool blackhead[numblack][numline];
    for(int i=0; i< numblack; ++i){
        bool doexist =true;
        for(int j=0; j< numline; ++j){
            blackhead[i][j] = true;
            doexist &= blackhead[i][j];
        }
    }

    bool prevflagblack[numline];
    bool prevflagwhite[numline];
    bool prevblackhead[numblack][numline];
    for(int j=0; j< numline; ++j){
        prevflagblack[j] = flagblack[j];
        prevflagwhite[j] = flagwhite[j];
        for(int i=0; i< numblack; ++i){
            prevblackhead[i][j] = blackhead[i][j];
        }
    }
    bool ismuch =false;

    while(!ismuch){

        vector<Chamber> chambers = createChambers(numline, flagwhite);

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

        for (auto& chamber : chambers) {
            for(int i = 0; i < numblack; ++i){
                for(int j = chamber.head;
                    j < chamber.head + chamber.size;
                    ++j)
                {
                    if (chamber.size < blacks[i]) {
                        blackhead[i][j] =false;
                    } else if(blackhead[i][j]
                        && j +blacks[i] > chamber.head + chamber.size)
                    {
                        blackhead[i][j] = false;
                    }
                }
            }
        }

        //==================================================================
{//
        int offsetblack =0;
        for (auto& chamber : chambers) {
            for(int j= chamber.head;
                j< chamber.head +chamber.size;
                ++j)
            {
                if(blackhead[offsetblack][j]){
                    int tail = j +blacks[offsetblack];
                    if(tail <= chamber.head +chamber.size){
                        ++offsetblack;
                        if(offsetblack >= numblack){
                            break;
                        }
                        for(int i= offsetblack; i< numblack; ++i){
                            for(int j2=0;
                                j2<= min(numline -1, tail);
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
        for(int k = chambers.size() -1; k >= 0; --k){
            auto& chamber = chambers[k];
            int indexwall = chamber.head + chamber.size;
            for(int j= indexwall -1;
                j>= chamber.head;
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
                            for(int j2= max(0, j-1);
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
        for (int k = 0; k < numstring; ++k){
            int countchamber = chambers.size() -1;
            while (chambers[countchamber].head > stringhead[k]){
                if(--countchamber <0){
                    throw "error at line " + to_string(indexLine) + " - black string";
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
                    for(int j= max(
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
                throw "error at line " + to_string(indexLine) + " - painting";
            }
        }
}//painting

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
    return false;
}

vector<Chamber> Solver::createChambers(
    int numline,
    bool flagwhite[]
)
{
    vector<Chamber> chambers;
    bool iswhite =true;
    for (int i = 0; i < numline; ++i){
        if(flagwhite[i]){
            iswhite =true;
        }else{
            if(iswhite){
                Chamber newChamber;
                newChamber.head = i;
                newChamber.size = 0;
                chambers.push_back(newChamber);
            }
            iswhite =false;
            chambers.back().size += 1;
        }
    }

    return chambers;
}

void Solver::printHints() {
    for (auto& lineHint: _lineHints) {
        for (int hint: lineHint.hints()) {
            printf("%d ", hint);
        }
        printf("\n");
    }
}

void Solver::printGrids() {
    for (int j = 0; j < _numCols; ++j) {
        printf("|%d", j / 10);
    }
    printf("|\n");

    for (int j = 0; j < _numCols; ++j) {
        printf("|%d", j % 10);
    }
    printf("|\n");

    for (int i = 0; i < _numRows; ++i) {
        for (int j = 0; j < _numCols; ++j) {
            printf("|%c",
                _flagblack[i * _numCols + j] ? '#' :
                _flagwhite[i * _numCols + j] ? ' ' :
                '.');
        }
        printf("|%02d|\n", i);
    }
}

void Solver::printHints(int index)
{
    if (index < 0 || index >= numRows() + numCols())
        throw "index out of range";

    for (int hint : _lineHints[index].hints()) {
        printf("|%d", hint);
    }
    printf("|\n");
}

void Solver::printLine(int index)
{
    if (index < 0 || index >= numRows() + numCols())
        throw "index out of range";

    int rowBegin = 0;
    int rowEnd = numRows();
    int colBegin = 0;
    int colEnd = numCols();

    if (index < numRows()) {
        rowBegin = index;
        rowEnd = index + 1;
    } else {
        colBegin = index;
        colEnd = index + 1;
    }

    for (int i = rowBegin; i < rowEnd; ++i) {
        for (int j = colBegin; j < colEnd; ++j) {
            printf("|%c",
                _flagblack[i * numCols() + j] ? '#' :
                _flagwhite[i * numCols() + j] ? ' ' :
                '.');
        }
    }
    printf("|\n");
}

