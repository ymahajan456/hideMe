#include "defines.h"
#include "Archive.h"
#include "utility.h"

bool FileSystemTree::generateFSTree(string inFolderPath, string folderName, FolderNode * parent)
{
    DIR *directory = opendir(inFolderPath.c_str());
    string path;
    struct dirent *dirp;
    struct stat fileStat;
    FolderNode *nFolderPtr;
    FileNode *nFilePtr;
    size_t tmpSizeT;

    // Check for error
    if (directory == NULL)
    {
        #ifdef PRINT_DEBUG_LEVEL_0
        myPrint << "🚨 Error: " << errno << " while tryting to open directory " 
            << inFolderPath << endl;
        #endif
        return false;
    }

    // create new folder node
    nFolderPtr = new FolderNode();

    if(parent == NULL)
    {
        tmpSizeT = inFolderPath.find_last_of("/\\");
        nFolderPtr->name = inFolderPath.substr(tmpSizeT + 1);
        nFolderPtr->relativePath = ".";
        this->root = nFolderPtr;
    }
    else
    {
        // add node to parents children list
        parent->children.push_back(nFolderPtr);
        nFolderPtr->parent = parent;
        nFolderPtr->name = folderName;
        nFolderPtr->relativePath = parent->relativePath + '/' + folderName;
    }
    // push the node to list
    this->fsNodes.push_back(nFolderPtr);

    #ifdef PRINT_DEBUG_LEVEL_3
    myPrint << "Folder: " << nFolderPtr->name << endl;
    #endif

    // create children nodes
    while(dirp = readdir(directory))
    {
        path = inFolderPath + "/" + dirp->d_name;

        if(strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
        {
            continue;
        }


        // read filestat
        if(stat(path.c_str(), &fileStat))
        {
            #ifdef PRINT_DEBUG_LEVEL_0
                myPrint << "🚨 Error: " << errno << " while tryting to read fileStat for " 
                    << path << endl;
            #endif
            return false;
        }
        else
        
        // check if child is folder
        if(S_ISDIR(fileStat.st_mode))
        {
            // recursive call to child directory
            this->generateFSTree(path, dirp->d_name, nFolderPtr);
        }
        else
        {
            // create new file node
            nFilePtr = new FileNode();
            nFolderPtr->children.push_back(nFilePtr);
            nFilePtr->parent = nFolderPtr;
            nFilePtr->name = dirp->d_name;
            nFilePtr->relativePath = nFolderPtr->relativePath + '/' + dirp->d_name;
            nFilePtr->size = fileStat.st_size;
            nFilePtr->timeData[0] = fileStat.st_ctime;
            nFilePtr->timeData[1] = fileStat.st_mtime;
            nFilePtr->timeData[2] = fileStat.st_atime;
            this->fsNodes.push_back(nFilePtr);
            #ifdef PRINT_DEBUG_LEVEL_3
                myPrint << "File: " << dirp->d_name << endl;
            #endif
        }
    }
}

FileSystemTree::FileSystemTree(string inFolderPath)
{
    bool status = this->generateFSTree(inFolderPath);
}

void FileSystemTree::printNodeData(FileSystemNode * fsNode, int printCtrl, FancyString prefix, int lineSize)
{
    string printStr = prefix.fString;
    lineSize += prefix.wOffset;

    string tmpString;
    if(printCtrl & PRINT_ICON)
    {
        printStr += (fsNode->isFolder) ? FOLDER_ICON.fString : FILE_ICON.fString ;
        lineSize += (FOLDER_ICON.wOffset + FILE_ICON.wOffset);
    }
    
    printStr += fsNode->name + "  ";
    printStr.resize(lineSize - 2, '.');
    printStr+= "  ";

    if(printCtrl & PRINT_ID)
    {
        tmpString = to_string(fsNode->id);
        tmpString.resize(5, ' ');
        printStr += tmpString;
    }

    // only applicable for file type nodes
    if(! fsNode->isFolder)
    {
        if(printCtrl & PRINT_SIZE)
        {
            tmpString = getRedableSize( static_cast<FileNode*>(fsNode)->size );
            tmpString = std::string(10-tmpString.length(), ' ') + tmpString;
            printStr += tmpString;
        }

        if(printCtrl & PRINT_C_TIME)
        {
            printStr += getRedableTime( static_cast<FileNode*>(fsNode)->timeData[0]);
        }

        if(printCtrl & PRINT_M_TIME)
        {
            printStr += getRedableTime( static_cast<FileNode*>(fsNode)->timeData[1]);
        }

        if(printCtrl & PRINT_A_TIME)
        {
            printStr += getRedableTime( static_cast<FileNode*>(fsNode)->timeData[1]);
        }
    }

    myPrint << printStr << endl;

}

void FileSystemTree::printUnwrap(FileSystemNode * root, FancyString prefix, int level, bool last, int printCtrl, int limitLevel)
{
    if (level >= limitLevel)
        return;
    
    // print node data
    this->printNodeData(root, printCtrl, (level > 0) ? (prefix + (last ? BOX_ICON_END : BOX_ICON_MID)) : FANCY_BLANK);
    
    if(root->isFolder)
    {
        // folder Node
        prefix += (level>0) ? (last ? BOX_ICON_BLANK : BOX_ICON_PASS) : FANCY_BLANK;
        level++;
        for(auto child = static_cast<FolderNode*>(root)->children.begin(); child != static_cast<FolderNode*>(root)->children.end()-1; child++)
        {
            this->printUnwrap(*child, prefix, level, false, printCtrl, limitLevel);
        }
        this->printUnwrap(static_cast<FolderNode*>(root)->children.back(), prefix, level, true, printCtrl, limitLevel);
    }
}

void FileSystemTree::print(FileSystemNode * root, int levels, int printCtrl)
{
    this->printUnwrap(root, FANCY_BLANK, 0, true, printCtrl, (levels == 0) ? std::numeric_limits<int>::max() : levels);
}

void FileSystemTree::simplePrint(FileSystemNode * root)
{
    myPrint << root->name << endl;
    if(root->isFolder)
    {
        for(auto child : static_cast<FolderNode*>(root)->children)
        {
            this->simplePrint(child);
        }
    }
}
