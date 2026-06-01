#ifndef __STRING_UTILITY_H__
#define __STRING_UTILITY_H__

#include <string>
#include <vector>

class StringUtility
{
public:
    static void split(std::vector<std::string>& output, const std::string& input, const std::string& delimiter)
    {
        output.clear();
        
        if (input.empty()) return;
        
        size_t start = 0;
        size_t end = input.find(delimiter);
        
        while (end != std::string::npos)
        {
            output.push_back(input.substr(start, end - start));
            start = end + delimiter.length();
            end = input.find(delimiter, start);
        }
        
        output.push_back(input.substr(start));
    }
    
    static void split(std::vector<std::string>& output, const std::string& input, const char* delimiter)
    {
        split(output, input, std::string(delimiter));
    }
};

#endif // __STRING_UTILITY_H__
