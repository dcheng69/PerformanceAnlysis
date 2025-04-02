#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include "file_utils.h"

#define TEST_ENTRIES_NUM        500

// Create TEST_ENTRIES_NUM different folders
void TestCreateDir() {
    for (int i = 0; i < TEST_ENTRIES_NUM; ++i) {
        std::string szDirPath = "test_dir_" + std::to_string(i);
        if (!CreateDir(szDirPath)) {
            std::cerr << "Failed to create directory: " << szDirPath << std::endl;
        }
    }

    return;
}

// Create test data for testing
void CreateFilesAndLinks(const std::string& szDirPath, int iNumFiles, int iNumDirs, int iNumSymlinks) {
    // Create files
    for (int i = 0; i < iNumFiles; ++i) {
        std::string filePath = szDirPath + "/file_" + std::to_string(i);
        std::ofstream file(filePath);
        file << "This is a test file: " << filePath << std::endl;
        file.close();
    }

    // Create sub folders
    for (int i = 0; i < iNumDirs; ++i) {
        std::string subDirPath = szDirPath + "/dir_" + std::to_string(i);
        CreateDir(subDirPath);
    }

    // Create symlinks
    for (int i = 0; i < iNumSymlinks; ++i) {
        std::string linkPath = szDirPath + "/link_" + std::to_string(i);
        std::string targetPath = szDirPath + "/file_" + std::to_string(i % iNumFiles);
        symlink(targetPath.c_str(), linkPath.c_str());
    }

    return;
}

// Test RemoveDir function to delete directory
void TestRemoveDir() {
    for (int i = 0; i < TEST_ENTRIES_NUM; ++i) {
        std::string szDirPath = "test_dir_" + std::to_string(i);

        CreateFilesAndLinks(szDirPath, 10, 5, 5);

        if (!RemoveDir(szDirPath, false)) {
            std::cerr << "Failed to remove directory: " << szDirPath << std::endl;
        }
    }
}

int main() {
    // Test CreateDir
    std::cout << "Testing CreateDir..." << std::endl;
    TestCreateDir();

    // Test RemoveDir
    std::cout << "Testing RemoveDir..." << std::endl;
    TestRemoveDir();

    return 0;
}
