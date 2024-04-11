//============================================================================
// Name        : hashskeleton.cpp
// Author      : Faculty of Engineering, Cairo University
// Version     :
// Description : Hashing using Extendible hashing
//============================================================================

#include <iostream>
#include "ExtendiableHashing.h"

int main()
{

    double score = 0.0;

    // Initialize File
    GlobalDirectory globalDirectory;
    Bucket initialFile;
    // Empty Directory
    displayDirectory(globalDirectory, initialFile, 1);
    //============================================================

    // test insertion without making extra directory
    insertItem(DataItem(13, 33), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    //============================================================
    // search
    searchItem(13, initialFile, globalDirectory);
    // delete item
    deleteItem(13, initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    //============================================================

    ////////////////////////////////////////////////////////////

    insertItem(DataItem(1, 20), initialFile, globalDirectory);
    insertItem(DataItem(2, 70), initialFile, globalDirectory);
    std::flush(std::cout);
    insertItem(DataItem(42, 11), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    //============================================================
    insertItem(DataItem(12, 54), initialFile, globalDirectory);
    std::flush(std::cout);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    insertItem(DataItem(240, 16), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    insertItem(DataItem(241, 99), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    //============================================================
    insertItem(DataItem(159, 30), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    //============================================================

    insertItem(DataItem(14, 45), initialFile, globalDirectory);
    insertItem(DataItem(13, 88), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    //============================================================
    insertItem(DataItem(37, 1), initialFile, globalDirectory);
    searchItem(13, initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    //============================================================
    deleteItem(14, initialFile, globalDirectory);
    std::flush(std::cout);
    deleteItem(13, initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    //============================================================
    insertItem(DataItem(158, 5), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    insertItem(DataItem(3, 9), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    deleteItem(158, initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    deleteItem(2, initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    deleteItem(42, initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    std::flush(std::cout);
    // std::cout << "Press any key to continue\n";
    // char t[1];
    // std::cin >> t;
    // return 0;
}
