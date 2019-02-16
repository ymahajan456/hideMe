#define DEFINE_VARIABLES
#include "Archive.h"


int main()
{
    #ifdef PRINT_DEBUG_IN_FILE
        myPrint.open("debugMsg.dbg");
    #endif
    FileSystemTree test("/mnt/246A0BEC6A0BB996/WorkingArea/hideMe/cppFiles");
    test.print();
}