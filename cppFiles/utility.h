#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "defines.h"

// Extern Variables Management
#ifdef DEFINE_VARIABLES
#define EXTERN                  
#define INITIALIZER(...)        = __VA_ARGS__
#else
#define EXTERN                  extern
#define INITIALIZER(...)       
#endif

string convertToString(double num);
double roundOff(double n);
string getRedableSize(size_t size);
string getRedableTime(time_t inTime);

struct FancyString
{
    std::string fString;
    int wOffset;

    struct FancyString& operator+=(const FancyString & rhs);
};

struct FancyString operator+(FancyString lhs ,const FancyString & rhs);

EXTERN struct FancyString BOX_ICON_MID INITIALIZER({"├── ", 6});
EXTERN struct FancyString BOX_ICON_END INITIALIZER({"└── ", 6});
EXTERN struct FancyString BOX_ICON_PASS INITIALIZER({"│   ", 2});
EXTERN struct FancyString BOX_ICON_BLANK INITIALIZER({"    ", 0});
EXTERN struct FancyString FANCY_BLANK INITIALIZER({"", 0});

// #define BOX_ICON_MID "├── "
// #define BOX_ICON_END "└── "
// #define BOX_ICON_PASS "│   "
// #define BOX_ICON_BLANK "    "

// #define BOX_ICON_MID "    "
// #define BOX_ICON_END "    "
// #define BOX_ICON_PASS "    "
// #define BOX_ICON_BLANK "    "

EXTERN struct FancyString FOLDER_ICON INITIALIZER({"📂 ", 1});
EXTERN struct FancyString FILE_ICON INITIALIZER({"📃 ", 1});

string centreAligned(string in, int outSize, char fillChar = ' ');

// terminal based selection
vector<int> terminalBasedSelector(const string message = "Enter selections");


template <typename T_LHS, typename T_RHS>
struct Eraser
{
    Eraser(const T_RHS & toBeRemoved) : i_toBeRemoved(toBeRemoved){}
    T_RHS i_toBeRemoved;
    bool operator()(T_LHS & element) const
    {
        return element == i_toBeRemoved;
    }
};

template <typename T_LHS, typename T_RHS>
struct PointerEraser
{
    PointerEraser(const T_RHS & toBeRemoved) : i_toBeRemoved(toBeRemoved){}
    T_RHS i_toBeRemoved;
    bool operator()(T_LHS element) const
    {
        return (*element) == i_toBeRemoved;
    }
};




#endif