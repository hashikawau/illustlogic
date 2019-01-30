
#include <string>
#include <vector>
using namespace std;

std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
vector<string> splitString(
    const string& target,
    const string& delimiter,
    vector<string> accum = vector<string>()
);
vector<string> remove_empty_string(const vector<string>& target);

