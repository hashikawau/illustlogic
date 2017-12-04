#include <iostream>
#include <sstream>
#include <vector>
#include <memory>

#include "IllustrationLogicSolver.h"

std::vector<std::string> split(const std::string& input, char delimiter)
{
    std::istringstream stream(input);

    std::string field;
    std::vector<std::string> result;
    while (std::getline(stream, field, delimiter)) {
        result.push_back(field);
    }
    return result;
}

//std::shared_ptr<IllustrationLogicSolver> IllustrationLogicSolver::createFrom(const std::string& inputString)
void IllustrationLogicSolver::init(const std::string& inputString)
{
    if (inputString.size() == 0)
        return;

    std::vector<std::string> lines = split(inputString, '\n');
    int i;
    std::vector<std::string> rows;
    std::vector<std::string> cols;
    // load row size, column size
    for (i = 0; i < lines.size(); ++i) {
        if (lines[i].size() == 0)
            continue;

        break;
    }
    // load row hints
    for (i = i + 1; i < lines.size(); ++i) {
        // skip empty line
        if (lines[i].size() == 0)
            continue;
        break;
    }
    for (; i < lines.size(); ++i) {
        // stop loading when find empty line
        if (lines[i].size() == 0)
            break;
        rows.push_back(lines[i]);
        //std::vector<std::string> words = split(lines[i], ' ');
        //std::vector<int> hints;
        //for (std::string word: words)
        //    hints.push_back(std::stoi(word));
        //_rowHints.push_back(hints);
    }
    // load column hints
    for (i = i + 1; i < lines.size(); ++i) {
        // skip empty line
        if (lines[i].size() == 0)
            continue;
        break;
    }
    for (; i < lines.size(); ++i) {
        // stop loading when find empty line
        if (lines[i].size() == 0)
            break;
        cols.push_back(lines[i]);
        //std::vector<std::string> words = split(lines[i], ' ');
        //std::vector<int> hints;
        //for (std::string word: words)
        //    hints.push_back(std::stoi(word));
        //_colHints.push_back(hints);
    }

    // 
    for (std::string line: rows) {
        std::vector<std::string> words = split(line, ' ');
        std::vector<int> hints;
        for (std::string word: words) {
            hints.push_back(std::stoi(word));
        }
        _rowHints.push_back(LineHint(hints, cols.size()));
    }
    for (std::string line: cols) {
        std::vector<std::string> words = split(line, ' ');
        std::vector<int> hints;
        for (std::string word: words)
            hints.push_back(std::stoi(word));
        _colHints.push_back(LineHint(hints, rows.size()));
    }

    // init
    _flagblack.resize(getNumRowHints() * getNumColHints(), false);
    _flagwhite.resize(getNumRowHints() * getNumColHints(), false);
    _ischanged.resize(getNumRowHints() + getNumColHints(), true);
}

void IllustrationLogicSolver::print()
{
    for (int i = 0; i < getNumRowHints(); ++i) {
        for (int j = 0; j < getNumColHints(); ++j) {
            if (_flagblack[i * getNumColHints() + j] || !_flagwhite[i * getNumColHints() + j])
                std::cout << " ■";
            else
            if (!_flagblack[i * getNumColHints() + j] || _flagwhite[i * getNumColHints() + j])
                std::cout << " □";
            else
                std::cout << " ？";
        }
        std::cout << std::endl;
    }
    //for (LineHint line: _rowHints) {
    //    for (Status status: line.getCellStatus()) {
    //        if (status == Status::BLACK)
    //            std::cout << " ■";
    //        else
    //        if (status == Status::WHITE)
    //            std::cout << " □";
    //        else
    //            std::cout << " ？";
    //    }
    //    std::cout << std::endl;
    //}
}

void IllustrationLogicSolver::resetFlags(bool black, bool white)
{
    if(black){
        for(int i=0; i< getNumRowHints() *getNumColHints(); ++i){
            _flagblack[i] =false;
        }
    }
    if(white){
        for(int i=0; i< getNumRowHints() *getNumColHints(); ++i){
            _flagwhite[i] =false;
        }
    }
    for(int i=0; i< getNumRowHints() +getNumColHints(); ++i){
        _ischanged[i] =true;
    }
}

bool IllustrationLogicSolver::calculateLine(int indexline)
{
    int numline;
    int offset;
    int step;
    if(indexline < getNumRowHints()){
        numline = getNumColHints();
        offset  = indexline *getNumColHints();
        step    = 1;
    }else
    if(indexline < getNumRowHints() +getNumColHints()){
        numline = getNumRowHints();
        offset  = indexline -getNumRowHints();
        step    = getNumColHints();
    }else{
        throw std::string("error at line ")
            +std::to_string(indexline)
            +std::string(" - initialization");
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
    int numblack = getBlack(indexline).getNumHints();
    int blacks[numblack];
    for(int i=0; i< numblack; ++i){
        blacks[i] = getBlack(indexline).getHint(i);
    }

    //==================================================================
    bool blackhead[numblack][numline];
    for(int i=0; i< numblack; ++i){
        for(int j=0; j< numline; ++j){
            blackhead[i][j] = true;
        }
    }

    //==================================================================
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

    //==================================================================
    while(!ismuch){

        //chamber
        //==================================================================
        int numchamber = calcNumChanbers(numline, flagwhite);

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
                                    j2<= std::min(numline -1, tail);
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
                                for(int j2= std::max(0, j-1);
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
                    throw std::string("error at line ")
                        +std::to_string(indexline)
                        +std::string(" - black string");
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
                        for(int j= std::max(
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
                    throw std::string("error at line ")
                        +std::to_string(indexline)
                        +std::string(" - painting");
                }
            }
        }//painting

        //==================================================================
        //ismuch = isFinished(numline, numblack, flagblack, prevflagblack, flagwhite, prevflagwhite, blackhead, prevblackhead);
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
    int offset2 = indexline < getNumRowHints()?
        getNumRowHints():
        0;
    for(int j=0; j< numline; ++j){
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

bool IllustrationLogicSolver::algorithm_01(bool* flagblack, bool* flagwhite)
{
}

int IllustrationLogicSolver::calcNumChanbers(int numline, bool* flagwhite)
{
    int numchamber = 0;
    bool iswhite =true;
    for(int j=0; j< numline; ++j){
        if(flagwhite[j]){
            iswhite =true;
        }else{
            if(iswhite){
                ++numchamber;
            }
            iswhite =false;
        }
    }
    return numchamber;
}

bool IllustrationLogicSolver::isFinished(int numline, int numblack, bool* flagblack, bool* prevflagblack, bool* flagwhite, bool* prevflagwhite, bool** blackhead, bool** prevblackhead)
{
    bool ismuch =true;
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
    return ismuch;
}
