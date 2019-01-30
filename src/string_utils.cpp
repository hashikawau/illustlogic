
#include "string_utils.h"

std::string& ltrim(std::string& str, const std::string& chars)
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string& rtrim(std::string& str, const std::string& chars)
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string& trim(std::string& str, const std::string& chars)
{
    return ltrim(rtrim(str, chars), chars);
}

vector<string> splitString(
    const string& target,
    const string& delimiter,
    vector<string> accum
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
        if (trim(s).length() > 0)
            result.push_back(s);
    return result;
}

