
#include <iostream>
#include <vector>

#include <fstream>

#include <memory>
class IllustLogicMatrix;
typedef std::shared_ptr<IllustLogicMatrix> IllustLogicMatrixPtr;

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

/**
 *
 */
class IllustLogicMatrix
{
public:
    static IllustLogicMatrixPtr create(const std::string& inputDataFilePath);

    bool isValidSolution();

};

/**
 *
 */
IllustLogicMatrixPtr IllustLogicMatrix::create(const std::string& inputDataFilePath)
{
    std::string delimiter = " ";

    //-----------------------------------------------------
    // open file
    //-----------------------------------------------------
    std::ifstream file{inputDataFilePath};
    if(!file)
        throw std::runtime_error{"permission denied"};

    //-----------------------------------------------------
    // read number-of-rows, number-of-columns
    //-----------------------------------------------------
    int numberOfRows;
    int numberOfCols;
    for(std::string line; std::getline(file, line); )
    {
        boost::algorithm::trim(line);
        if(line.empty())
            continue;

        if(line.at(0) == '#')
            continue;

        std::vector<std::string> words;
        boost::algorithm::split(words, line, delimiter);
        // TODO: check format
        numberOfRows = words.at(0);
        numberOfCols = words.at(1);
    }

    //-----------------------------------------------------
    // read row
    //-----------------------------------------------------
    // TODO: rename variable "hintsOfRows"
    std::vector<std::vector<int>> hintsOfRows;
    {
        const int maxNumberOfLines =numberOfRows;
        std::vector<std::vector<int>>& hintsOfLines = hintsOfRows;

        hintsOfLines.resize(maxNumberOfLines);
        int count =0;
        for(std::string line; std::getline(file, line); )
        {
            boost::algorithm::trim(line);
            if(line.empty())
                continue;

            if(line.at(0) == '#')
                continue;

            std::vector<std::string> words;
            boost::algorithm::split(words, line, delimiter);

            // TODO: check format
            std::vector<int> hint;
            for(const std::string& word: words)
            {
                hint.push_back(std::stoi(word));
            }
            hintsOfLines.push_back(std::move(hint));

            // termination condition
            ++count;
            if(count >= maxNumberOfLines)
                break;

        }
    }

    //-----------------------------------------------------
    // read columns
    //-----------------------------------------------------
    // TODO: rename variable "hintsOfCols"
    std::vector<std::vector<int>> hintsOfCols;
    {
        const int maxNumberOfLines =numberOfCols;
        std::vector<std::vector<int>>& hintsOfLines = hintsOfCols;

        hintsOfLines.resize(maxNumberOfLines);
        int count =0;
        for(std::string line; std::getline(file, line); )
        {
            boost::algorithm::trim(line);
            if(line.empty())
                continue;

            if(line.at(0) == '#')
                continue;

            std::vector<std::string> words;
            boost::algorithm::split(words, line, delimiter);

            // TODO: check format
            std::vector<int> hint;
            for(const std::string& word: words)
            {
                hint.push_back(std::stoi(word));
            }
            hintsOfLines.push_back(std::move(hint));

            // termination condition
            ++count;
            if(count >= maxNumberOfLines)
                break;

        }
    }

    return std::make_shared<IllustLogicMatrix>();
}

/**
 *
 */
bool IllustLogicMatrix::isValidSolution()
{
    return true;
}

/**
 *
 */
class IllustLogicSolver
{
public:
    void writeSolutionInto(IllustLogicMatrixPtr matrix);

};

/**
 *
 */
