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
#include <cerrno>
#include <cstring>
#include <iostream>
#include "trace.h"

bool CreateDir(const std::string& szPath)
{
    TRACE_FUNCTION();
    std::string szCommand;

    if(szPath.empty()) {
        errno = EINVAL;
        std::cerr << __FUNCTION__ << ": "
            << "Invalid input!";
        return false;
    }

    szCommand = "mkdir -p \"" + szPath + "\"";
    if(system(szCommand.c_str()) < 0) {
        std::cerr << __FUNCTION__ << ": "
            << "Failed to craete directory, err: "
            << std::strerror(errno);

        return false;
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
