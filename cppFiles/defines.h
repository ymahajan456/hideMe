#ifndef _DEFINES_H_
#define _DEFINES_H_


// Standard Includes
//------------------------
#include <iostream>
#include <stdio.h>
#include <fstream>

// sltLib
// -----------------------
// #include <stdlib.h>
// #include <string>
#include <vector>
// #include <algorithm>
// #include <limits>
// #include <unordered_map>

// Filesystem Traversal
// -----------------------
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

// Crypto++ Includes
// --------------------
#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/base64.h>
#include <crypto++/sha.h>
#include <cryptopp/osrng.h>
#include <crypto++/filters.h>

// Utility Includes
// -------------------
#include <iomanip>
#include <sstream>
#include <ctime>
#include <limits>

// Extern Variables Management
#ifdef DEFINE_VARIABLES
#define EXTERN                  
#define INITIALIZER(...)        = __VA_ARGS__
#else
#define EXTERN                  extern
#define INITIALIZER(...)       
#endif 

using namespace std;

// CryptoPP byte as unsigned car for newer versions of GCC
namespace CryptoPP
{
    using byte = unsigned char;
}


#define PRINT_DEBUG_MSG

#ifdef PRINT_DEBUG_MSG
    #define PRINT_DEBUG_IN_FILE
    #define PRINT_DEBUG_LEVEL_0
    #define PRINT_DEBUG_LEVEL_1
    #define PRINT_DEBUG_LEVEL_2
    // #define PRINT_DEBUG_LEVEL_3
#endif

#ifdef PRINT_DEBUG_IN_FILE
    EXTERN ofstream myPrint;
#else
    #define myPrint std::cout
#endif

#endif