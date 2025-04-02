/* Copyright (C) 
 * 2025 - Clay Cheng
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */

#include "file_utils.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string>
#include <vector>
#include <sstream>
#include <queue>
#include <stack>
#include <dirent.h>
#include "TrieNode.h"
#include "PathTrie.h"
#include "trace.h"

bool CreateDir(const std::string& szPath)
{
    TRACE_FUNCTION();
    if (szPath.empty()) {
        errno = EINVAL;
        std::cerr << __FUNCTION__ << ": Invalid input!" << std::endl;
        return false;
    }

    struct stat st;
    if (stat(szPath.c_str(), &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            return true; // Directory already existed
        } else {
            std::cerr << __FUNCTION__ << ": Path exists but is not a directory!" << std::endl;
            return false;
        }
    }

    // Split the path into components
    std::vector<std::string> vParts;
    std::stringstream ss(szPath);
    std::string item;
    
    while (std::getline(ss, item, '/')) {
        if (!item.empty()) {
            vParts.push_back(item);
        }
    }

    std::string szCurrentPath;
    if (szPath[0] == '/') {
        szCurrentPath = "/"; // Handle absolute paths
    }

    // Iterate through each part and create missing directories
    for (const auto& part : vParts) {
        if (!szCurrentPath.empty() && szCurrentPath.back() != '/') {
            szCurrentPath += "/";
        }
        szCurrentPath += part;

        if (stat(szCurrentPath.c_str(), &st) != 0) { // Check if it exists
            if (mkdir(szCurrentPath.c_str(), 0755) != 0) { // Create if missing
                if (errno != EEXIST) { // Ignore "already exists" error
                    std::cerr << __FUNCTION__ << ": Failed to create directory '"
                              << szCurrentPath << "', err: " << std::strerror(errno) << std::endl;
                    return false;
                }
            }
        } else if (!S_ISDIR(st.st_mode)) {
            std::cerr << __FUNCTION__ << ": '" << szCurrentPath << "' exists but is not a directory!" << std::endl;
            return false;
        }
    }

    return true;
}

bool RemoveDir(const std::string& szPath, bool bSaveParentPath)
{
    TRACE_FUNCTION();
    PathTrie pathTrie;
    std::queue<TrieNode*> queueDirs;
    std::stack<TrieNode*> stackDirs;

    TrieNode* pNode = pathTrie.insert(szPath);
    if(pNode) {
        queueDirs.push(pNode);
    }
    while(!queueDirs.empty()) {
        std::string currentDir = TrieNode::getFullPath(queueDirs.front());
        queueDirs.pop();

        DIR* dir = opendir(currentDir.c_str());
        if(!dir) {
            std::cerr << "Failed to open directory: " << currentDir << std::endl;
            return false;
        }

        struct dirent* entry;
        while((entry = readdir(dir)) != nullptr) {
            if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            std::string fullPath = currentDir + "/" + entry->d_name;
            struct stat st;
            if (lstat(fullPath.c_str(), &st) == 0) {
                if (S_ISLNK(st.st_mode)) {
                    // If it's a symbolic link, delete it immediately
                    if (remove(fullPath.c_str()) != 0) {
                        std::cerr << "Failed to delete symbolic link: " << strerror(errno) << std::endl;
                        return false;
                    }
                } else if (S_ISDIR(st.st_mode)) {
                    // If it's a directory, continue processing
                    pNode = pathTrie.insert(fullPath);
                    if (pNode) {
                        queueDirs.push(pNode);
                        stackDirs.push(pNode);
                    } else {
                        // Error
                        return false;
                    }
                } else {
                    // If it's a regular file, attempt to delete it
                    if (remove(fullPath.c_str()) != 0) {
                        std::cerr << "Failed to delete file: " << strerror(errno) << std::endl;
                        return false;
                    }
                }
            } else {
                std::cerr << "Failed to stat file: " << strerror(errno) << std::endl;
                return false;
            }
        }
        closedir(dir);
    }

    // Reversely rmdir until parent folder
    while(!stackDirs.empty()) {
        pNode = stackDirs.top();
        stackDirs.pop();
        std::string fullPath = TrieNode::getFullPath(pNode);
        if (fullPath != "" && rmdir(fullPath.c_str()) != 0) {
            std::cerr << "Failed to delete directory: " << fullPath << std::endl;
            return false;
        }
    }

    if (!bSaveParentPath) {
        if (rmdir(szPath.c_str()) != 0) {
            std::cerr << "Failed to delete directory: " << szPath << std::endl;
            return false;
        }
    }

    return true;
}
