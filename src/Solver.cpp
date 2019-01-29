

#include "Solver.h"

#include <iostream>

#include <fstream>
#include <cstring>
#include <vector>
using namespace std;

vector<string> splitString(
    const string& target,
    const string& delimiter,
    vector<string> accum = vector<string>()
)
{
    if (delimiter.length() == 0) {
        accum.push_back(target);
        return accum;
    }

    auto pos = target.find(delimiter);
    if (pos == string::npos) {
        accum.push_back(target);
        return accum;
    } else {
        accum.push_back(target.substr(0, pos));
        return splitString(
            target.substr(pos + delimiter.length()),
            delimiter,
            accum);
    }
}

vector<string> remove_empty_string(const vector<string>& target)
{
    vector<string> result;
    for (auto s : target)
        if (s.length() > 0)
            result.push_back(s);
    return result;
}

Solver::Solver()
{
    _numcalclation =50;
    _numblack =0;
    _blacks   =0;

    _gridsize =10;
    _grid =0;
    _flag =0;
}

Solver::~Solver()
{
}

void Solver::readSize(ifstream& istream)
{
    char line[256];
    if (!istream.getline(line, 255))
        throw "file not read";
    //cout << "line=" << line << endl;

    vector<string> split = remove_empty_string(
        splitString(line, " "));
    for (auto s : split)
        cout << "s=" << s << endl;
    _numRows = stoi(split[0]);
    _numCols = stoi(split[1]);
}

void Solver::readHints(ifstream& istream)
{
    char line[256];
    while (istream.getline(line, 255)) {
        if (strlen(line) == 0)
            continue;

        cout << "line=" << line << endl;

        vector<string> split = remove_empty_string(
            splitString(line, " "));

        for (auto s : split)
            cout << "s=" << s << endl;
        cout << endl;

        vector<int> hintLine;
        for (string str : split)
            hintLine.push_back(stoi(str));
        _hints.push_back(hintLine);
    }
}

void Solver::open(const string& fileName)
{
    if(_blacks){
        for(int i=0; i< _numRows +_numCols; ++i){
            delete [] _blacks[i];
        }
        delete [] _blacks;
    }
    delete [] _numblack;
    delete [] _grid;
    delete [] _flag;

    //char name[255];
    ////wcstombs(name, fileName.toCharArray(), filename.length() +1);
    ifstream istream;
    istream.open(fileName);
    if (!istream)
        throw "file not found: path=" + fileName;
    readSize(istream);
    readHints(istream);
    istream.close();

    printf("rows=%d, cols=%d\n", _numRows, _numCols);
    printHints();

    _grid = new bool [_numRows *_numCols];
    _flag = new bool [_numRows *_numCols];
    for(int i=0; i< _numRows *_numCols; ++i){
        _grid[i] =false;
        _flag[i] =false;
    }

    //setSize(_gridsize *_numCols +2, _gridsize *_numRows +2);
}

void Solver::solve()
{
}

