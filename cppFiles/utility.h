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

#endif