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


string centreAligned(string in, int outSize, char fillChar)
{
    if(in.length() >= outSize)
    {
        return in.substr(0, outSize);
    }
    string ret((outSize - in.length())/2, fillChar);
    ret += in;
    ret += std::string(outSize - ret.length(), fillChar);
    return ret;
}

// convert duplicate spaces to single spaces
bool BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }

void removeMultipleSpaces(string & input)
{
    std::string::iterator new_end = std::unique(input.begin(),input.end(), BothAreSpaces);
    input.erase(new_end, input.end());
}

vector<int> terminalBasedSelector(const string message)
{
    vector<int> selections;
    char *endPtr;
    string input;
    string section;
    long convertedData;
    char tmpChar;
    cout << endl << "➡ " << message << ":" << endl << "⬅ ";
    // ' ' is considered by default
    // ------------------------------------
    string additionalDelimeters = ".,;";
    // ------------------------------------
    while(true)
    {
        std::getline(std::cin, input);
        for(int i = 0; i < additionalDelimeters.size(); i++)
        {
            std::replace(input.begin(), input.end(), additionalDelimeters[i], ' ');
        }
        removeMultipleSpaces(input);
        // get the integers
        std::stringstream inputStream(input);
        while(std::getline(inputStream, section, ' '))
        {
            const char *charArray = section.c_str();
            convertedData = std::strtol(charArray, &endPtr, 10);
            if(!((endPtr == charArray) || (*endPtr != '\0')))
            {
                selections.push_back((int)convertedData);
            }
        }
        // confirt the selection
        cout << "➡ Selected entries are: ";
        for(auto selection : selections)
        {
            cout << selection << ' ';
        }

        cout << "\n➡ Enter 'y' to confirm | 'n' to cancel | <anything else> to reenter: ";
        std::cin >> tmpChar;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if(tmpChar == 'y')
        {
            return selections;
        }
        else if(tmpChar == 'n')
        {
            selections.clear();
            return selections;
        }
        else
        {
            selections.clear();
            cout << "⬅ ";
        }
    }
}