void IllustLogicSolver::writeSolutionInto(IllustLogicMatrixPtr matrix)
{
    //-----------------------------------------------------
    //  problem:
    //           1
    //           1 3
    //         4 1 1 3
    //       +--------
    //      4|
    //    1 1|
    //      1|
    //    1 1|
    //      4|
    //
    //  solution: = 'B'
    //           1
    //           1 3
    //         5 1 1 3
    //       +--------
    //      3| * * *
    //    1 1| *   *
    //      4| * * * *
    //    1 1| *     *
    //      4| * * * *
    //
    //  method:
    //    process[0]
    //           1
    //           1 3
    //         5 1 1 3
    //       +--------
    //      3|   * *
    //    1 1|
    //      4| * * * *
    //    1 1| 
    //      4| * * * *
    //
    //    process[1]
    //           1
    //           1 3
    //         5 1 1 3
    //       +--------
    //      3| * * * -
    //    1 1| * - * -
    //      4| * * * *
    //    1 1| * - - *
    //      4| * * * *
    //-----------------------------------------------------
    enum BoxState: unsigned char
    {
        UNKNOWN =0,
        WHITE,
        BLACK,
    };
    typedef std::vector<std::vector<BoxState>> BoxStateMatrix;
    typedef std::vector<unsigned int> HintsOfLine;

    //-----------------------------------------------------
    // ---> test code
    const unsigned int rows =5;
    const unsigned int cols =4;

    //
    BoxStateMatrix mat(rows);
    for(int i=0; i< rows; ++i)
    {
        mat.at(i).resize(cols);
        for(int j=0; j< cols; ++j)
        {
            mat[i][j] = BoxState::UNKNOWN;
        }
    }

    //
    const std::vector<HintsOfLine> hintsofrows = {
        HintsOfLine{   3},
        HintsOfLine{1, 1},
        HintsOfLine{   4},
        HintsOfLine{1, 1},
        HintsOfLine{   4},
    };

    //
    const std::vector<HintsOfLine> hintsofcols = {
        HintsOfLine{      5},
        HintsOfLine{1, 1, 1},
        HintsOfLine{   3, 1},
        HintsOfLine{      3},
    };

    //
    auto procline = [](const std::vector<BoxState>& boxstates, const HintsOfLine& hintsofline) -> std::vector<BoxState>{
        std::vector<BoxState> result;
        return result;
    };

    //
    std::vector<decltype(procline)> tasklist;
    while(!tasklist.empty())
    {
        //std::vector<BoxState> result = tasklist.front()(boxstates, hintsofline);
    }

    // <--- test code
    //-----------------------------------------------------

}

class OutputFormat;
typedef std::shared_ptr<OutputFormat> OutputFormatPtr;
/**
 *
 */
class OutputFormat
{
public:
    static OutputFormatPtr create();
    void viewSolution(const IllustLogicMatrixPtr matrix);

};

/**
 *
 */
OutputFormatPtr OutputFormat::create()
{
    return std::make_shared<OutputFormat>();
}

/**
 *
 */
void OutputFormat::viewSolution(const IllustLogicMatrixPtr matrix)
{
}

/**
 *
 */
int main(int argc, char* argv[])
{
    //-----------------------------------------------------
    // arguments check
    //
    // argument:
    //   arg[0]
    //     input data file path
    //-----------------------------------------------------
    if(argc != 2)
    {
        std::cerr << "usage: file-path" << std::endl;
        return 1;
    }

    std::string inputDataFilePath = arg[1];
    if(!boost::filesystem::exists(inputDataFilePath))
    {
        std::cerr << "input data file not exists: " << inputDataFilePath << std::endl;
        return 1;
    }

    //-----------------------------------------------------
    // read input data file 
    //
    // input data file format:
    //   space separated values
    //     number-of-rows number-of-columns
    //
    //     row[0]
    //       :
    //
    //     column[0]
    //       :
    //
    //   csv
    //
    //   xml
    //
    //   png
    //     scanned image
    //-----------------------------------------------------
    IllustLogicMatrixPtr matrix = IllustLogicMatrix::create(inputDataFilePath);
    if(!matrix)
    {
        std::cerr << "unsupported file format: " << inputDataFilePath << std::endl;
        return 1;
    }

    //-----------------------------------------------------
    // solve the problem
    //
    //
    //-----------------------------------------------------
    {
        IllustLogicSolver solver;
        solver.writeSolutionInto(matrix);
        if(!matrix->isValidSolution())
        {
            std::cerr << "failed to solve: " << inputDataFilePath << std::endl;
            return 1;
        }
    }

    //-----------------------------------------------------
    // output solution
    //
    // output format:
    //   text
    //
    //   gui <- implement as another application?
    //
    //-----------------------------------------------------
    {
        OutputFormatPtr output = OutputFormat::create();
        output->viewSolution(matrix);
    }

    return 0;
}



