#include "defines.h"
#include "utility.h"

string convertToString(double num)
{
    ostringstream convert;
    convert << num; 
    return convert.str();
}

double roundOff(double n)
{
    double d = n * 100.0;
    int i = d + 0.5;
    d = (float)i / 100.0;
    return d;
}

string getRedableSize(size_t size)
{
    static const char *SIZES[] = { "B", "KB", "MB", "GB" };
    int div = 0;
    size_t rem = 0;

    while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES)) {
        rem = (size % 1024);
        div++;
        size /= 1024;
    }

    double size_d = (float)size + (float)rem / 1024.0;
    string result = convertToString(roundOff(size_d)) + " " + SIZES[div];
    return result;
}

string getRedableTime(time_t inTime)
{
    std::tm * ptm = std::localtime(&inTime);
    char buffer[10];
    std::strftime(buffer, 10, " %d-%m-%y ", ptm);
    return string(buffer);
}

struct FancyString& FancyString::operator+=(const FancyString & rhs){fString += rhs.fString; wOffset += rhs.wOffset; return *this;}

struct FancyString operator+(FancyString lhs ,const FancyString & rhs)
{
    return lhs += rhs;
}