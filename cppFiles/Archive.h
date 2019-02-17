#ifndef _ARCHIVER_H_
#define _ARCHIVER_H_

#include "defines.h"
#include "utility.h"

// Extern Variables Management
#ifdef DEFINE_VARIABLES
#define EXTERN                  
#define INITIALIZER(...)        = __VA_ARGS__
#else
#define EXTERN                  extern
#define INITIALIZER(...)       
#endif

#define PRINT_ID                1<<0
#define PRINT_SIZE              1<<1
#define PRINT_C_TIME            1<<2
#define PRINT_M_TIME            1<<3
#define PRINT_A_TIME            1<<4
#define PRINT_ICON              1<<5
#define PRINT_BOX_CHARS         1<<6
#define PRINT_DOTTED_LINE       1<<7
#define DEFAULT_PRINT_CONFIG    (PRINT_ID | PRINT_ICON | PRINT_SIZE | PRINT_BOX_CHARS | PRINT_DOTTED_LINE)

// struct DataDisplayChoice
// {
//     bool PRINT_ID            : 1;  // 1
//     bool PRINT_SIZE          : 1;  // 2
//     bool PRINT_C_TIME        : 1;  // 3
//     bool PRINT_M_TIME        : 1;  // 4
//     bool PRINT_A_TIME        : 1;  // 5
//     bool PRINT_ICON          : 1;  // 6
//     bool PRINT_BOX_CHARS     : 1;  // 7
//     bool PRINT_DOTTED_LINE   : 1;  // 8
// };

// EXTERN struct DataDisplayChoice DEFAULT_CHOICE INITIALIZER({true, true, false, false, false, true, true, true});

class FileSystemNode
{
    public:
    bool isFolder = false;
    int id = -1;
    string name = "";
    string relativePath = "";
    string description = "";
    FileSystemNode * parent = NULL;

    virtual ~FileSystemNode(){}
    bool operator==(const FileSystemNode & rhs){return this->id == rhs.id;}
    bool operator==(const int rhs){return this->id == rhs;}
};

class FolderNode : public FileSystemNode
{
    public:
    vector<FileSystemNode *> children;
    FolderNode(void)
    {
        isFolder = true;
    }

    ~FolderNode(){}
};

class FileNode : public FileSystemNode
{
    public:
    size_t size = 0;
    // [c_time, m_time, a_time]
    time_t timeData[3];

    ~FileNode(){}
};

class FileSystemTree
{
    private:
    void printNodeData(FileSystemNode * fsNode, int printCtrl, FancyString prefix, int lineSize = 80, ostream & stream = std::cout);
    void printUnwrap(FileSystemNode * root, FancyString prefix, int level = 0, bool last = false, int printCtrl = (1<<6)-1, int limitLevel = std::numeric_limits<int>::max(), int printWidth = 80, ostream & stream = std::cout);
    bool generateFSTree(string inFolderPath, string folderName = "", FolderNode * parent = NULL);
    void asssignIds();
    vector<int> getRecursiveLocs(FileSystemNode * root);
    public:
    vector<FileSystemNode *> fsNodes;
    FileSystemNode * root = NULL;
    FileSystemTree(string inFolderPath);
    // print functions
    void print(FileSystemNode * root, int levels = 0, int printCtrl = DEFAULT_PRINT_CONFIG , int printWidth = 80, ostream & stream = std::cout);
    void print(){this->print(this->root);};
    void print(int levels, ostream & stream = std::cout){this->print(this->root, levels, DEFAULT_PRINT_CONFIG, 80, stream);}
    void simplePrint(FileSystemNode * root);
    void removeSubTrees(vector<int> & removeList);
};

// class Archive
// {
//     public:

//     Archive(string inFolderPath, string outFolderPath = "./archives/", bool excludeFiles = false);

// };

#endif