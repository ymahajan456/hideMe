#define DEFINE_VARIABLES
// #include "Archive.h"
#include <iostream>
#include <stdio.h>
#include <ctime>
using namespace std;

#define BOX_CHAR_MID "├───"
#define BOX_CHAR_END "└───"
#define BOX_CHAR_PASS "│   "

#define HEX_0 0
#define HEX_1 1
#define HEX_2 2
#define HEX_3 3
#define HEX_4 4

class Base
{
    public:
    int base_int = 18;
};

class Derived : public Base
{
    public:
    int derived_int = 19;
};

int main()
{
    // std::cout << BOX_CHAR_MID << endl << BOX_CHAR_PASS << endl << BOX_CHAR_END << endl;
    // printf("|%5s|%5s|%5s|%5s|\n", "te", "tee", "teee", "teeeee");
    // printf("|%5s|%5s|%5s|%5s|\n", "te", "tee", "teee", "teeeee");

    // cout << (HEX_0 | HEX_1 | HEX_2 | HEX_3 | HEX_4) << endl;
    // cout << (HEX_0 & HEX_1 & HEX_2 & HEX_3 & HEX_4) << endl;

    // Base* basepointer = new Derived();
    // cout << static_cast<Derived*>(basepointer)->derived_int;

    std::time_t now = std::time(NULL);
    std::tm * ptm = std::localtime(&now);
    char buffer[32];
    // Format: Mo, 15.06.2009 20:20:00
    std::strftime(buffer, 32, "%a, %d-%m-%y %H:%M:%S", ptm);
    cout << buffer << endl;
}