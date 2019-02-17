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
    bool childrenStatus = true;

    // Check for error
    if (directory == NULL)
    {
        #ifdef PRINT_DEBUG_LEVEL_0
        DEBUG_PRINT << "🚨 Error: " << errno << " while tryting to open directory " 
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
    DEBUG_PRINT << "Folder: " << nFolderPtr->name << endl;
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
                DEBUG_PRINT << "🚨 Error: " << errno << " while tryting to read fileStat for " 
                    << path << endl;
            #endif
            return false;
        }
        else
        
        // check if child is folder
        if(S_ISDIR(fileStat.st_mode))
        {
            // recursive call to child directory
            childrenStatus &= this->generateFSTree(path, dirp->d_name, nFolderPtr);
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
                DEBUG_PRINT << "File: " << dirp->d_name << endl;
            #endif
        }
    }

    return childrenStatus;
}

FileSystemTree::FileSystemTree(string inFolderPath)
{
    bool status = this->generateFSTree(inFolderPath);
    if(status)
    {
        this->asssignIds();
    }
}

void FileSystemTree::printNodeData(FileSystemNode * fsNode, int printCtrl, FancyString prefix, int lineSize, ostream & stream)
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
    printStr.resize(lineSize - 2, (printCtrl & PRINT_DOTTED_LINE) ? '.' : ' ');
    printStr+= "  ";

    if(printCtrl & PRINT_ID)
    {
        tmpString = to_string(fsNode->id);
        tmpString = std::string(5-tmpString.length(), ' ') + tmpString;
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

    stream << printStr << endl;

}

void FileSystemTree::printUnwrap(FileSystemNode * root, FancyString prefix, int level, bool last, int printCtrl, int limitLevel, int printWidth, ostream & stream)
{
    if (level >= limitLevel)
        return;
    
    // print node data
    this->printNodeData(root, printCtrl, (level > 0) ? (prefix + ((printCtrl & PRINT_BOX_CHARS) ? (last ? BOX_ICON_END : BOX_ICON_MID) : BOX_ICON_BLANK)) : FANCY_BLANK, printWidth, stream);
    
    if(root->isFolder)
    {
        // folder Node
        prefix += (level>0) ? ((last ||!(printCtrl & PRINT_BOX_CHARS) ) ? BOX_ICON_BLANK : BOX_ICON_PASS) : FANCY_BLANK;
        level++;
        for(auto child = static_cast<FolderNode*>(root)->children.begin(); child != static_cast<FolderNode*>(root)->children.end()-1; child++)
        {
            this->printUnwrap(*child, prefix, level, false, printCtrl, limitLevel, printWidth, stream);
        }
        this->printUnwrap(static_cast<FolderNode*>(root)->children.back(), prefix, level, true, printCtrl, limitLevel, printWidth, stream);
    }
}

void FileSystemTree::print(FileSystemNode * root, int levels, int printCtrl, int printWidth, ostream & stream)
{
    stream << std::string(printWidth, '-') << endl;
    string postHeader = "";
    int nameWidth = printWidth;
    if(printCtrl & PRINT_ID)
    {
        nameWidth -= 5;
        postHeader += "  Id ";
    }
    if(printCtrl & PRINT_SIZE)
    {
        nameWidth -= 10;
        postHeader += "   Size   ";
    }
    if(printCtrl & PRINT_C_TIME)
    {
        nameWidth -= 10;
        postHeader += "  C_TIME  ";
    }
    if(printCtrl & PRINT_M_TIME)
    {
        nameWidth -= 10;
        postHeader += "  M_TIME  ";
    }
    if(printCtrl & PRINT_A_TIME)
    {
        nameWidth -= 10;
        postHeader += "  A_TIME  ";
    }

    stream << centreAligned("File Names", nameWidth) << postHeader << endl;
    stream << std::string(printWidth, '-') << endl;
    this->printUnwrap(root, FANCY_BLANK, 0, true, printCtrl, (levels == 0) ? std::numeric_limits<int>::max() : levels, nameWidth, stream);
}

void FileSystemTree::simplePrint(FileSystemNode * root)
{
    DEBUG_PRINT << root->name << endl;
    if(root->isFolder)
    {
        for(auto child : static_cast<FolderNode*>(root)->children)
        {
            this->simplePrint(child);
        }
    }
}


void FileSystemTree::asssignIds()
{
    for(int i = 0; i < this->fsNodes.size(); i++)
    {
        this->fsNodes[i]->id = i;
    }
}


void FileSystemTree::removeSubTrees(vector<int> & removeList)
{
    vector<int> removeLocs;
    #ifdef PRINT_DEBUG_LEVEL_3
        DEBUG_PRINT << "Remove requests: ";
        for(auto element : removeList)
            DEBUG_PRINT << element << ' ';
        DEBUG_PRINT << endl;
    #endif
    std::vector<FileSystemNode *>::iterator it;
    for(auto loc : removeList)
    {
        // disconnect parent connections
        static_cast<FolderNode*>(this->fsNodes[loc]->parent)->children.erase(
            std::remove_if(static_cast<FolderNode*>(this->fsNodes[loc]->parent)->children.begin(), 
                static_cast<FolderNode*>(this->fsNodes[loc]->parent)->children.end(),
                PointerEraser<FileSystemNode *, int>(loc)),
            static_cast<FolderNode*>(this->fsNodes[loc]->parent)->children.end()
        );
        vector<int> partialList = getRecursiveLocs(this->fsNodes[loc]);
        removeLocs.insert(removeLocs.end(), partialList.begin(), partialList.end() );
    }
    // remove duplicate entries if any
    std::sort(removeLocs.begin(), removeLocs.end());
    removeLocs.erase(std::unique(removeLocs.begin(), removeLocs.end()), removeLocs.end());

    #ifdef PRINT_DEBUG_LEVEL_3
        DEBUG_PRINT << "Complete Remove List: ";
        for(auto element : removeLocs)
            DEBUG_PRINT << element << ' ';
        DEBUG_PRINT << endl;
    #endif

    // delete the node objects and vector entries
    for(int i = 0; i < removeLocs.size(); i ++)
    {
        it = (this->fsNodes.begin() + removeLocs[i] - i);
        #ifdef PRINT_DEBUG_LEVEL_0
            DEBUG_PRINT << "Removing node : " << (*it)->id << endl;
        #endif
        // // delete object
        delete *it;
        // // delete pointer
        this->fsNodes.erase(it);
    }
    // reassign ids
    this->asssignIds();
}

vector<int> FileSystemTree::getRecursiveLocs(FileSystemNode * root)
{
    vector<int> locs = {root->id};
    if(root->isFolder)
    {
        for(auto child : static_cast<FolderNode*>(root)->children)
        {
            vector<int> partialLocs = getRecursiveLocs(child);
            locs.insert(locs.end(), partialLocs.begin(), partialLocs.end());
        }
    }
    return locs;
}