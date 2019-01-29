

#include "Solver.h"

#include <iostream>
#include <vector>
#include <fstream>
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
    cout << "line=" << line << endl;

    vector<string> split = remove_empty_string(
        splitString(line, " "));
    for (auto s : split)
        cout << "s=" << s << endl;
    _numRows = stoi(split[0]);
    _numCols = stoi(split[1]);
}

vector<int> read(ifstream& istream)
{
    char ch;
    char str[10];
    //LinkedList<int> list;
    vector<int> list;
    int count =0;
    while(istream.get(ch)){
        if(ch == '\n'){
            if(count){
                str[count] = '\0';
                list.push_back(stoi(str));
            }
            list.push_back(0);
            count =0;
        }else
        if(ch == ' '){
            str[count] = '\0';
            list.push_back(stoi(str));
            count =0;
        }else{
            str[count] = ch;
            ++count;
        }
    }

    if(count){
        str[count] = '\0';
        list.push_back(stoi(str));
    }
    list.push_back(0);

    return list;
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
    vector<int> list = read(istream);
    istream.close();

    printf("rows=%d, cols=%d\n", _numRows, _numCols);

    list.insert(list.begin() + 0, _numRows);
    list.insert(list.begin() + 1, _numCols);
    list.insert(list.begin() + 2, 0);

    //for(auto n: list)
    //    cout << n << endl;

    int size = list.size();
    int *array = &list[0];

    _blacks   = new int* [_numRows +_numCols];
    _numblack = new char [_numRows +_numCols];

    int start =0;
    int index =-1;
    int count =0;
    int countzero =0;
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

    for (int i = 0; i < _numRows +_numCols; ++i) {
        for (int j = 0; j < _numblack[i]; ++j) {
            printf("%d ", _blacks[i][j]);
        }
        printf("\n");
    }

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

