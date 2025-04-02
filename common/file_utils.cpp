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
    std::string szCommand;

    if(szPath.empty()) {
        errno = EINVAL;
        std::cerr << __FUNCTION__ << ": "
            << "Invalid input!";
        return false;
    }

    if(bSaveParentPath) {
        szCommand = "rm -rf \"" + szPath + "/*\"";
    } else {
        szCommand = "rm -rf \"" + szPath + "\"";
    }
    if(system(szCommand.c_str()) < 0) {
        std::cerr << __FUNCTION__ << ": "
            << "Failed to remove directory, err: "
            << std::strerror(errno);

        return false;
    }

    return true;
}
