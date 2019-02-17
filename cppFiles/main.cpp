#define DEFINE_VARIABLES
#include "Archive.h"


int main()
{
    #ifdef PRINT_DEBUG_IN_FILE
        debugFile.open("debugMsg.dbg");
    #endif
    FileSystemTree test("/mnt/246A0BEC6A0BB996/WorkingArea/hideMe/cppFiles");
    test.print();
    vector<int> selection = terminalBasedSelector("Select files to be removed");
    test.removeSubTrees(selection);
    test.print();
    // test.print();
}
