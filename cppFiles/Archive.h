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
// #define PRINT_BOX_CHARS         1<<6
// #define PRINT_DOTTED_LINE       1<<7

class FileSystemNode
{
    public:
    bool isFolder = false;
    int id = -1;
    string name = "";
    string relativePath = "";
    string description = "";
    FileSystemNode * parent = NULL;
};

class FolderNode : public FileSystemNode
{
    public:
    vector<FileSystemNode *> children;
    FolderNode(void)
    {
        isFolder = true;
    }
};

class FileNode : public FileSystemNode
{
    public:
    size_t size = 0;
    // [c_time, m_time, a_time]
    time_t timeData[3];
};

class FileSystemTree
{
    private:
    void printNodeData(FileSystemNode * fsNode, int printCtrl, FancyString prefix, int lineSize = 80);
    void printUnwrap(FileSystemNode * root, FancyString prefix, int level = 0, bool last = false, int printCtrl = (1<<6)-1, int limitLevel = std::numeric_limits<int>::max());
    public:
    vector<FileSystemNode *> fsNodes;
    FileSystemNode * root = NULL;
    FileSystemTree(string inFolderPath);
    bool generateFSTree(string inFolderPath, string folderName = "", FolderNode * parent = NULL);
    void print(FileSystemNode * root, int levels = 0, int printCtrl = (1<<6)-1);
    void print(){this->print(this->root);};
    void simplePrint(FileSystemNode * root);
};

// class Archive
// {
//     public:

//     Archive(string inFolderPath, string outFolderPath = "./archives/", bool excludeFiles = false);

// };

#endif